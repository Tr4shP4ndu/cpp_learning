/* 17-arrays (C) — the raw array is native C; C++ inherited it unchanged. */
#include <stdio.h>

int main(void) {
    int scores[5] = {10, 20, 30, 40, 50};

    for (int i = 0; i < 5; ++i) {
        printf("scores[%d] = %d\n", i, scores[i]);
    }

    /* Same sizeof trick in C. Note this ONLY works where the real array is
       visible; once passed to a function it "decays" to a pointer and sizeof
       would give the pointer's size instead (a classic C gotcha). */
    size_t count = sizeof(scores) / sizeof(scores[0]);
    printf("element count = %zu\n", count);
    return 0;
}
