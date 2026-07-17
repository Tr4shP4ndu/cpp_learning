/* 02-comments (C) — notes for humans that the compiler ignores. */
#include <stdio.h>

/* Doc-style comment: describe what a function does above its definition.
   double_it: returns twice its argument. */
int double_it(int n) {
    return n * 2;  /* end-of-line comment: // works here too in C99+ */
}

int main(void) {
    /* This is a block comment. It starts with slash-star and ends with
       star-slash, and it can span multiple lines. C89 had ONLY this form. */

    // This is a line comment. Everything after // is ignored.
    // Line comments are C99+; a strict C89 compiler would reject them.

    int speed = 60;  // comments can sit at the end of a line
    speed += 5;      /* good comments explain *why*, not the obvious *what* */

    printf("speed = %d, doubled = %d\n", speed, double_it(speed));
    return 0;
}
