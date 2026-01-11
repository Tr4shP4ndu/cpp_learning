#include <iostream>
#include <numeric>
#include <iterator>

int main() {
    int rawArray[5] = {10, 20, 30, 40, 50};// Declare and initialize a raw array

    // Accessing elements
    for (int i = 0; i < 5; ++i) {
        std::cout << "Element at index " << i << ": " << rawArray[i] << std::endl;
    }

    // Unsafe operation example (out of bounds)
    // std::cout << rawArray[5]; // Uncommenting this line would lead to undefined behavior

    int ids[100];
    std::iota(std::begin(ids), std::end(ids), 0);
    for(int i=0; i < 100; i++){
      std::cout << ids[i] << std::endl;
    }

    return 0;
}
