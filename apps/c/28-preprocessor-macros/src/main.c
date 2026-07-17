/* 28-preprocessor-macros (C) — textual substitution done before compilation. */
#include <stdio.h>

/* Object-like macro: a plain textual replacement. */
#define PI 3.14159

/* Function-like macro: PARENTHESISE each argument AND the whole body, or
   precedence/adjacency bugs creep in (e.g. SQUARE(1+2) or 1/SQUARE(2)). */
#define SQUARE(x) ((x) * (x))

/* Toggle a build feature; try `-DVERBOSE` on the command line. */
#define VERBOSE 0

int main(void) {
    printf("PI = %.5f\n", PI);
    printf("SQUARE(1 + 2) = %d\n", SQUARE(1 + 2)); /* 9, not 5, thanks to parens */

#if VERBOSE
    printf("verbose build\n");
#else
    printf("quiet build\n");
#endif

#ifdef PI
    printf("PI is defined\n");
#endif

#undef PI   /* PI is no longer usable below this line */
    return 0;
}
