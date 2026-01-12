#include <iostream>

int multiply(int x, int y) {
    return x * y;
}

int main() {
    int product = multiply(4, 5);
    std::cout << "The product is: " << product << std::endl;
    return 0;
}
