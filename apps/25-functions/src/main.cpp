// 25-functions — package reusable logic behind a name; call it as needed.
#include <iostream>

// A forward DECLARATION (prototype): the name, parameters, and return type.
// It lets main() call add() before the DEFINITION appears below.
int add(int a, int b);

int main() {
    // A function call: pass arguments, get a return value back.
    std::cout << "add(1, 2)  = " << add(1, 2) << "\n";
    std::cout << "add(10, 5) = " << add(10, 5) << "\n";
    return 0;
}

// The DEFINITION supplies the body. A function has exactly one definition
// but may be declared many times (the "one definition rule").
int add(int a, int b) {
    return a + b;
}
