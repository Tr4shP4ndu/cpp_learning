// 06-operators — the symbols that compute with values: arithmetic, comparison,
// logical, bitwise, and increment. (asserts double as a self-check.)
#include <iostream>
#include <cassert>

int main() {
    int a = 7, b = 3;

    // Arithmetic. Note: int / int truncates toward zero.
    assert(a + b == 10);
    assert(a - b == 4);
    assert(a * b == 21);
    assert(a / b == 2);    // 2, not 2.33 — integer division
    assert(a % b == 1);    // remainder

    // Cast one side to double for real division.
    double real = static_cast<double>(a) / b;
    std::cout << "7 / 3 as int    = " << a / b  << "\n";
    std::cout << "7 / 3 as double = " << real   << "\n";

    // Comparison operators produce a bool (prints as 1/0).
    assert((a > b) == true);
    assert((a == b) == false);

    // Logical operators short-circuit: the right side is skipped when the
    // result is already known. This makes null-checks safe.
    int* p = nullptr;
    if (p != nullptr && *p > 0) { /* *p is never evaluated when p is null */ }

    // Bitwise operators work on the individual bits of integers.
    unsigned flags = 0b0101;   // 5
    flags |= 0b0010;           // set a bit  -> 0b0111 (7)
    flags &= ~0b0001u;         // clear a bit -> 0b0110 (6)
    assert(flags == 0b0110);

    // Increment: prefix changes-then-reads, postfix reads-then-changes.
    int x = 5;
    assert(++x == 6);   // x is now 6, expression is 6
    assert(x++ == 6);   // expression is 6, then x becomes 7
    assert(x == 7);

    std::cout << "all operator checks passed\n";
    return 0;
}
