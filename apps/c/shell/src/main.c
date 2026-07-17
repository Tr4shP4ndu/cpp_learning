// shell — a tiny POSIX shell: run commands, built-ins, and pipelines.
// Our own implementation of the standard technique (fork + execvp + pipe + dup2);
// the tutorial that inspired it is https://github.com/tokenrove/build-your-own-shell .
//
// POSIX only (macOS / Linux; on Windows use WSL or MSYS2). The interesting part
// is process control: the shell forks a child per command and wires their
// stdin/stdout together with pipes so `ls | sort | wc -l` works.
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_ARGS 64
#define MAX_CMDS 16

// Launch every command in the pipeline, each in its own process, wiring the
// output of one into the input of the next.
static void run_pipeline(char* cmds[][MAX_ARGS], int ncmds) {
    int in_fd = 0;               // first command reads the shell's stdin
    pid_t pids[MAX_CMDS];
    for (int i = 0; i < ncmds; ++i) {
        int pipefd[2] = {-1, -1};
        int out_fd = 1;          // last command writes the shell's stdout
        if (i < ncmds - 1) {
            if (pipe(pipefd) < 0) { perror("pipe"); return; }
            out_fd = pipefd[1];
        }
        pid_t pid = fork();
        if (pid < 0) { perror("fork"); return; }
        if (pid == 0) {          // child: redirect, then become the command
            if (in_fd != 0)  { dup2(in_fd, 0);  close(in_fd); }
            if (out_fd != 1) { dup2(out_fd, 1); close(out_fd); }
            if (pipefd[0] != -1) close(pipefd[0]);
            execvp(cmds[i][0], cmds[i]);
            fprintf(stderr, "%s: command not found\n", cmds[i][0]);
            _exit(127);
        }
        pids[i] = pid;           // parent: close our copies, keep the read end
        if (in_fd != 0) close(in_fd);
        if (out_fd != 1) close(out_fd);
        in_fd = pipefd[0];
    }
    for (int i = 0; i < ncmds; ++i) waitpid(pids[i], NULL, 0);
}

int main(void) {
    char* line = NULL;
    size_t cap = 0;

    for (;;) {
        fprintf(stderr, "mysh$ ");     // prompt on stderr so piped stdout stays clean
        if (getline(&line, &cap, stdin) < 0) { fprintf(stderr, "\n"); break; }  // EOF

        // Split the line into pipeline stages on '|', then each stage into argv.
        char* cmds[MAX_CMDS][MAX_ARGS];
        int ncmds = 0;
        char* save1;
        for (char* seg = strtok_r(line, "|", &save1);
             seg && ncmds < MAX_CMDS; seg = strtok_r(NULL, "|", &save1)) {
            int n = 0;
            char* save2;
            for (char* t = strtok_r(seg, " \t\r\n", &save2);
                 t && n < MAX_ARGS - 1; t = strtok_r(NULL, " \t\r\n", &save2))
                cmds[ncmds][n++] = t;
            cmds[ncmds][n] = NULL;
            if (n > 0) ++ncmds;            // skip empty stages
        }
        if (ncmds == 0) continue;

        // Built-ins must run IN the shell process (a forked child couldn't
        // change the shell's own directory) — so handle them before forking.
        if (ncmds == 1) {
            char** a = cmds[0];
            if (strcmp(a[0], "exit") == 0) break;
            if (strcmp(a[0], "cd") == 0) {
                const char* dir = a[1] ? a[1] : getenv("HOME");
                if (dir && chdir(dir) != 0) perror("cd");
                continue;
            }
            if (strcmp(a[0], "help") == 0) {
                puts("mysh built-ins: cd [dir], exit, help");
                puts("everything else runs as a program; pipes work: ls | sort | wc -l");
                continue;
            }
        }

        run_pipeline(cmds, ncmds);
    }

    free(line);
    return 0;
}
