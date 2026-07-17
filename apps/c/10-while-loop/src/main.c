/* 10-while-loop (C) — loop while a condition holds: count a number's digits. */
#include <stdio.h>

int main(void) {
    int n = 90210;
    int digits = 0;

    /* Peel one digit at a time by dividing by 10 until nothing is left.
       The condition is checked BEFORE each pass. */
    int rest = n;
    while (rest > 0) {
        rest /= 10;   /* integer division drops the last digit */
        digits++;
    }

    printf("%d has %d digits\n", n, digits);
    return 0;
}
