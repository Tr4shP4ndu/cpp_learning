// 37-value-semantics — by default, assigning or passing a value makes a COPY.
#include <iostream>

// The parameter `n` is a fresh copy of whatever was passed in.
void tryToChange(int n) {
    n = 100;  // changes only this local copy
    std::cout << "  inside function, n = " << n << "\n";
}

int main() {
    int a = 42;
    int b = a;   // b is a COPY of a — an independent value
    b = 7;       // changing b does not touch a
    std::cout << "a = " << a << ", b = " << b << "\n";   // a = 42, b = 7

    std::cout << "before call, a = " << a << "\n";
    tryToChange(a);   // a is copied into the parameter
    std::cout << "after call,  a = " << a << "\n";        // still 42

    return 0;
}
