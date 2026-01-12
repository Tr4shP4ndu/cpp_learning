#include <iostream>

int main() {
    for (int i = 0; i < 10; ++i) {
        if (i % 2 == 0) {
            continue; // Skip the rest of the loop body for even numbers
        }
        std::cout << "Odd number: " << i << std::endl;
    }
    return 0;
}
