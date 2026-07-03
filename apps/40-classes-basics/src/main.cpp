// main.cpp — create objects (instances) and call their methods.
#include <iostream>
#include "rectangle.hpp"

int main() {
    Rectangle r;          // an object of type Rectangle
    r.width = 3.0;        // set members with .
    r.height = 4.0;

    std::cout << "area  = " << r.area() << "\n";   // 12 — call a method with .

    r.scale(2.0);
    std::cout << "after scale(2): " << r.width << " x " << r.height
              << ", area = " << r.area() << "\n";  // 6 x 8, area 48

    return 0;
}
