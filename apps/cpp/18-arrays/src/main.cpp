// 18-arrays — a fixed-size, contiguous block of same-typed values (C style).
#include <iostream>

int main() {
    // A C-style array: the size is part of the type and cannot change.
    int scores[5] = {10, 20, 30, 40, 50};

    // Indexing is zero-based: valid indices are 0..4.
    for (int i = 0; i < 5; ++i) {
        std::cout << "scores[" << i << "] = " << scores[i] << "\n";
    }

    // A raw array does NOT know its own length at runtime — you must track it.
    // sizeof gives the total bytes; divide by one element to get the count.
    int count = sizeof(scores) / sizeof(scores[0]);
    std::cout << "element count = " << count << "\n";

    // scores[5] would read past the end -> undefined behavior (ASan will flag).
    return 0;
}
