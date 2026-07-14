// rectangle.hpp — a class bundles DATA (members) with BEHAVIOR (methods).
#pragma once

class Rectangle {
public:
    // Data members — the state each Rectangle object carries.
    double width;
    double height;

    // Member functions (methods) — operate on THIS object's members.
    double area() const;      // `const`: doesn't modify the object
    void scale(double factor); // modifies width/height
};
