// 11-typedef-aliases — give a readable name to an existing type.
#include <iostream>
#include <string>
#include <vector>

// The old C way: typedef <existing type> <alias>;
typedef std::string text_t;

// The modern C++ way: using <alias> = <existing type>;  (reads left-to-right,
// and works with templates — prefer this).
using number_t = int;
using pair_list_t = std::vector<std::pair<std::string, int>>;

int main() {
    text_t   firstName = "Tr4shP4ndu";
    number_t age = 21;

    // The verbose type on the left is now just `pair_list_t`.
    pair_list_t scores = { {"math", 90}, {"art", 88} };

    std::cout << firstName << " is " << age << "\n";
    for (const auto& [subject, score] : scores)
        std::cout << subject << ": " << score << "\n";

    return 0;
}
