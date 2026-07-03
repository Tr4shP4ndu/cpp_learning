// 01-hello-world — the smallest complete C++ program that produces output.
#include <iostream>  // brings in std::cout, the standard output stream

int main() {
    // std::cout is the console. operator<< sends text to it.
    // "\n" ends the line. Prefer it over std::endl unless you need a flush.
    std::cout << "Hello, World!\n";
    return 0;  // 0 tells the operating system "the program succeeded"
}
