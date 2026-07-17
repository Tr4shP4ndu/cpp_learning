/* 08-switch (C) — pick one branch out of many based on a single value. */
#include <stdio.h>

int main(void) {
    int day = 6;  /* 1 = Monday ... 7 = Sunday */

    /* switch tests an integer (int, char, enum) against constant case labels.
       You cannot switch on a string in C — no case "monday". */
    switch (day) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            /* cases 1-5 share this body via intentional fall-through. */
            printf("Weekday\n");
            break;      /* break stops the switch; without it, control would
                           fall through into the next case. */
        case 6:
            printf("Saturday\n");
            /* no break here: deliberately fall through into case 7 */
        case 7:
            printf("Weekend\n");
            break;
        default:
            printf("Not a valid day\n");
            break;
    }

    /* switch also works on a char, since a char is an integer type. */
    char grade = 'B';
    switch (grade) {
        case 'A': printf("Excellent\n"); break;
        case 'B': printf("Good\n");      break;
        default:  printf("See me\n");    break;
    }
    return 0;
}
