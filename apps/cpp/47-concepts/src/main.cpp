// 47-concepts — constrain templates so requirements are explicit (C++20).
#include <iostream>
#include <concepts>   // std::integral, std::floating_point, ...

// A concept names a requirement on a type. This one is satisfied by any
// integer or floating-point type.
template <typename T>
concept Numeric = std::integral<T> || std::floating_point<T>;

// Constrain the template with the concept. Now `sum` accepts ONLY numeric types,
// and calling it with (say) a std::string gives a short, clear error instead of
// a wall of template diagnostics.
template <Numeric T>
T sum(T a, T b) {
    return a + b;
}

// `requires` can also be written inline for ad-hoc constraints.
template <typename T>
    requires std::floating_point<T>
T half(T x) {
    return x / T{2};
}

int main() {
    std::cout << "sum(3, 4)       = " << sum(3, 4) << "\n";
    std::cout << "sum(1.5, 2.25)  = " << sum(1.5, 2.25) << "\n";
    std::cout << "half(9.0)       = " << half(9.0) << "\n";

    // sum(std::string{"a"}, std::string{"b"});  // error: constraint not satisfied
    return 0;
}
