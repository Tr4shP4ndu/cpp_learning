/* 31-const-and-volatile (C) — read-only data, and values that change behind the compiler's back. */
#include <stdio.h>
#include <signal.h>   /* sig_atomic_t (used here as a plain type, no handler installed) */

/* const in a parameter documents "I will not modify what this points to". */
static int sum(const int *a, size_t n) {
    int total = 0;
    for (size_t i = 0; i < n; ++i)
        total += a[i];   /* reading through a const pointer is fine */
    return total;
}

/* volatile tells the compiler this may change outside normal control flow
   (a hardware register, or a flag set by a signal handler), so every read
   must actually re-load it — it must not be cached or optimised away. */
static volatile sig_atomic_t flag = 0;

int main(void) {
    const double PI = 3.14159;   /* read-only: assigning to PI is a compile error */
    printf("PI = %.5f\n", PI);

    const int nums[] = {1, 2, 3, 4};
    size_t n = sizeof nums / sizeof nums[0];
    printf("sum = %d over %zu values\n", sum(nums, n), n);

    flag = 1;   /* imagine a signal handler set this; the loop must re-read it */
    while (flag) {
        printf("flag observed as set; clearing\n");
        flag = 0;   /* without volatile the compiler could assume flag never changes */
    }
    printf("done\n");
    return 0;
}
