// 35-testing — a hand-rolled test harness, no framework required.
#include <iostream>

// The function under test.
static int add(int a, int b) { return a + b; }

// A tiny harness: count checks, report failures with file/line, keep going.
static int checks = 0;
static int failures = 0;

#define CHECK(expr)                                                       \
    do {                                                                  \
        ++checks;                                                         \
        if (!(expr)) {                                                    \
            ++failures;                                                   \
            std::cout << "FAIL " << __FILE__ << ':' << __LINE__           \
                      << "  " << #expr << '\n';                           \
        }                                                                 \
    } while (0)

int main() {
    CHECK(add(2, 3) == 5);
    CHECK(add(-1, 1) == 0);
    CHECK(add(0, 0) == 0);

    std::cout << checks - failures << '/' << checks << " checks passed\n";
    // A test runner signals failure through its exit code, not just output.
    return failures == 0 ? 0 : 1;
}
