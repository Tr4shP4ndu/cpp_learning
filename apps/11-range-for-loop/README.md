# 11-range-for-loop

The range-based `for` loop (C++11+) iterates directly over elements of a range (arrays, `std::array`, `std::vector`, maps, strings, etc.). It’s cleaner and safer when you don’t need the index.

## Syntax

```cpp
for (declaration : range) {
  // use declaration
}
```

- `declaration` can be `auto`, `auto&`, or `const auto&` depending on whether you need copies, references, or read-only access.

## Examples

Iterate a `std::vector` read-only:
```cpp
std::vector<int> v{1, 2, 3};
for (const auto& x : v) {
  // read x
}
```

Modify elements:
```cpp
for (auto& x : v) {
  x *= 2; // modifies elements in-place
}
```

Work with `std::array`:
```cpp
std::array<std::string, 3> names{"Ada", "Bjarne", "Grace"};
for (const auto& name : names) {
  std::cout << name << "\n";
}
```

Iterate key/value pairs (maps):
```cpp
std::map<std::string, int> scores{{"alice", 10}, {"bob", 15}};
for (const auto& [key, value] : scores) { // C++17 structured bindings
  std::cout << key << ": " << value << "\n";
}
```

Strings:
```cpp
std::string s = "hello";
for (char c : s) {
  // use c
}
```

## When to use
- Use range-for when you only need the element, not the index.
- Use classic `for` when you need indices, custom steps, or cross-element relationships.
- Prefer `const auto&` for read-only to avoid copies; use `auto&` to modify.

## Pitfalls
- Range must be iterable (has `begin/end`). Raw C arrays are fine; pointers are not.
- Copy vs reference: `auto` copies each element; `const auto&` avoids copies.
- Modifying a container during iteration can invalidate iterators (for many containers).

## Small example
```cpp
#include <vector>
#include <iostream>

int main() {
  std::vector<int> nums{1, 2, 3, 4, 5};

  // Print
  for (const auto& x : nums) {
    std::cout << x << " ";
  }
  std::cout << "\n";

  // Double in-place
  for (auto& x : nums) {
    x *= 2;
  }

  // Print again
  for (const auto& x : nums) {
    std::cout << x << " ";
  }
  std::cout << "\n";
}
```

## Build and run (from repository root)

Run these from the repository root:
- `make build app=11-range-for-loop`
- `make run app=11-range-for-loop`

Binary path: `build/11-range-for-loop/bin/11-range-for-loop`

Alternative (from inside this folder):
- `cd app/11-range-for-loop`
- `make run`

Outputs go to the centralized top-level `build/` folder.
