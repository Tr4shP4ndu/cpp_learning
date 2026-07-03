// 36-dynamic-memory — allocate memory at run time; you must free exactly once.
#include <iostream>

int main() {
    // `new` allocates one int on the heap and returns a pointer to it.
    int* p = new int(42);
    std::cout << "*p = " << *p << "\n";
    delete p;          // hand it back. Forgetting this is a memory LEAK.
    p = nullptr;       // avoid accidentally reusing a freed pointer

    // `new[]` allocates an array; it must be released with `delete[]`.
    int n = 5;
    int* arr = new int[n];
    for (int i = 0; i < n; ++i) arr[i] = i * i;
    std::cout << "arr[4] = " << arr[4] << "\n";
    delete[] arr;      // note: delete[], not delete, for arrays
    arr = nullptr;

    std::cout << "all heap memory released\n";
    // The lesson: this manual pairing is error-prone. Lesson 37 (smart
    // pointers) makes the release automatic so you can't forget.
    return 0;
}
