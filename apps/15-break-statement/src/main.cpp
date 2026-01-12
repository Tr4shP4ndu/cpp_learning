#include <iostream>

int main() {
    for (int i = 0; i < 10; ++i) {
        if (i == 5) {
            std::cout << "Break at i = " << i << std::endl;
            break; // Exit the loop when i equals 5
        }
        std::cout << "Current value of i: " << i << std::endl;
    }
    std::cout << "Loop exited." << std::endl;
    return 0;
}
