// 36-debugging — tools for finding out why code misbehaves.
#include <cassert>
#include <iostream>

// static_assert checks a condition at COMPILE time; it never runs.
static_assert(sizeof(int) >= 2, "int is unexpectedly small");

static int factorial(int n) {
    // A precondition, checked with assert(): in a debug build this aborts with
    // the failing expression, file, and line if n is ever negative. A release
    // build (-DNDEBUG) removes the check entirely.
    assert(n >= 0 && "factorial expects a non-negative argument");
    int result = 1;
    for (int i = 2; i <= n; ++i) result *= i;
    return result;
}

int main() {
    std::cout << "factorial(5) = " << factorial(5) << '\n';

    // Printf-style tracing: the humble but effective first tool. Print a value
    // and where it came from so you can follow the flow.
    for (int n = 0; n <= 3; ++n)
        std::cout << "[trace] n=" << n << " -> " << factorial(n) << '\n';

    // Try uncommenting this: in a debug build the assert fires and names the
    // failing condition — that is the bug caught at its source.
    // std::cout << factorial(-1) << '\n';
    return 0;
}
