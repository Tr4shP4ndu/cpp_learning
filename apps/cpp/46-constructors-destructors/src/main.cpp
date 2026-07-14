#include <iostream>
#include "buffer.hpp"

int main() {
    std::cout << "entering main\n";
    {
        Buffer b(4);        // constructor runs here — memory acquired
        b.set(0, 10);
        b.set(3, 40);
        std::cout << "b[0] = " << b.get(0) << ", b[3] = " << b.get(3) << "\n";
    }                       // b leaves scope here — destructor runs, memory freed
    std::cout << "left the block; buffer already released\n";
    return 0;
}
