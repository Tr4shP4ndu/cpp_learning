// 19-std-array — std::array: a raw array that knows its size and stays safe.
#include <iostream>
#include <array>
#include <numeric>  // std::iota fills with increasing values

int main() {
    // Same fixed size as a raw array, but it's a real object: it knows its
    // length, can be copied, and offers bounds-checked access.
    std::array<int, 5> ids{};

    // Fill with 0, 1, 2, 3, 4.
    std::iota(ids.begin(), ids.end(), 0);

    std::cout << "size() = " << ids.size() << "\n";

    // .at(i) does a bounds check and throws std::out_of_range if i is invalid.
    // ids.at(99) would throw here. operator[] (ids[i]) does NOT check.
    ids.at(4) = 42;

    for (int id : ids) {
        std::cout << id << "\n";
    }
    return 0;
}
