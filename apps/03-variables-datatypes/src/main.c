/* 03-variables-datatypes (C) — C's built-in types and printf format specifiers.
   C has no std::string and no std::cout; text is char arrays and printf. */
#include <stdio.h>
#include <stdint.h>  /* fixed-width integer types: int32_t, uint64_t, ... */

int main(void) {
    int   count = -10;      /* signed integer */
    unsigned int ucount = 10;
    long  big = -30000L;
    float f = 10.5f;
    double d = 20.99;
    char  letter = 'A';     /* a single byte */

    /* printf needs a format specifier per value:
       %d int, %u unsigned, %ld long, %f floating, %c char, %zu size_t */
    printf("int   count  = %d  (%zu bytes)\n", count,  sizeof count);
    printf("uint  ucount = %u  (%zu bytes)\n", ucount, sizeof ucount);
    printf("long  big    = %ld (%zu bytes)\n", big,    sizeof big);
    printf("float f      = %f  (%zu bytes)\n", (double)f, sizeof f);
    printf("double d     = %f  (%zu bytes)\n", d,      sizeof d);
    printf("char  letter = %c  (%zu bytes)\n", letter, sizeof letter);

    /* A C "string" is just an array of char ending in a '\0' byte. */
    char name[] = "Tr4shP4ndu";
    printf("name         = %s  (%zu bytes incl. terminator)\n", name, sizeof name);
    return 0;
}
