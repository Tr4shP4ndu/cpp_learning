// main.cpp — the other translation unit; becomes main.o, then links to adder.o.
#include <iostream>
#include "adder.hpp"

int main() {
    std::cout << "sum_of_squares(3, 4) = " << sum_of_squares(3, 4) << "\n";
    return 0;
}
