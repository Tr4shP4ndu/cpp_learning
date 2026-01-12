#include <iostream>

// Function to add integers
int add(int x, int y) {
    return x + y;
}

// Overloaded function to add doubles
double add(double x, double y) {
    return x + y;
}

int main() {
    std::cout << "Integer sum: " << add(3, 4) << std::endl;
    std::cout << "Double sum: " << add(3.5, 4.5) << std::endl;
    return 0;
}
