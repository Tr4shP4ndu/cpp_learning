// 05-io-basics — read input from the keyboard, write output to the screen.
#include <iostream>
#include <string>

int main() {
    // Output with std::cout, input with std::cin. ">>" reads one whitespace-
    // separated token; std::getline reads a whole line (spaces included).

    std::cout << "What is your name? ";
    std::string name;
    std::getline(std::cin, name);  // read the full line into `name`

    std::cout << "How old are you? ";
    int age = 0;
    std::cin >> age;               // read an integer

    std::cout << "Hello, " << name << "! Next year you'll be " << age + 1 << ".\n";
    return 0;
}
