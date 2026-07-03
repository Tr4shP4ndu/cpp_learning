# 14-std::fill

`std::fill` (in `<algorithm>`) sets every element in a half-open range `[first, last)` to a given value. It works with arrays, `std::array`, `std::vector`, and any container with iterators.

## Basic usage

```cpp
#include <algorithm>
#include <array>

std::array<int, 5> a{};         // {0,0,0,0,0}
std::fill(a.begin(), a.end(), 42); // {42,42,42,42,42}
```

With `std::vector`:
```cpp
#include <vector>

std::vector<int> v(10);              // 10 default-initialized ints (0)
std::fill(v.begin() + 2, v.end(), 7); // set elements [2..end) to 7
```

## Variants

- `std::fill_n(it, count, value)`: writes `count` elements starting at `it`.
- `std::ranges::fill(rng, value)`: C++20 ranges-friendly overload that takes a range directly.

```cpp
#include <algorithm>
#include <vector>

std::vector<int> v(5);
std::fill_n(v.begin(), 3, 1); // v = {1,1,1,0,0}
```

## Tips and pitfalls

- The range is half-open: `[first, last)`. Ensure `first <= last` and iterators belong to the same container.
- For non-trivial types, `value` is copied into each element (copy assignment invoked).
- Prefer algorithms like `std::fill` over manual loops for clarity and fewer errors.

## Small example

```cpp
#include <algorithm>
#include <array>
#include <iostream>

int main() {
  std::array<int, 6> digits{};             // {0,0,0,0,0,0}
  std::fill(digits.begin(), digits.end(), 9); // {9,9,9,9,9,9}

  std::fill(digits.begin() + 2, digits.begin() + 4, 3); // {9,9,3,3,9,9}

  for (const auto& x : digits) std::cout << x << ' ';
  std::cout << '\n';
}
```

## Build and run (from repository root)

Run these from the repository root:
  - make build app=14-std::fill
  - make run app=14-std::fill

Binary path: build/14-std::fill/bin/14-std::fill

Alternative (from inside this folder):
  - cd app/14-std::fill
  - make run

This uses the per-app Makefile and still outputs to the centralized top-level build/ folder.
