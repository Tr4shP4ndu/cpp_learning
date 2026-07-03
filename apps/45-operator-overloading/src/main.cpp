#include <iostream>
#include <cassert>
#include "vec2.hpp"

int main() {
    Vec2 a{1.0, 2.0};
    Vec2 b{3.0, 4.0};

    Vec2 c = a + b;               // calls operator+
    std::cout << a << " + " << b << " = " << c << "\n";  // uses operator<<

    assert((c == Vec2{4.0, 6.0}));   // uses operator==

    a += b;                       // calls operator+=
    assert((a == c));
    std::cout << "operator checks passed\n";
    return 0;
}
