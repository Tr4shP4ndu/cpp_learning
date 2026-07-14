/* 41-dynamic-memory (C) — malloc/free are C's equivalent of new/delete. */
#include <stdio.h>
#include <stdlib.h>   /* malloc, free */

int main(void) {
    /* malloc takes a size in BYTES and returns void* (or NULL on failure).
       Always check the result and size it with sizeof. */
    int *p = malloc(sizeof *p);
    if (!p) return 1;
    *p = 42;
    printf("*p = %d\n", *p);
    free(p);            /* release it. Forgetting free() is a leak. */
    p = NULL;

    /* An array: malloc a block of n ints. There's no separate array free —
       one free() releases the whole block. */
    int n = 5;
    int *arr = malloc((size_t)n * sizeof *arr);
    if (!arr) return 1;
    for (int i = 0; i < n; ++i) arr[i] = i * i;
    printf("arr[4] = %d\n", arr[4]);
    free(arr);
    arr = NULL;

    printf("all heap memory released\n");
    return 0;
}
