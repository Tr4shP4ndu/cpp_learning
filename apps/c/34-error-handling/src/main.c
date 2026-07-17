/* 34-error-handling (C) — failures come back as return values plus errno. */
#include <stdio.h>
#include <stdlib.h>   /* EXIT_SUCCESS, EXIT_FAILURE */
#include <errno.h>    /* errno */
#include <string.h>   /* strerror */

int main(void) {
    errno = 0;
    /* fopen returns NULL on failure and sets errno to say why. */
    FILE *f = fopen("this-file-does-not-exist.txt", "r");
    if (f == NULL) {
        /* Two common ways to report the reason: */
        perror("fopen");                                   /* prints "fopen: <reason>" */
        printf("strerror(errno) = %s\n", strerror(errno)); /* the same reason, as a string */
        /* A real tool would stop here with `return EXIT_FAILURE;` so the shell
           sees a non-zero exit code. We reported it and carry on, so the lesson
           runs cleanly under `make run-c-app-...`. */
    } else {
        printf("opened the file\n");
        fclose(f);
    }

    printf("handled it — carrying on\n");
    return EXIT_SUCCESS;       /* 0 = success to the shell */
}
