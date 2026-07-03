// 21-strings — std::string: text that grows, concatenates, and frees itself.
#include <iostream>
#include <string>

int main() {
    std::string first = "Ada";
    std::string last  = "Lovelace";

    // Concatenate with +, measure with .size(), index like an array.
    std::string full = first + " " + last;
    std::cout << "full   : " << full << "\n";
    std::cout << "length : " << full.size() << "\n";
    std::cout << "first  : " << full[0] << "\n";

    // Strings own their memory: append freely, no manual allocation.
    full += "!";
    std::cout << "append : " << full << "\n";

    // Handy members: substr, find.
    std::cout << "substr : " << full.substr(0, 3) << "\n";        // "Ada"
    std::cout << "find   : " << full.find("Love") << "\n";        // index 4

    return 0;
}
