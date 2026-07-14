// 19-std-array — std::array: a raw array that knows its size and stays safe,
// plus std::iota / std::fill: your first standard algorithms.
#include <iostream>
#include <array>
#include <algorithm>  // std::fill
#include <numeric>    // std::iota

int main() {
    // Same fixed size as a raw array, but it's a real object: it knows its
    // length, can be copied, and offers bounds-checked access.
    std::array<int, 5> ids{};

    // std::iota and std::fill are standard algorithms: reusable functions that
    // work on any container through the half-open [begin, end) range, so you
    // don't hand-write the loop.
    std::iota(ids.begin(), ids.end(), 0);   // fill with 0, 1, 2, 3, 4

    std::cout << "size() = " << ids.size() << "\n";

    // .at(i) does a bounds check and throws std::out_of_range if i is invalid.
    // ids.at(99) would throw here. operator[] (ids[i]) does NOT check.
    ids.at(4) = 42;

    for (int id : ids) std::cout << id << "\n";

    // std::fill sets every element in [begin, end) to one value — clearer and
    // less error-prone than a hand-written loop.
    std::array<int, 5> flags{};
    std::fill(flags.begin(), flags.end(), 1);
    std::cout << "flags filled: " << flags[0] << " ... " << flags[4] << "\n";
    return 0;
}
