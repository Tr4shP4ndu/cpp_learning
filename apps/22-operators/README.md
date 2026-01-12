# 22-operators

- Operators let you perform actions on values and objects. C++ has rich operator sets with specific precedence and associativity rules.

## Core categories

- Arithmetic: + - * / % (modulo)
- Assignment: =, compound: += -= *= /= %=, and with bitwise/logical variants
- Comparison: == != < <= > >=
- Logical: && || ! (short-circuit)
- Bitwise: & | ^ ~ << >>
- Increment/Decrement: ++ -- (prefix vs postfix)
- Pointer-related: & (address-of), * (dereference)
- Member access: . and ->, pointer-to-member: .* and ->*
- Subscript and call: [] ()
- Ternary conditional: cond ? a : b
- Type-related: sizeof, alignof, typeid, static_cast/dynamic_cast/reinterpret_cast/const_cast
- New/delete: new, delete, new[], delete[]
- Other: comma operator (,), scope (::), co_yield/co_await (coroutines), etc.

## Precedence and associativity (essentials)

- Higher-precedence operators bind tighter; use parentheses to make intent clear.
- Most binary operators are left-associative; assignment and conditional are right-associative.
- Don't memorize the full table; prefer parentheses in complex expressions.

## Short-circuiting

- Logical && and || evaluate left to right and short-circuit:
  - a && b: b is evaluated only if a is true.
  - a || b: b is evaluated only if a is false.
- Use this to guard operations (e.g., pointer checks) without extra if statements.

## Examples

// Arithmetic and comparison
int a = 7, b = 3;
std::cout << a / b << "\n";   // 2 (integer division)
std::cout << a % b << "\n";   // 1 (remainder)
std::cout << (a > b) << "\n"; // 1 (true)

// Increment: prefix vs postfix
int x = 5;
std::cout << ++x << "\n"; // 6 (x becomes 6, value is 6)
std::cout << x++ << "\n"; // 6 (value is 6, x becomes 7)

// Logical short-circuit
int* p = nullptr;
if (p && *p > 0) { /* safe: *p not evaluated when p==nullptr */ }

// Bitwise
unsigned flags = 0b0101;     // 5
flags |= 0b0010;             // set bit 1 -> 0b0111
flags &= ~0b0001;            // clear bit 0 -> 0b0110

// Address-of and dereference
int v = 10; int* ptr = &v; *ptr = 20; // v becomes 20

// Conditional operator
int y = (v > 15) ? 1 : 0;

## Pitfalls and tips

- Integer division truncates; cast to double for floating-point: static_cast<double>(a) / b.
- Beware of undefined behavior with signed overflow and shifting negatives.
- Postfix ++/-- returns the old value; prefer prefix when possible for clarity and efficiency.
- Operator precedence surprises: use parentheses; e.g., a + b << 1 is (a + b) << 1 due to precedence.
- Don't mix logical and bitwise operators accidentally: && vs & and || vs | have different semantics.
- For user-defined types, overload operators carefully to preserve expected meaning and avoid surprising side effects.

## Build and run (from repository root)

Run these from the repository root:
  - make build app=22-operators
  - make run app=22-operators

Binary path: build/22-operators/bin/22-operators

Alternative (from inside this folder):
  - cd app/22-operators
  - make run

This uses the per-app Makefile and still outputs to the centralized top-level build/ folder.
