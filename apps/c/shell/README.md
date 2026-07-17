# shell

A tiny **POSIX shell**, in C: a prompt, built-in commands, external programs,
and **pipelines** (`ls | sort | wc -l`). The point is process control — how a
shell uses `fork`, `execvp`, `pipe`, and `dup2` to run other programs and wire
them together. This is the C-track systems counterpart to the graphics projects.

Inspired by <https://github.com/tokenrove/build-your-own-shell> (our
implementation is our own). **POSIX only** — macOS/Linux; on Windows use WSL2 or
MSYS2.

**Builds on (C track):** strings (20), pointers (39), dynamic memory (40),
arrays (18) — plus POSIX process syscalls, which are new here.

## Run
```sh
make run-c-app-shell            # interactive: type commands, `help`, `exit`
echo 'echo hello | tr a-z A-Z'  | make run-c-app-shell    # scripted
printf 'ls | wc -l\nexit\n'     | make run-c-app-shell
```
(The `mysh$ ` prompt is printed to stderr, so piped output stays clean.)

## Build it step by step
1. **A REPL.** Print a prompt, `getline` a line, loop until EOF (`Ctrl-D`).
2. **Tokenize.** Split the line into an `argv` array on whitespace.
3. **Run a program.** `fork`; in the child `execvp(argv[0], argv)`; the parent
   `waitpid`s. That's how every non-built-in command runs.
4. **Built-ins.** `cd`, `exit`, `help` must run in the *shell itself* — a forked
   child changing directory wouldn't affect the shell — so handle them before
   forking.
5. **Pipelines.** Split on `|`; for each stage create a `pipe`, `fork`, and
   `dup2` the previous read end onto stdin and this stage's write end onto
   stdout. Close the ends you don't need, then `waitpid` for all.

## Try it yourself
- I/O redirection: `>` / `<` / `>>` (open a file, `dup2` it onto fd 1 or 0).
- More built-ins: `pwd`, `export`, a `$?` exit-status variable.
- A prompt that shows the current directory (`getcwd`).
