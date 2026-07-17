/* 23-function-pointers (C) — variables that hold and call functions. */
#include <stdio.h>
#include <stdlib.h>

static int add(int x, int y) { return x + y; }
static int mul(int x, int y) { return x * y; }

/* qsort's comparator: cast the void* back to the real type, then compare. */
static int cmp_int(const void *a, const void *b) {
    int x = *(const int *)a;
    int y = *(const int *)b;
    return (x > y) - (x < y);   /* -1, 0, or 1 without overflow */
}

int main(void) {
    /* Declare a function pointer, assign a function, call through it. */
    int (*op)(int, int) = add;
    printf("op(3,4) = %d\n", op(3, 4));   /* 7 */
    op = mul;                              /* re-point at another function */
    printf("op(3,4) = %d\n", op(3, 4));   /* 12 */

    /* A real callback: qsort takes cmp_int as a function pointer. */
    int nums[] = {5, 2, 9, 1, 7};
    size_t n = sizeof nums / sizeof nums[0];
    qsort(nums, n, sizeof nums[0], cmp_int);

    printf("sorted:");
    for (size_t i = 0; i < n; i++) printf(" %d", nums[i]);
    printf("\n");
    return 0;
}
