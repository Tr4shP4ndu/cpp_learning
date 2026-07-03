// 20-std-fill — set every element of a range to one value in a single call.
#include <iostream>
#include <array>
#include <algorithm>  // std::fill

int main() {
    std::array<int, 5> numbers{};  // value-initialized to zeros

    // std::fill(first, last, value) assigns `value` to each element in
    // [first, last). Clearer and less error-prone than a hand-written loop.
    std::fill(numbers.begin(), numbers.end(), 42);

    for (int n : numbers) {
        std::cout << n << "\n";
    }
    return 0;
}
