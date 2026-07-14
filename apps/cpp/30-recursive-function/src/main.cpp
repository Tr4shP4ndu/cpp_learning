// 30-recursive-function — a function that calls itself to solve smaller cases.
#include <iostream>
#include <cassert>

// factorial(n) = n * (n-1) * ... * 1
long long factorial(int n) {
    // BASE CASE: the smallest input that stops the recursion.
    if (n <= 1) {
        return 1;
    }
    // RECURSIVE CASE: solve a smaller problem, then combine.
    // factorial(5) -> 5 * factorial(4) -> 5 * 4 * factorial(3) -> ...
    return n * factorial(n - 1);
}

int main() {
    // Self-check: verify a couple of known values.
    assert(factorial(0) == 1);
    assert(factorial(5) == 120);

    for (int n = 0; n <= 6; ++n) {
        std::cout << n << "! = " << factorial(n) << "\n";
    }
    return 0;
}
