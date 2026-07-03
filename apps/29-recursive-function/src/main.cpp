#include <iostream>

// Recursive function to calculate factorial
int factorial(int n) {
    if (n <= 1) { // Base Case: When n is 0 or 1, the function returns 1.
        return 1;
    } else {
        return n * factorial(n - 1); // Recursive call - Recursive Case: The function calls itself with n - 1, reducing the problem size by one in each step.

        /*
        For example, to calculate factorial(5), the recursive calls look like this:
        5 * factorial(4)
        4 * factorial(3)
        3 * factorial(2)
        2 * factorial(1)
        The base case is reached at factorial(1), which returns 1.
        */
    }
}

int main() {
    int number = 5;
    std::cout << "Factorial of " << number << " is: " << factorial(number) << std::endl;
    return 0;
}
