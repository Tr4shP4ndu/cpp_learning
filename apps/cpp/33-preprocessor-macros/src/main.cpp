// 33-preprocessor-macros — text substitution that runs before the compiler.
#include <iostream>

// Object-like macro: a bare name replaced by text. Prefer `constexpr` in real
// code — a macro has no type and no scope.
#define GREETING "hello from the preprocessor"

// Function-like macro. Wrap every parameter AND the whole body in parentheses,
// or precedence bites you: without them, SQUARE(1 + 2) would expand to
// 1 + 2 * 1 + 2 == 5 instead of 9.
#define SQUARE(x) ((x) * (x))

// Conditional compilation: code the compiler only sees when the symbol is set.
#define ENABLE_EXTRA 1

int main() {
    std::cout << GREETING << '\n';
    std::cout << "SQUARE(1 + 2) = " << SQUARE(1 + 2) << '\n';  // 9, thanks to ()

#if ENABLE_EXTRA
    std::cout << "extra feature compiled in\n";
#else
    std::cout << "extra feature compiled out\n";
#endif

    // Predefined macros the compiler fills in automatically.
    std::cout << "this is line " << __LINE__ << " of " << __FILE__ << '\n';
    return 0;
}
