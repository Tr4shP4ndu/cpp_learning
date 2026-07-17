# 01-hello-world

## Concept
The first program in any language: print a line of text and exit. It proves
your toolchain works and introduces the three things every C/C++ program has —
an `#include`, a `main` function, and a return value.

## Minimal example
See `src/main.cpp`. The C version of this lesson lives in [`apps/c/01-hello-world`](../../c/01-hello-world/) (run it with `make run-c-app-01-hello-world`).

## Line-by-line
- `#include <iostream>` — pulls in the declarations for `std::cout`. Without it
  the compiler doesn't know what `std::cout` is.
- `int main()` — the entry point. Execution starts here. It returns an `int`.
- `std::cout << "Hello, World!\n";` — send text to standard output. `<<` is the
  "stream insertion" operator; read it as "put this into cout".
- `return 0;` — hand `0` back to the operating system, meaning success. Any
  non-zero value conventionally means an error.

## Common pitfalls
- Forgetting the `#include` → "`cout` was not declared".
- Forgetting the `;` at the end of a statement.
- Missing newline → your shell prompt ends up glued to the output.

## C vs C++
| | C | C++ |
|---|---|---|
| Header | `<stdio.h>` | `<iostream>` |
| Output | `printf("...\n");` | `std::cout << "...\n";` |
| `main` | `int main(void)` | `int main()` |

C uses *format-string* functions (`printf`); C++ uses *streams* (`std::cout`).
In C, `int main(void)` explicitly says "no parameters"; C++ treats empty `()`
the same way. In both, returning `0` from `main` is success (and in both you may
even omit the `return` — the standard supplies an implicit `return 0;`).

## Build & run
```sh
make run-cpp-app-01-hello-world      # C++
make run-c-app-01-hello-world    # C
```

## Try it yourself
Print a second line with your name. In C++ add another `std::cout <<` line; in C
add another `printf`.
