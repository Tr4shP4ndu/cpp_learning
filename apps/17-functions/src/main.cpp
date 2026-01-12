#include <iostream>

// Function declaration
int add(int num1, int num2) {
    return num1 + num2; // Return the sum of num1 and num2
}
int main() {
    std::cout << "The sum of " << num1 << " and " << num2 << " is: " << add(1, 2) << std::endl;
    return 0;
}

/* Forward Declaration
#include <iostream>
// Foward Declared
// Function declaration
int add(int a, int b);

int main() {
    std::cout << "The sum of " << num1 << " and " << num2 << " is: " << add(1, 2) << std::endl;
    return 0;
}

int add(int num1, int num2) {
    return num1 + num2; // Return the sum of num1 and num2
}
    */
