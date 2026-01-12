# 18-void-functions

`void` functions don’t return a value. They’re used for actions with side effects (printing, logging, mutating state) or when a result isn’t needed.

## Basics

```cpp
void greet(const std::string& name) {
  std::cout << "Hello, " << name << "\n";
}
```

## Early return

You can exit a `void` function early with `return;`:
```cpp
void process(int x) {
  if (x < 0) return; // bail out
  // ... work ...
}
```

## Side effects and parameters

- Prefer `const&` parameters for read-only to avoid copies.
- Use references (`&`) to modify caller-owned data.

```cpp
void reset(std::vector<int>& v) {
  v.clear();
}
```

## Const correctness

Mark member functions `const` when they don’t modify the object:
```cpp
struct Printer {
  void print(const std::string& s) const {
    std::cout << s << "\n";
  }
};
```

## Small example

```cpp
#include <iostream>
#include <vector>

void log_values(const std::vector<int>& v) {
  for (int x : v) std::cout << x << ' ';
  std::cout << '\n';
}

void multiply_by_two(std::vector<int>& v) {
  for (int& x : v) x *= 2;
}

int main() {
  std::vector<int> nums{1,2,3};
  log_values(nums);       // 1 2 3
  multiply_by_two(nums);
  log_values(nums);       // 2 4 6
}
```

## Build and run (from repository root)

Run these from the repository root:
  - make build app=18-void-functions
  - make run app=18-void-functions

Binary path: build/18-void-functions/bin/18-void-functions

Alternative (from inside this folder):
  - cd app/18-void-functions
  - make run

This uses the per-app Makefile and still outputs to the centralized top-level build/ folder.
