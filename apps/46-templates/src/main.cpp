#include <iostream>
#include <string>
#include "generic.hpp"

int main() {
    // The same maximum() works for any type with operator>.
    std::cout << "maximum(3, 4)       = " << maximum(3, 4) << "\n";
    std::cout << "maximum(1.5, 0.5)   = " << maximum(1.5, 0.5) << "\n";
    std::cout << "maximum(\"abc\",\"abd\") = "
              << maximum(std::string("abc"), std::string("abd")) << "\n";

    // Class template: two different concrete types from one blueprint.
    Box<int> bi(42);
    Box<std::string> bs("hello");
    std::cout << "Box<int>    = " << bi.get() << "\n";
    std::cout << "Box<string> = " << bs.get() << "\n";
    return 0;
}
