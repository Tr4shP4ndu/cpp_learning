#include <iostream>
#include <typeinfo>  // Required for typeid()
#include <vector>
#include <algorithm>

void PassByValue(std::vector<int> arg) {
    arg[0] = 1;  // Modifies the copy of the vector
}

void PassByReference(std::vector<int>& alias) {
    alias[0] = 1;  // Modifies the original vector
}

int main() {
    int value = 42;           // Original variable
    int& ref = value;         // Reference to value
    // "int&" is the full type for a reference type

    ref = 43;  // Changing the value through the reference

    // Printing the value, type, and address of value
    std::cout << "value       : " << value << std::endl;                // 43
    std::cout << "type(value) : " << typeid(value).name() << std::endl; // int (implementation-specific name)
    std::cout << "&value      : " << &value << std::endl;               // Address of value

    // Printing the value, type, and address of ref
    std::cout << "ref         : " << ref << std::endl;                  // 43
    std::cout << "type(ref)   : " << typeid(ref).name() << std::endl;   // int
    std::cout << "&ref        : " << &ref << std::endl;                 // Same address as value

    // Demonstrate pass-by-value vs pass-by-reference safely
    std::vector<int> vec(5, 0); // small vector initialized to zeros
    std::cout << "vec[0] initially        : " << vec[0] << std::endl;   // 0

    PassByValue(vec);  // copies vec; modifies the copy only
    std::cout << "after PassByValue vec[0]: " << vec[0] << std::endl;   // still 0

    PassByReference(vec); // modifies the original
    std::cout << "after PassByRef  vec[0]: " << vec[0] << std::endl;    // now 1

    // Fill example without requiring <iterator>
    std::fill(vec.begin(), vec.end(), 2);
    std::cout << "after fill, vec[0]      : " << vec[0] << std::endl;   // 2

    return 0;
}
