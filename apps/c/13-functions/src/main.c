/* 13-functions (C) — declare a prototype above main, define it below. */
#include <stdio.h>

/* Prototypes: the return type and parameter types, ending in ';'. These tell
   the compiler how to call each function before it sees the definition. */
int add(int a, int b);
void greet(void);        /* void params = "takes nothing"; void return = "no result" */

int main(void) {
    greet();
    int sum = add(3, 4);
    printf("3 + 4 = %d\n", sum);
    return 0;
}

/* Definitions live below main. Because the prototypes were visible above, the
   compiler already knows the exact signatures. */
int add(int a, int b) {
    return a + b;
}

void greet(void) {
    printf("hello from a function\n");
}
