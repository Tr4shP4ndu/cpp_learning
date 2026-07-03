#include "shapes.hpp"

Circle::Circle(double radius) : radius_(radius) {}
double Circle::area() const { return 3.14159265 * radius_ * radius_; }
std::string Circle::name() const { return "Circle"; }

Square::Square(double side) : side_(side) {}
double Square::area() const { return side_ * side_; }
std::string Square::name() const { return "Square"; }
