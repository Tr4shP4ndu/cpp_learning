/* 05-operators (C) — the symbols that compute with values. */
#include <stdio.h>

int main(void) {
    int a = 7, b = 2;

    /* Arithmetic. int / int truncates toward zero; % is remainder. */
    printf("a+b=%d a-b=%d a*b=%d a/b=%d a%%b=%d\n",
           a + b, a - b, a * b, a / b, a % b);

    /* Integer vs floating division: 7/2 is 3, but 7.0/2 is 3.5. */
    printf("7/2 = %d   7.0/2 = %.1f\n", 7 / 2, 7.0 / 2);

    /* Relational operators yield int 1 (true) or 0 (false). */
    printf("a>b=%d a==b=%d\n", a > b, a == b);

    /* Logical && || ! short-circuit: the right side is skipped when the
       result is already decided. */
    printf("logical: %d\n", (a > 0 && b > 0) || !(a == b));

    /* Bitwise operators act on individual bits. */
    unsigned f = 0x5u;          /* 0101 */
    f |= 0x2u;                  /* set  -> 0111 */
    f &= ~0x1u;                 /* clear-> 0110 */
    printf("bits: %u  xor=%u  <<1=%u  >>1=%u\n", f, 6u ^ 3u, 1u << 3, 16u >> 2);

    /* Compound assignment and increment/decrement (kept on separate
       statements — modifying a variable more than once between sequence
       points is undefined behavior). */
    int x = 5;
    x += 3; x *= 2;             /* x = (5+3)*2 = 16 */
    printf("x=%d ", x);
    ++x;                        /* prefix: increment, then value is 17 */
    printf("after ++x: %d ", x);
    x--;                        /* postfix here as a statement: back to 16 */
    printf("after x--: %d\n", x);

    /* Ternary ?: is a compact expression that produces a value. */
    printf("max = %d\n", a > b ? a : b);
    return 0;
}
