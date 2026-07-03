// 39-enums — a type with a fixed set of named values.
#include <iostream>
#include <string>

// Prefer `enum class` (scoped): values are namespaced and don't implicitly
// convert to int, which prevents a whole category of bugs.
enum class Direction { North, East, South, West };

// A plain (unscoped) enum leaks its names into the surrounding scope and
// converts to int freely — occasionally handy, usually a footgun.
enum Color { Red, Green, Blue };

std::string name(Direction d) {
    switch (d) {
        case Direction::North: return "North";
        case Direction::East:  return "East";
        case Direction::South: return "South";
        case Direction::West:  return "West";
    }
    return "?";
}

int main() {
    Direction d = Direction::East;         // must qualify with the enum name
    std::cout << "direction: " << name(d) << "\n";

    Color c = Green;                        // unscoped: no qualification needed
    std::cout << "color as int: " << c << "\n";   // 1 — implicit int conversion

    return 0;
}
