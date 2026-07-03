#include "vec2.hpp"
#include <ostream>

Vec2& Vec2::operator+=(const Vec2& rhs) {
    x += rhs.x;
    y += rhs.y;
    return *this;          // return the modified object for chaining
}

Vec2 operator+(const Vec2& a, const Vec2& b) {
    Vec2 result = a;
    result += b;           // reuse operator+= — don't duplicate the logic
    return result;
}

bool operator==(const Vec2& a, const Vec2& b) {
    return a.x == b.x && a.y == b.y;
}

std::ostream& operator<<(std::ostream& os, const Vec2& v) {
    return os << "(" << v.x << ", " << v.y << ")";
}
