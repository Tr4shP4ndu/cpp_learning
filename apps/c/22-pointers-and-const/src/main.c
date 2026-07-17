/* 22-pointers-and-const (C) — where const sits changes what is frozen. */
#include <stdio.h>

/* Out-parameter via pointer-to-pointer: make *out point at the array. */
static void pick(int **out, int *arr) {
    *out = arr;
}

int main(void) {
    int a = 1, b = 2;

    /* pointer to const: can't change the value THROUGH p; can re-point p. */
    const int *p = &a;
    /* *p = 99;  <- would not compile */
    p = &b;
    printf("pointer-to-const *p = %d\n", *p);

    /* const pointer: can't re-point q; can change the value through it. */
    int *const q = &a;
    *q = 42;
    /* q = &b;  <- would not compile */
    printf("const pointer *q = %d\n", *q);

    /* pointer-to-pointer as an out-parameter. */
    int nums[3] = {7, 8, 9};
    int *chosen = NULL;
    pick(&chosen, nums);
    printf("chosen[1] = %d\n", chosen[1]);

    /* char ** is exactly the type of argv: an array of strings. */
    const char *words[] = {"one", "two", "three"};
    const char **w = words;
    printf("words: %s %s %s\n", w[0], w[1], w[2]);
    return 0;
}
