# 03-variables-datatypes (C)

The C version of [`apps/cpp/03-variables-datatypes`](../../cpp/03-variables-datatypes/).

## Concept
C's built-in numeric types match C++'s (`int`, `long`, `float`, `double`,
`char`). The differences are in *text* and in *printing*.

## Line-by-line
- `#include <stdint.h>` — fixed-width types (`int32_t`, `uint64_t`, …).
- `printf` needs a **format specifier** per value: `%d` int, `%u` unsigned,
  `%ld` long, `%f` floating, `%c` char, `%zu` `size_t`.
- `sizeof x` reports a type's size in bytes.
- A C "string" is a `char` array ending in a `'\0'` byte — no `std::string`.

## Compared to C++
C++ prints any type with `std::cout << x` (no format specifier) and gives you
`std::string` for text. See the [C++ version](../../cpp/03-variables-datatypes/).

## Build & run
```sh
make run-c app=03-variables-datatypes
```
