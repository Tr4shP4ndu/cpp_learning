# hello_world

The first program you should write in any language is "Hello, World!".

## Minimal example

// File: src/main.cpp
#include <iostream>

int main() {
  std::cout << "Hello, World!\n";
  return 0; // optional in C++, implicit 0 if omitted
}

## Notes

- `#include <iostream>` pulls in the standard output stream `std::cout`.
- `main` is the program entry point; returning 0 signals success to the OS.
- The `\n` newline flushes in many environments; for immediate flush use `std::endl` (also forces flush) or flush explicitly.
- Avoid `using namespace std;` in headers and large scopes; qualify with `std::` for clarity.

## Common pitfalls

- Missing newline leaves the shell prompt on the same line as your output.
- Non-ASCII output may require setting the locale and a compatible terminal font.

## Build and run (from repository root)

Run these from the repository root:
  - make build app=hello_world
  - make run app=hello_world

Binary path: build/hello_world/bin/hello_world

Alternative (from inside this folder):
  - cd app/hello_world
  - make run

This uses the per-app Makefile and still outputs to the centralized top-level build/ folder.
