// 13-range-for-loop — visit every element of a container without an index.
#include <iostream>
#include <array>

int main() {
    std::array<int, 3> values{1, 3, 5};

    // "for (element : container)" reads each item in turn. No counter, no
    // bounds — so no off-by-one bugs. Use `const auto&` to avoid copying.
    for (const auto& element : values) {
        std::cout << element << "\n";
    }

    // Use a non-const reference when you want to modify in place.
    for (auto& element : values) {
        element *= 10;
    }
    std::cout << "after *= 10: " << values[0] << ", " << values[1]
              << ", " << values[2] << "\n";
    return 0;
}
