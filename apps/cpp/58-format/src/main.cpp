// 58-format — type-safe, Python-style string formatting (C++20/23).
#include <format>
#include <iostream>
#include <string>

int main() {
    std::string who = "world";
    std::cout << std::format("hello, {}!\n", who);

    // Positional args: reuse and reorder by index.
    std::cout << std::format("{0} + {0} = {1}\n", 2, 4);

    // Format specs after ':' — alignment, width, precision, base.
    std::cout << std::format("|{:>8}|\n", "right");   // right-align in width 8
    std::cout << std::format("|{:<8}|\n", "left");    // left-align
    std::cout << std::format("pi ~= {:.3f}\n", 3.14159);
    std::cout << std::format("255 in hex = {:#x}\n", 255);

    // std::format returns a std::string you can store, not just print.
    std::string line = std::format("[{:04d}] {}", 42, "logged");
    std::cout << line << '\n';
    return 0;
}
