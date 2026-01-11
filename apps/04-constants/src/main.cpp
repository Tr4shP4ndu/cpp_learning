#include <iostream>
#include <type_traits>  // For std::is_const
#include <typeinfo>     // For typeid

int main() {
    /*
    The `const` keyword specifies that a variable's value is constant.
    This tells the compiler to prevent anything from modifying it (read-only).
    It's a common convention to name constants using all uppercase letters.
    */

    const double PI = 3.14159;
    // PI = 420.69; // This line would cause a compilation error because PI is constant.

    double radius = 10.0;
    double circumference = 2 * PI * radius;

    // Check if PI is a constant
    std::cout << "Is PI constant? " << std::is_const<decltype(PI)>::value << std::endl;  // Output: 1 (true)

    // Display the type of PI using typeid and decltype
    std::cout << "Type of PI: " << typeid(decltype(PI)).name() << std::endl;  // Output: d (double)

    std::cout << "The circumference is " << circumference << " cm." << std::endl;  // Output: 62.8318 cm

    return 0;
}
