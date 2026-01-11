# 13-do-while-loop

A `do-while` loop executes the body first, then checks the condition. Use it when the loop must run at least once (e.g., menus, input prompts).

## Syntax

```cpp
do {
  // body
} while (condition);
```

## Examples

Input prompt (runs at least once):
```cpp
#include <iostream>

int main() {
  int choice = -1;
  do {
    std::cout << "Enter 0 to quit: ";
    std::cin >> choice;
  } while (choice != 0);
}
```

Validate input:
```cpp
#include <iostream>
#include <limits>

int value;
do {
  std::cout << "Enter a positive number: ";
  if (!(std::cin >> value)) { // handle bad input
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    value = -1;
  }
} while (value <= 0);
```

## Pitfalls
- Body runs once even if condition is false initially.
- Be careful with input stream state (`failbit`) and clearing invalid input.
- Ensure the condition can become false to avoid infinite loops.

## When to use
- Use `do-while` for at-least-one execution scenarios (menus, retries).
- Use `while` when you may skip the body entirely if the condition is false.

## Build and run (from repository root)

Run these from the repository root:
- `make build app=13-do-while-loop`
- `make run app=13-do-while-loop`

Binary path: `build/13-do-while-loop/bin/13-do-while-loop`

Alternative (from inside this folder):
- `cd app/13-do-while-loop`
- `make run`

Outputs go to the centralized top-level `build/` folder.
