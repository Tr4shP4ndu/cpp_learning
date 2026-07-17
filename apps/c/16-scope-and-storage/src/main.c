/* 16-scope-and-storage (C) — block vs file scope, static, extern. */
#include <stdio.h>

/* File scope with internal linkage: private to this translation unit. */
static int file_private = 100;

/* A definition another file could reach via `extern int shared;`. */
int shared = 7;

/* `extern` here just DECLARES that `shared` exists (defined above/elsewhere). */
extern int shared;

int next_id(void) {
    /* A static local keeps its value across calls — initialized once. */
    static int counter = 0;
    counter += 1;                 /* persists; not reset each call */
    return counter;
}

int main(void) {
    /* Automatic storage (the default for locals): lives only in this block. */
    int local = 1;
    {
        int inner = local + 1;    /* block scope: visible only inside {} */
        printf("inner = %d\n", inner);
    }
    /* `inner` is out of scope here. */

    printf("ids: %d %d %d\n", next_id(), next_id(), next_id());  /* 1 2 3 */
    printf("file_private = %d, shared = %d\n", file_private, shared);
    return 0;
}
