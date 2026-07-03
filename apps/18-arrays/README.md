# 07-arrays

C-style arrays are fixed-size, contiguous blocks of elements with minimal features. They’re lightweight but less safe than `std::array` or `std::vector`.

## Basics

- Declaration with compile-time size:
  - `int a[5];`              // uninitialized
  - `int b[5] = {1,2,3,4,5};` // aggregate initialization
- Size is not stored: use a constant or `std::size(b)` (C++17) if available.

## Access and iteration

- Indexing:
  - `int x = b[0];` // unchecked; out-of-bounds is undefined behavior
- Classic for loop:
  - `for (int i = 0; i < 5; ++i) { /* b[i] */ }`
- Range-based for (elements only):
  - `for (int x : b) { /* use x */ }` (works if array size can be deduced)

## Common pitfalls

- No bounds checking: `b[5]` is UB for `int b[5]`.
- No `.size()` or member functions.
- Decays to pointer when passed to functions; size information is lost.
- Harder to use with STL algorithms; prefer `std::array` or `std::vector`.

## When to use

- Very simple, fixed-size buffers where overhead must be minimal.
- Prefer `std::array<T, N>` for safety and STL compatibility when size is known.

## Example

```cpp
#include <iostream>

int main() {
    int numbers[5] = {10, 20, 30, 40, 50};
    for (int index = 0; index < 5; ++index) {
        std::cout << "Element at index " << index << ": " << numbers[index] << "\n";
    }

    // Unsafe: out of bounds (undefined behavior)
    // std::cout << numbers[5] << "\n";
}
```

## Build and run (from repository root)

Run these from the repository root:
- `make build app=08-arrays`
- `make run app=08-arrays`

Binary path: `build/08-arrays/bin/08-arrays`

Alternative (from inside this folder):
- `cd app/08-arrays`
- `make run`

This uses the per-app Makefile and still outputs to the centralized top-level `build/` folder.
