/* 37-type-conversions (C) — implicit promotions, the signed/unsigned trap, and casts. */
#include <stdio.h>

int main(void) {
    /* Integer promotion: char/short are promoted to int in arithmetic. */
    char c = 100;
    printf("c + c = %d (chars promoted to int before adding)\n", c + c);

    /* The signed/unsigned trap: mixing them converts the signed value to
       unsigned, so -1 becomes a huge number and the comparison is false.
       Writing `a < b` does exactly this implicitly; here we spell out the
       (unsigned) cast the compiler inserts (a bare `a < b` warns under
       -Wsign-compare precisely to flag this pitfall). */
    int a = -1;
    unsigned b = 1u;
    printf("(-1 < 1u) is %s  <- surprising!\n", ((unsigned)a < b) ? "true" : "false");
    printf("-1 as unsigned = %u\n", (unsigned)a);

    /* float -> int truncates toward zero (it does not round). */
    double d = 3.9;
    int t = (int)d;
    printf("(int)3.9 = %d (truncated, not rounded)\n", t);

    /* Integer division vs. an explicit cast to get real division. */
    int x = 7, y = 2;
    printf("7 / 2      = %d (integer division)\n", x / y);
    printf("(double)7/2 = %.1f (cast one operand first)\n", (double)x / y);
    return 0;
}
