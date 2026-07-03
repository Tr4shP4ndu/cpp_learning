// 34-pointers — a pointer is a variable that stores the ADDRESS of another.
#include <iostream>

int main() {
    int value = 42;

    // `&value` is the address-of operator: where value lives in memory.
    // `int*` is "pointer to int". ptr holds that address.
    int* ptr = &value;

    std::cout << "value      = " << value << "\n";
    std::cout << "&value     = " << &value << "\n";  // an address like 0x16d...
    std::cout << "ptr        = " << ptr << "\n";     // the same address
    std::cout << "*ptr       = " << *ptr << "\n";    // dereference: read through it (lesson 35)

    // A pointer can point at nothing. Use nullptr, and check before using.
    int* nothing = nullptr;
    if (nothing == nullptr) {
        std::cout << "nothing points to nullptr\n";
    }

    // A pointer can be re-pointed to a different object (references cannot).
    int other = 7;
    ptr = &other;
    std::cout << "*ptr now   = " << *ptr << "\n";     // 7

    return 0;
}
