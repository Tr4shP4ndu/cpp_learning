// 14-while-loop — repeat while a condition holds; check happens first.
#include <iostream>

int main() {
    int i = 0;
    // The condition is tested BEFORE each pass. If it's false at the start,
    // the body never runs. Use while when you don't know the count up front.
    while (i < 5) {
        std::cout << "iteration " << i << "\n";
        ++i;  // advance toward the exit condition — forget this and it loops forever
    }
    return 0;
}
