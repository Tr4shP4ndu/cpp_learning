// 39-pointers — a pointer stores the ADDRESS of another object; * follows it.
#include <iostream>

int main() {
    int value = 42;

    // `&value` is the address-of operator: where value lives in memory.
    // `int*` is "pointer to int". ptr holds that address.
    int* ptr = &value;

    std::cout << "value  = " << value  << "\n";
    std::cout << "&value = " << &value << "\n";  // an address like 0x16d...
    std::cout << "ptr    = " << ptr    << "\n";  // the same address
    std::cout << "*ptr   = " << *ptr   << "\n";  // dereference: read the value there

    // Dereferencing also WRITES through the pointer: because ptr points at
    // value, changing *ptr changes value itself.
    *ptr = 20;
    std::cout << "after *ptr = 20, value = " << value << "\n";  // 20

    // A pointer can point at nothing. Use nullptr, and check before using.
    int* nothing = nullptr;
    if (nothing == nullptr) std::cout << "nothing points to nullptr\n";

    // A pointer can be re-pointed to a different object (references cannot).
    int other = 7;
    ptr = &other;
    std::cout << "*ptr now = " << *ptr << "\n";  // 7
    return 0;
}
