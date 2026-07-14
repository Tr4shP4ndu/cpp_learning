// vec2.hpp — give your own type natural syntax with operator overloading.
#pragma once
#include <iosfwd>

// A 2D vector. We teach the compiler what +, ==, and << mean for it.
struct Vec2 {
    double x = 0.0;
    double y = 0.0;

    // Member operator: `a += b`.
    Vec2& operator+=(const Vec2& rhs);
};

// Non-member operators (symmetric operands read best as free functions).
Vec2 operator+(const Vec2& a, const Vec2& b);
bool operator==(const Vec2& a, const Vec2& b);

// Let Vec2 print with std::cout << v.
std::ostream& operator<<(std::ostream& os, const Vec2& v);
