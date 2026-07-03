#include <iostream>

int main() {
    int i = 0;
    do {
        std::cout << "Do-while loop iteration: " << i << std::endl;
        ++i;
    } while (i < 5); // Loop continues while i is less than 5
    return 0;
}
