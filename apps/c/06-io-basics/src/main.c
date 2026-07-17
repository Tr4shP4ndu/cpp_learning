/* 06-io-basics (C) — the same, using printf/scanf/fgets. */
#include <stdio.h>
#include <string.h>  /* strcspn — used to trim the trailing newline */

int main(void) {
    char name[64];
    int age = 0;

    printf("What is your name? ");
    /* fgets reads a whole line (up to 63 chars) safely into the buffer. */
    if (fgets(name, sizeof name, stdin) == NULL) return 1;
    /* fgets keeps the '\n'; overwrite it with a string terminator. */
    name[strcspn(name, "\n")] = '\0';

    printf("How old are you? ");
    /* scanf with %d reads one integer. */
    if (scanf("%d", &age) != 1) return 1;

    printf("Hello, %s! Next year you'll be %d.\n", name, age + 1);
    return 0;
}
