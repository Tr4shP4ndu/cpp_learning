// 40-dereferencing — the * operator follows a pointer to the value it points at.
#include <iostream>

int main() {
    int num = 10;
    int* ptr = &num;   // ptr holds the address of num

    // Reading through the pointer:
    std::cout << "num   = " << num  << "\n";   // 10
    std::cout << "ptr   = " << ptr  << "\n";   // an address
    std::cout << "*ptr  = " << *ptr << "\n";   // 10 — dereference: value at that address

    // Writing through the pointer changes the pointed-to object:
    *ptr = 20;
    std::cout << "after *ptr = 20, num = " << num << "\n";   // 20 — num changed

    return 0;
}
