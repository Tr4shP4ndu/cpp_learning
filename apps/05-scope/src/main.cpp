#include <iostream>

int main() {
    int x = 10; // x is in the main block scope

    std::cout << "x in main: " << x << std::endl;

    {
        int y = 20; // y is in a new block scope
        std::cout << "y in block: " << y << std::endl;
        std::cout << "x in block: " << x << std::endl; // x is accessible here
    }

    // std::cout << "y in main: " << y << std::endl; // This would cause an error: 'y' was not declared in this scope

    return 0;
}
