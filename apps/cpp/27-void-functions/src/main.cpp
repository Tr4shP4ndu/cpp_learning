// 27-void-functions — functions that DO something but return no value.
#include <iostream>
#include <string>

// `void` means "returns nothing". Called for its side effect (here, printing).
void printBanner() {
    std::cout << "==============\n";
}

// A void function can still take parameters.
void greet(const std::string& name) {
    std::cout << "Hello, " << name << "!\n";
    // A bare `return;` (optional) can exit early; there's no value to return.
}

int main() {
    printBanner();
    greet("Ada");
    printBanner();
    return 0;
}
