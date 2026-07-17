/* 07-conditionals (C) — run different code depending on a test. */
#include <stdio.h>
#include <stdbool.h>  /* bool, true, false — a C99 addition */

int main(void) {
    int score = 72;

    /* if / else if / else: the first branch whose condition is nonzero runs;
       the rest are skipped. A condition is just an int expression — any
       nonzero value counts as true, zero as false. */
    if (score >= 90) {
        printf("Grade: A\n");
    } else if (score >= 70) {
        printf("Grade: B\n");
    } else {
        printf("Grade: needs work\n");
    }

    /* Before C99, C had no bool type; you used int (0 = false, else true).
       <stdbool.h> now provides bool/true/false as a thin layer over int. */
    bool passed = score >= 50;
    printf("passed = %d\n", passed);   /* prints 1, since bool is an int */

    /* Any nonzero value is "true": here score is nonzero, so this runs. */
    if (score) {
        printf("score is nonzero\n");
    }

    /* The ternary ?: is a compact if/else that produces a value. */
    const char *status = (score >= 50) ? "pass" : "fail";
    printf("Status: %s\n", status);
    return 0;
}
