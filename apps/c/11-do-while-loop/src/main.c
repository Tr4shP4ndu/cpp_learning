/* 11-do-while-loop (C) — the body runs at least once, condition checked after. */
#include <stdio.h>

int main(void) {
    /* Classic retry/menu shape, driven by a script instead of real input so it
       stays non-interactive. Attempt 3 is the "correct" one. */
    int attempt = 0;
    int correct = 3;

    do {
        attempt++;
        printf("attempt %d...\n", attempt);
    } while (attempt != correct);

    printf("succeeded on attempt %d\n", attempt);
    return 0;
}
