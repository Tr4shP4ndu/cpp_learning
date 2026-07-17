/* 09-for-loop (C) — a counted loop that sums 1..n. */
#include <stdio.h>

int main(void) {
    int n = 5;
    int sum = 0;

    /* init; condition; step — all in one header.
       Declaring the counter `i` in the init clause is C99+; C89 required
       declaring it before the loop. */
    for (int i = 1; i <= n; i++) {
        printf("adding %d\n", i);
        sum += i;
    }

    printf("sum of 1..%d = %d\n", n, sum);
    return 0;
}
