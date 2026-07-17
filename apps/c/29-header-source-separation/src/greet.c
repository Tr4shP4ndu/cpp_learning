#include <stdio.h>
#include "greet.h"

/* Definition: the actual body, compiled as its own translation unit. */
void greet(const char *name) {
    printf("Hello, %s!\n", name);
}
