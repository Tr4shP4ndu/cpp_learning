// 53-constexpr — compute at compile time, not run time.
#include <array>
#include <iostream>

// A constexpr function CAN run at compile time when its inputs are known
// constants, and still works at run time otherwise.
constexpr int factorial(int n) {
    int result = 1;
    for (int i = 2; i <= n; ++i) result *= i;
    return result;
}

// consteval (C++20): MUST run at compile time — calling it with a value not
// known until run time is a compile error.
consteval int square(int x) { return x * x; }

int main() {
    // Evaluated by the compiler; usable anywhere a constant is required.
    constexpr int f5 = factorial(5);
    static_assert(f5 == 120, "factorial(5) computed at compile time");

    // A compile-time constant can size an array; a run-time int cannot.
    std::array<int, factorial(4)> buffer{};   // size 24
    std::cout << "buffer size = " << buffer.size() << '\n';

    std::cout << "square(9) = " << square(9) << '\n';   // 81, at compile time

    // The same constexpr function still works at run time with a run-time arg.
    int n = 6;
    std::cout << "factorial(6) at run time = " << factorial(n) << '\n';
    return 0;
}
