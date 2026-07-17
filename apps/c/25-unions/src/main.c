/* 25-unions (C) — a union overlays all members in one shared block of storage. */
#include <stdio.h>

/* All members share the same bytes: sizeof(union Number) == sizeof(double). */
union Number {
    int i;
    double d;
};

/* A tagged union: the enum records which member is currently valid. */
enum Kind { KIND_INT, KIND_DOUBLE };

struct Value {
    enum Kind kind;
    union Number data;
};

static void print_value(struct Value v) {
    if (v.kind == KIND_INT) {
        printf("int    %d\n", v.data.i);
    } else {
        printf("double %.2f\n", v.data.d);
    }
}

int main(void) {
    union Number n;
    n.i = 42;                    /* write one member... */
    printf("as int: %d\n", n.i); /* ...and read the same one back */
    n.d = 3.14;                  /* writing d now clobbers i */

    printf("sizeof(union Number) = %zu\n", sizeof(union Number));

    struct Value a = {KIND_INT, {.i = 7}};
    struct Value b = {KIND_DOUBLE, {.d = 2.5}};
    print_value(a);
    print_value(b);
    return 0;
}
