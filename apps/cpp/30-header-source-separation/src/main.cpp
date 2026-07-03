// main.cpp — a CONSUMER: uses greet() knowing only its header.
#include <iostream>
#include "greeter.hpp"  // brings in the declaration; not greeter.cpp itself

int main() {
    // We can call greet() because the header declared it. The linker later
    // connects this call to the definition compiled from greeter.cpp.
    std::cout << greet("Ada") << "\n";
    std::cout << greet("Grace") << "\n";
    return 0;
}
