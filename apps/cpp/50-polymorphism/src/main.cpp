#include <iostream>
#include <memory>
#include <vector>
#include "shapes.hpp"

int main() {
    // A container of base-class pointers, each pointing at a different derived
    // object. We use unique_ptr (lesson 42) so cleanup is automatic.
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::make_unique<Circle>(2.0));
    shapes.push_back(std::make_unique<Square>(3.0));

    // The SAME call, area()/name(), dispatches to the correct derived version
    // at runtime. That is polymorphism.
    for (const auto& s : shapes) {
        std::cout << s->name() << " area = " << s->area() << "\n";
    }
    return 0;
}
