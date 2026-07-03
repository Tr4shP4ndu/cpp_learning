#include <iostream>

// Function definition
void foo() {
    int x = 72; // Local variable inside the function
}

int main() {
    int x = 42;               // Integer variable
    float y = 72.0;           // Float variable
    char a = 'a';             // Character variable
    signed char b = 'b';      // Signed character variable
    unsigned char c = 'c';    // Unsigned character variable

    // Printing the addresses of the variables
    std::cout << "Address of x: " << &(x) << std::endl;            // Example output: Address of x: 0x7ffd8d4e1234
    std::cout << "Size of x: " << sizeof(x) << " bytes" << std::endl; // Example output: Size of x: 4 bytes
    std::cout << "Address of y: " << &(y) << std::endl;            // Example output: Address of y: 0x7ffd8d4e1238
    std::cout << "Address of a: " << (void*)&(a) << std::endl;     // Example output: Address of a: 0x7ffd8d4e123c
    std::cout << "Address of b: " << (void*)&(b) << std::endl;     // Example output: Address of b: 0x7ffd8d4e123d
    std::cout << "Address of c: " << (void*)&(c) << std::endl;     // Example output: Address of c: 0x7ffd8d4e123e

    // Printing the addresses of the functions
    std::cout << "Address of foo function: " << (void*)&foo << std::endl;   // Example output: Address of foo function: 0x55e2b7f10270
    std::cout << "Address of main function: " << (void*)&main << std::endl; // Example output: Address of main function: 0x55e2b7f10290

    return 0;
}
