/* 14-function-parameters (C) — pass-by-value vs pointers vs array decay. */
#include <stdio.h>

/* Pass-by-value: n is a COPY, so this cannot change the caller's variable. */
void try_increment(int n) { n = n + 1; }

/* To modify the caller's variable, take its address and write through it. */
void increment(int *n) { *n = *n + 1; }

/* An array parameter is really a pointer (the array "decays"), so this writes
   into the caller's original storage. */
void fill(int a[], int n) {
    for (int i = 0; i < n; ++i) a[i] = i * 10;
}

int main(void) {
    int x = 5;
    try_increment(x);
    printf("after try_increment: %d\n", x);   /* still 5 — copy was changed */

    increment(&x);
    printf("after increment: %d\n", x);        /* 6 — modified through pointer */

    int nums[3] = {0, 0, 0};
    fill(nums, 3);
    printf("nums = %d %d %d\n", nums[0], nums[1], nums[2]);  /* 0 10 20 */
    return 0;
}
