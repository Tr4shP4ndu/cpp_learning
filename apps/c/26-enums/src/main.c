/* 26-enums (C) — an enum names integer constants, auto-incrementing from 0. */
#include <stdio.h>

/* MON is 0, TUE is 1, ...; you can also set explicit values. */
enum Day { MON, TUE, WED, THU, FRI, SAT, SUN };

enum Http { OK = 200, NOT_FOUND = 404, TEAPOT = 418 };

int main(void) {
    enum Day today = WED;
    printf("WED = %d\n", today);        /* underlying type is int */

    /* An enum value is just an int, so it drives a switch cleanly. */
    switch (today) {
        case SAT:
        case SUN:
            printf("weekend\n");
            break;
        default:
            printf("weekday\n");
            break;
    }

    enum Http status = NOT_FOUND;
    printf("status = %d\n", status);
    return 0;
}
