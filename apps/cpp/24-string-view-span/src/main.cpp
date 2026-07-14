// 25-string-view-span — non-owning views over characters and contiguous data.
#include <iostream>
#include <span>
#include <string>
#include <string_view>
#include <vector>

// Takes a view, not a std::string: no copy, and it accepts std::string,
// string literals, and substrings alike.
static void greet(std::string_view name) {
    std::cout << "hello, " << name << " (" << name.size() << " chars)\n";
}

// Takes a span: one function works for C arrays, std::array, and std::vector.
static int sum(std::span<const int> values) {
    int total = 0;
    for (int v : values) total += v;
    return total;
}

int main() {
    std::string owned = "world";
    greet(owned);        // from a std::string
    greet("literal");    // from a string literal — no temporary std::string

    // string_view can cheaply "slice" without allocating:
    std::string_view sv = owned;
    std::cout << "first two: " << sv.substr(0, 2) << '\n';

    int raw[] = {1, 2, 3, 4};
    std::vector<int> vec = {10, 20, 30};
    std::cout << "sum(raw) = " << sum(raw) << '\n';   // span deduces size 4
    std::cout << "sum(vec) = " << sum(vec) << '\n';   // same function, a vector
    return 0;
}
