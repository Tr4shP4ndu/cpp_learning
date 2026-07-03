// 51-error-handling — three ways to report failure: optional, expected, throw.
#include <iostream>
#include <optional>    // std::optional (C++17)
#include <expected>    // std::expected (C++23)
#include <string>
#include <stdexcept>

// 1) optional<T>: "a value, or nothing." Use when absence isn't really an error.
std::optional<int> firstDigit(const std::string& s) {
    for (char c : s)
        if (c >= '0' && c <= '9') return c - '0';
    return std::nullopt;   // found nothing
}

// 2) expected<T, E>: "a value, OR an error explaining why not." Best for
//    recoverable failures where the caller wants the reason.
std::expected<double, std::string> safeDivide(double a, double b) {
    if (b == 0.0) return std::unexpected("division by zero");
    return a / b;
}

// 3) exceptions: throw for truly exceptional cases; unwinds until caught.
int parsePositive(int n) {
    if (n <= 0) throw std::invalid_argument("must be positive");
    return n;
}

int main() {
    // optional
    if (auto d = firstDigit("abc7xyz")) std::cout << "first digit: " << *d << "\n";
    if (!firstDigit("abc"))             std::cout << "no digit found\n";

    // expected
    auto r = safeDivide(10, 2);
    if (r) std::cout << "10/2 = " << *r << "\n";
    auto bad = safeDivide(1, 0);
    if (!bad) std::cout << "error: " << bad.error() << "\n";

    // exceptions
    try {
        parsePositive(-5);
    } catch (const std::exception& e) {
        std::cout << "caught: " << e.what() << "\n";
    }
    return 0;
}
