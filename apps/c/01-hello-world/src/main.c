/* 01-hello-world (C) — the same program, the C way. */
#include <stdio.h>  /* brings in printf, C's output function */

int main(void) {
    /* No streams and no std:: in C. printf writes formatted text to stdout. */
    printf("Hello, World!\n");
    return 0;
}
