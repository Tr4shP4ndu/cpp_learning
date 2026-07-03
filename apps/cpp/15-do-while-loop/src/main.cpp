// 15-do-while-loop — like while, but the body always runs at least once.
#include <iostream>

int main() {
    int i = 0;
    // The condition is tested AFTER the body, so the body runs before the first
    // check. Useful for menus / prompts that must show at least once.
    do {
        std::cout << "iteration " << i << "\n";
        ++i;
    } while (i < 5);  // note the semicolon — required here

    // Even with a condition false from the start, the body runs once:
    int n = 100;
    do {
        std::cout << "runs once even though 100 < 5 is false\n";
    } while (n < 5);

    return 0;
}
