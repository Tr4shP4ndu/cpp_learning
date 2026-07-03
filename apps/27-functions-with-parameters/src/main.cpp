// 27-functions-with-parameters — how arguments get passed: by value vs by ref.
#include <iostream>
#include <string>

// Pass by value: `x` and `y` are COPIES. Cheap for small types like int.
int multiply(int x, int y) {
    return x * y;
}

// Pass by value for a big object COPIES the whole thing — wasteful.
// Pass by const reference (const T&) to see it without copying, and promise
// not to modify it. This is the default for strings, vectors, and other
// non-trivial types.
std::size_t lengthOf(const std::string& text) {
    return text.size();
}

// A plain (non-const) reference lets the function MODIFY the caller's variable.
void doubleInPlace(int& n) {
    n *= 2;
}

int main() {
    std::cout << "multiply(4, 5) = " << multiply(4, 5) << "\n";

    std::string message = "hello world";
    std::cout << "lengthOf(...)  = " << lengthOf(message) << "\n";

    int value = 21;
    doubleInPlace(value);   // value is changed by the function
    std::cout << "after doubleInPlace, value = " << value << "\n";
    return 0;
}
