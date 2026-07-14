// 43-structs — bundle related values into one named type.
#include <iostream>
#include <string>

// A struct groups fields under one type. In C++ a struct is a class whose
// members are public by default — it can also have functions (see lesson 45).
struct Point {
    double x;
    double y;
};

struct Person {
    std::string name;
    int age;
};

int main() {
    Point p{3.0, 4.0};              // aggregate initialization, field by field
    std::cout << "point (" << p.x << ", " << p.y << ")\n";  // access with .

    Person alice{"Alice", 30};
    alice.age += 1;                 // modify a field
    std::cout << alice.name << " is " << alice.age << "\n";

    return 0;
}
