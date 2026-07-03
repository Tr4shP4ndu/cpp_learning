// 08-switch — pick one branch out of many based on a single value.
#include <iostream>

int main() {
    int day = 3;  // 1 = Monday ... 7 = Sunday

    switch (day) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            // cases 1-5 share this body because they "fall through" to here.
            std::cout << "Weekday\n";
            break;  // break stops the switch; without it, execution continues
                    // into the next case.
        case 6:
            std::cout << "Saturday\n";
            [[fallthrough]];  // deliberate fall-through — silences the warning
        case 7:
            std::cout << "Weekend\n";
            break;
        default:
            std::cout << "Not a valid day\n";
            break;
    }

    return 0;
}
