// 07-conditionals — run different code depending on whether a test is true.
#include <iostream>

int main() {
    int score = 72;

    // if / else if / else: the first true branch runs; the rest are skipped.
    if (score >= 90) {
        std::cout << "Grade: A\n";
    } else if (score >= 70) {
        std::cout << "Grade: B\n";
    } else {
        std::cout << "Grade: needs work\n";
    }

    // The ternary operator ?: is a compact if/else that produces a value.
    const char* status = (score >= 50) ? "pass" : "fail";
    std::cout << "Status: " << status << "\n";

    // C++17: an if can declare a variable scoped to the if/else.
    if (int bonus = score % 10; bonus > 5) {
        std::cout << "Bonus points: " << bonus << "\n";
    }

    return 0;
}
