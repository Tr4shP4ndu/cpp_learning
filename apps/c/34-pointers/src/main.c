/* 34-pointers (C) — pointers are native C; the syntax is identical.
   C uses NULL (from <stddef.h>/<stdio.h>) where C++ prefers nullptr. */
#include <stdio.h>

int main(void) {
    int value = 42;
    int *ptr = &value;   /* &value = address; int* = pointer to int */

    printf("value = %d\n", value);
    printf("ptr   = %p\n", (void *)ptr);   /* %p prints a pointer */
    printf("*ptr  = %d\n", *ptr);          /* dereference to read through it */

    int *nothing = NULL;
    if (nothing == NULL) {
        printf("nothing is NULL\n");
    }

    int other = 7;
    ptr = &other;                          /* re-point */
    printf("*ptr  = %d\n", *ptr);          /* 7 */
    return 0;
}
