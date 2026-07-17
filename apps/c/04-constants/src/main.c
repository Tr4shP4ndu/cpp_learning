/* 04-constants (C) — three ways to name a constant, and when to use each. */
#include <stdio.h>

/* 1. #define: a preprocessor text substitution. No type, no storage; MAX is
      literally replaced by 100 before compilation. Great for array sizes. */
#define MAX 100

/* 3. enum constant: a named int, evaluated at compile time. Ideal for a set
      of related integer constants and for array sizes. */
enum { LIMIT = 50 };

int main(void) {
    /* 2. const object: a typed, read-only variable. Best when you want a real
          type and scope. PI = 3.0; would be a compile error. */
    const double PI = 3.14159;

    int buffer[MAX];        /* #define constant as array size — fine */
    int slots[LIMIT];       /* enum constant as array size — fine */

    /* In C a `const int` is NOT a constant expression, so it may not size an
       array: `const int n = 8; int a[n];` is a variable-length array, not a
       fixed one (and is rejected where a constant is required). */
    const int n = 8;
    printf("MAX=%d LIMIT=%d PI=%.2f n=%d\n", MAX, LIMIT, PI, n);
    printf("sizeof buffer=%zu, sizeof slots=%zu\n",
           sizeof buffer, sizeof slots);
    return 0;
}
