/* 15-recursion (C) — a function that calls itself, with a base case. */
#include <stdio.h>

/* factorial(n) = n * factorial(n - 1), stopping at the base case n <= 1.
   Each call adds a stack frame; the base case is what lets them unwind. */
unsigned long factorial(unsigned int n) {
    if (n <= 1) return 1;            /* base case — stops the recursion */
    return n * factorial(n - 1);     /* recursive case — smaller subproblem */
}

int main(void) {
    for (unsigned int i = 0; i <= 5; ++i) {
        printf("%u! = %lu\n", i, factorial(i));
    }
    return 0;
}
