// 38-references — a reference is another NAME for an existing variable.
#include <iostream>
#include <string>

// Taking the parameter by reference (&) lets the function change the caller's
// variable directly — no copy is made.
void addBang(std::string& text) {
    text += "!";
}

int main() {
    int value = 42;
    int& ref = value;   // ref is an alias for value — not a copy

    ref = 43;           // writing through ref writes to value
    std::cout << "value = " << value << "\n";   // 43
    std::cout << "&value == &ref ? " << (&value == &ref) << "\n";  // 1: same object

    std::string message = "hello";
    addBang(message);   // passed by reference
    std::cout << "message = " << message << "\n";   // hello!

    return 0;
}
