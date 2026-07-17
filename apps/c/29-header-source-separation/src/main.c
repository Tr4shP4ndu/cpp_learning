/* 29-header-source-separation (C) — declare in a header, define in a .c, link them. */
#include "greet.h"   /* brings in the declaration of greet() */

int main(void) {
    greet("world");  /* the linker resolves this to greet.c's definition */
    greet("C");
    return 0;
}
