#include <iostream>

// Function that modifies the value of its parameter
void PassByValue(int number) {
    number = 100;  // This change affects only the local copy of the variable
    std::cout << "Inside function, number = " << number << std::endl;  // Output: 100
}

int main() {
    int num = 42;
    std::cout << "Before function call, num = " << &num << std::endl;
    std::cout << "Before function call, num = " << num << std::endl;   // Output: 42

    // Call the function with pass by value
    PassByValue(num);

    std::cout << "After function call, num = " << &num << std::endl;
    std::cout << "After function call, num = " << num << std::endl;    // Output: 42
    return 0;
}
