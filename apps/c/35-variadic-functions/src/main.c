/* 35-variadic-functions (C) — accept a variable number of arguments with <stdarg.h>. */
#include <stdio.h>
#include <stdarg.h>   /* va_list, va_start, va_arg, va_end */

/* A fixed parameter (count) tells us how many extra arguments follow.
   There is no built-in way to know the count — the caller must tell us,
   exactly like printf's format string tells printf what to expect. */
static int sum_ints(int count, ...) {
    va_list ap;
    va_start(ap, count);        /* start after the last named parameter */
    int total = 0;
    for (int i = 0; i < count; ++i)
        total += va_arg(ap, int);   /* fetch the next argument, typed as int */
    va_end(ap);                 /* required cleanup */
    return total;
}

/* Default argument promotions apply to the "..." part: float becomes double,
   char/short become int. So read floating values back as double, not float. */
static double average(int count, ...) {
    va_list ap;
    va_start(ap, count);
    double total = 0.0;
    for (int i = 0; i < count; ++i)
        total += va_arg(ap, double);
    va_end(ap);
    return count ? total / count : 0.0;
}

int main(void) {
    printf("sum_ints(3, ...) = %d\n", sum_ints(3, 10, 20, 30));
    printf("average(2, ...)  = %.2f\n", average(2, 1.5, 2.5));
    return 0;
}
