# 12-while-loop

A `while` loop repeats a block as long as a condition stays true. Use it when the iteration count isn’t known ahead of time.

## Syntax

```cpp
while (condition) {
  // body
}
```

`do { /* body */ } while (condition);` runs the body at least once.

## Examples

Read until EOF:
```cpp
#include <iostream>
#include <string>

int main() {
  std::string line;
  while (std::getline(std::cin, line)) {
    std::cout << line << "\n";
  }
}
```

Count down:
```cpp
int n = 5;
while (n > 0) {
  // work with n
  --n;
}
```

`do-while` menu (runs at least once):
```cpp
int choice;
do {
  // print menu, read choice
  std::cin >> choice;
} while (choice != 0);
```

## Pitfalls
- Make sure the loop condition eventually becomes false (avoid infinite loops).
- Update variables in the loop that affect the condition.
- Validate input; failed extraction (e.g., `std::cin >> x`) sets `failbit`, so check state.

## When to use while vs for
- Use `while` when the end condition depends on runtime state or input.
- Use `for` when the iteration count or step is known upfront.

## Build and run (from repository root)

Run these from the repository root:
- `make build app=12-while-loop`
- `make run app=12-while-loop`

Binary path: `build/12-while-loop/bin/12-while-loop`

Alternative (from inside this folder):
- `cd app/12-while-loop`
- `make run`

Outputs go to the centralized top-level `build/` folder.
