# 15-break-statement

The `break` statement immediately exits the nearest enclosing loop or `switch` statement. Control continues with the first statement after that loop/switch.

## When to use
- Stop a search once a match is found (avoid extra work)
- Exit on error or sentinel condition
- Leave a `switch` case (common in `switch`/`case`)

## Examples

Exit a loop early when found:
```cpp
#include <vector>
#include <iostream>

int main() {
  std::vector<int> v{1, 3, 5, 8, 11};
  int index = -1;
  for (int i = 0; i < static_cast<int>(v.size()); ++i) {
    if (v[i] % 2 == 0) { index = i; break; }
  }
  std::cout << "First even index: " << index << "\n"; // prints 3
}
```

In `switch` statements:
```cpp
switch (code) {
  case 0: /* handle */ break;
  case 1: /* handle */ break;
  default: /* handle */ break;
}
```

Nested loops (break exits only the inner loop):
```cpp
for (int r = 0; r < R; ++r) {
  for (int c = 0; c < C; ++c) {
    if (/* condition */) { break; } // leaves the inner 'for'
  }
  // execution continues here after inner break
}
```

## Pitfalls
- `break` only exits one level; for multi-level exits, consider flags, functions returning early, or `goto` (sparingly).
- Overusing `break` can reduce clarity; prefer structured control flow where possible.

## See also
- `continue`: skip to the next iteration
- `return`: exit the current function

## Build and run (from repository root)

Run these from the repository root:
  - make build app=15-break-statement
  - make run app=15-break-statement

Binary path: build/15-break-statement/bin/15-break-statement

Alternative (from inside this folder):
  - cd app/15-break-statement
  - make run

This uses the per-app Makefile and still outputs to the centralized top-level build/ folder.
