/* 33-command-line-arguments (C) — argc/argv let a program read its invocation. */
#include <stdio.h>
#include <stdlib.h>   /* strtol */

int main(int argc, char *argv[]) {
    /* argc is the count; argv[0] is the program name, argv[1..argc-1] the args.
       argv[argc] is guaranteed to be NULL. */
    printf("argc = %d\n", argc);
    for (int i = 0; i < argc; ++i)
        printf("  argv[%d] = \"%s\"\n", i, argv[i]);

    /* Arguments always arrive as strings. To use one as a number, convert it. */
    if (argc > 1) {
        char *end;
        long v = strtol(argv[1], &end, 10);   /* base 10 */
        if (*end == '\0')
            printf("argv[1] as a number: %ld (doubled: %ld)\n", v, v * 2);
        else
            printf("argv[1] is not a whole number\n");
    }
    return 0;
}
