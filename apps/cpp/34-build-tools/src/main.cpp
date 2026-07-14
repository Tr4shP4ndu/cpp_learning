// 34-build-tools — the program can see how it was built.
#include <iostream>

int main() {
    // __cplusplus encodes the language standard the compiler used.
    std::cout << "__cplusplus = " << __cplusplus << '\n';

    // Which compiler compiled this translation unit?
#if defined(__clang__)
    std::cout << "compiler: clang " << __clang_major__ << '.' << __clang_minor__ << '\n';
#elif defined(__GNUC__)
    std::cout << "compiler: gcc " << __GNUC__ << '.' << __GNUC_MINOR__ << '\n';
#else
    std::cout << "compiler: unknown\n";
#endif

    // NDEBUG is defined by release builds (-DNDEBUG); it also disables assert().
#ifdef NDEBUG
    std::cout << "build type: release (NDEBUG defined)\n";
#else
    std::cout << "build type: debug\n";
#endif
    return 0;
}
