// 12-for-loop — repeat code a known number of times with a counter.
#include <iostream>
#include <array>

int main() {
    std::array<int, 3> values{1, 3, 5};

    // for (init; condition; step)
    //   init      runs once at the start   -> std::size_t i = 0
    //   condition checked before each pass  -> i < values.size()
    //   step      runs after each pass      -> ++i
    // Use std::size_t (unsigned) to match what .size() returns.
    for (std::size_t i = 0; i < values.size(); ++i) {
        std::cout << "values[" << i << "] = " << values[i] << "\n";
    }
    return 0;
}
