#include <iostream>
#include <array>
#include <algorithm> // for std::fill

int main() {
    std::array<int, 5> numbers; // Uninitialized array

    // Fill array with the value 42
    std::fill(numbers.begin(), numbers.end(), 42);

    for (int num : numbers) {
        std::cout << "Array element: " << num << std::endl;
    }

    return 0;
}
