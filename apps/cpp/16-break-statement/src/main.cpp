// 16-break-statement — leave a loop early, before its condition would end it.
#include <iostream>

int main() {
    // Search for the first value >= 30, then stop.
    int data[] = {10, 20, 30, 40, 50};
    for (int value : data) {
        if (value >= 30) {
            std::cout << "found " << value << ", stopping\n";
            break;  // jump straight out of the loop
        }
        std::cout << "skipping " << value << "\n";
    }
    std::cout << "loop exited\n";
    return 0;
}
