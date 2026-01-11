# 09-std:arrays

`std::array<T, N>` is a fixed-size, stack-allocated container with the safety and convenience of the C++ Standard Library. Prefer it over raw arrays when size is known at compile time.

## Why use std::array?
- Size known at compile time; allocated inline (no heap by default)
- Provides `.size()`, `.at()`, `.front()`, `.back()`, iterators
- Works with STL algorithms (begin/end)
- Safer than raw arrays (bounds-checked access via `.at()`)

## Basic usage

```cpp
#include <array>

std::array<int, 5> numbers = {10, 20, 30, 40, 50};
int first = numbers[0];        // unchecked
int safe  = numbers.at(1);     // throws std::out_of_range if invalid
static_assert(numbers.size() == 5);
```

## Iteration patterns

Indexed loop:
```cpp
for (std::size_t i = 0; i < numbers.size(); ++i) {
  // use numbers[i]
}
```

Range-based loop (preferred when you don’t need the index):
```cpp
for (const auto& value : numbers) {
  // read value
}
for (auto& value : numbers) {
  // modify value
}
```

## Common operations

Fill values:
```cpp
numbers.fill(0);
```

Raw pointer (interop with C APIs):
```cpp
int* ptr = numbers.data();
```

Algorithms:
```cpp
#include <algorithm>

std::array<int, 5> a = {5, 1, 4, 3, 2};
std::sort(a.begin(), a.end());
```

## Pitfalls and tips
- The size is part of the type: `std::array<int, 3>` is different from `std::array<int, 4>`
- `.at()` is bounds-checked; `operator[]` is not
- Prefer `std::array` over raw arrays for STL compatibility and safety
- Use `std::vector` when size is dynamic or unknown at compile time

## Example

```cpp
#include <array>
#include <iostream>

int main() {
  std::array<int, 5> nums = {10, 20, 30, 40, 50};

  // Print elements
  for (const auto& x : nums) {
    std::cout << x << "\n";
  }

  // Safe access
  try {
    std::cout << nums.at(5) << "\n"; // throws
  } catch (const std::out_of_range& e) {
    std::cerr << "Out of range: " << e.what() << "\n";
  }
}
```

## Build and run (from repository root)

Run these from the repository root:
- `make build app=09-std::arrays`
- `make run app=09-std::arrays`

Binary path: `build/09-std::arrays/bin/09-std::arrays`

Alternative (from inside this folder):
- `cd app/09-std::arrays`
- `make run`

This uses the per-app Makefile and still outputs to the centralized top-level `build/` folder.
