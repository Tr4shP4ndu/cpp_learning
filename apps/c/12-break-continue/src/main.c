/* 12-break-continue (C) — break exits a loop; continue skips to the next pass. */
#include <stdio.h>

int main(void) {
    int sum = 0;

    for (int i = 1; i <= 20; i++) {
        if (i % 2 != 0) {
            continue;      /* skip odd numbers — jump to i++ */
        }
        if (i > 10) {
            break;         /* stop the loop entirely once past 10 */
        }
        sum += i;          /* only even i in 2..10 reach here */
    }

    printf("sum of even numbers 2..10 = %d\n", sum);  /* 2+4+6+8+10 = 30 */
    return 0;
}
