/* 36-standard-library-tour (C) — a runnable sampler of the C standard library. */
#include <stdio.h>
#include <stdlib.h>    /* qsort, bsearch, srand, rand, strtol, abs */
#include <assert.h>    /* assert */
#include <limits.h>    /* INT_MAX */
#include <stdint.h>    /* int32_t, SIZE_MAX */
#include <inttypes.h>  /* PRId32 */

/* Comparator for qsort/bsearch: return <0, 0, >0. */
static int cmp_int(const void *a, const void *b) {
    int x = *(const int *)a, y = *(const int *)b;
    return (x > y) - (x < y);   /* branch-free, no overflow */
}

int main(void) {
    /* <stdlib.h>: sort, then binary-search the sorted array. */
    int v[] = {5, 2, 9, 1, 7};
    size_t n = sizeof v / sizeof v[0];
    qsort(v, n, sizeof v[0], cmp_int);
    int key = 7;
    int *hit = bsearch(&key, v, n, sizeof v[0], cmp_int);
    printf("sorted[0]=%d, found 7 at index %td\n", v[0], hit ? hit - v : -1);

    /* Deterministic pseudo-random: fixed seed -> same output every run. */
    srand(1);
    printf("rand()%%100 = %d\n", rand() % 100);

    /* strtol parses a string to a long; abs gives magnitude. */
    long parsed = strtol("-42", NULL, 10);
    printf("strtol(\"-42\") = %ld, abs = %d\n", parsed, abs((int)parsed));

    /* <assert.h>: a passing assertion (aborts only if the condition is false). */
    assert(v[0] <= v[n - 1]);

    /* <limits.h>/<stdint.h>: sizes and bounds. */
    int32_t small = 100;
    printf("INT_MAX=%d, int32_t=%" PRId32 ", SIZE_MAX=%zu\n", INT_MAX, small, SIZE_MAX);
    return 0;
}
