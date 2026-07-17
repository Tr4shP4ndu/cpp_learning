/* 21-pointer-arithmetic (C) — pointer math scales by the pointed-to type. */
#include <stdio.h>

int main(void) {
    int arr[5] = {10, 20, 30, 40, 50};
    int n = 5;

    /* An array name decays to a pointer to its first element. */
    int *base = arr;                 /* same as &arr[0] */
    printf("arr[0] = %d, *base = %d\n", arr[0], *base);

    /* arr[i] IS *(arr + i); &arr[i] IS arr + i. */
    printf("arr[2] = %d, *(arr+2) = %d\n", arr[2], *(arr + 2));
    printf("&arr[2] - arr = %ld\n", (long)(&arr[2] - arr));  /* 2, not 8 */

    /* base + 1 advances by sizeof(int) bytes, not one byte. */
    printf("base=%p base+1=%p\n", (void *)base, (void *)(base + 1));

    /* Iterate with a moving pointer instead of an index. */
    int sum = 0;
    for (int *p = arr; p < arr + n; p++) {
        sum += *p;
    }
    printf("sum = %d\n", sum);
    return 0;
}
