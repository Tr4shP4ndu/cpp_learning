// 25-iterators — a generalized pointer that walks any container.
#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::vector<int> v = {10, 20, 30, 40};

    // begin() points at the first element; end() points ONE PAST the last.
    // The half-open range [begin, end) is the vocabulary every STL algorithm
    // speaks. An iterator dereferences with *, like a pointer.
    for (std::vector<int>::iterator it = v.begin(); it != v.end(); ++it)
        std::cout << *it << ' ';
    std::cout << '\n';

    // A range-based for loop is just sugar over begin()/end().
    for (int x : v) std::cout << x << ' ';
    std::cout << '\n';

    // Algorithms take and return iterators. find returns end() on no match.
    auto found = std::find(v.begin(), v.end(), 30);
    if (found != v.end())
        std::cout << "found 30 at index " << (found - v.begin()) << '\n';

    // Reverse iterators walk back to front.
    for (auto it = v.rbegin(); it != v.rend(); ++it) std::cout << *it << ' ';
    std::cout << '\n';
    return 0;
}
