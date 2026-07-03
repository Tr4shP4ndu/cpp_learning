// 17-continue-statement — skip the rest of THIS pass, go to the next one.
#include <iostream>

int main() {
    // Print only the odd numbers by skipping the even ones.
    for (int i = 0; i < 10; ++i) {
        if (i % 2 == 0) {
            continue;  // even -> jump to the loop's step (++i) and next test
        }
        std::cout << "odd: " << i << "\n";
    }
    return 0;
}
