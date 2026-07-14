// rectangle.cpp — define the methods declared in the header.
#include "rectangle.hpp"

// `ClassName::method` says this definition belongs to Rectangle.
// Inside, width/height refer to the current object's members.
double Rectangle::area() const {
    return width * height;
}

void Rectangle::scale(double factor) {
    width  *= factor;
    height *= factor;
}
