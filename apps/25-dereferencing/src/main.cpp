#include <iostream>

int main() {
    int num = 10;
    int* ptr = &num;  // Pointer to num

    std::cout << "Value of num: " << num << std::endl;              // Output: 10
    std::cout << "Address of num (&num): " << &num << std::endl;    // Prints the address of num
    std::cout << "Pointer value (ptr): " << ptr << std::endl;       // Prints the same address as &num
    std::cout << "Dereferenced pointer (*ptr): " << *ptr << std::endl;  // Output: 10

    // Modify the value of num using the pointer
    *ptr = 20;
    std::cout << "New value of num after dereferencing: " << num << std::endl;  // Output: 20

    return 0;
}
