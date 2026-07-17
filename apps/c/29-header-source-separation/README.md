# 29-header-source-separation (C)

Real C programs split across files: a header (`.h`) *declares* what exists, and
a source file (`.c`) *defines* it. Each `.c` compiles independently, then the
linker stitches them together.

## Line-by-line
- `include/greet.h` — declares `void greet(const char *name);` behind an include
  guard (`#ifndef GREET_H / #define GREET_H / #endif`).
- `src/greet.c` — `#include "greet.h"` then supplies the function body (the definition).
- `src/main.c` — `#include "greet.h"` to see the declaration, then calls `greet()`.
- The Makefile compiles `src/*.c` with `-Iinclude` so `"greet.h"` is found.

## Common pitfalls
- Putting a function *definition* in a header: include it from two `.c` files and
  the linker complains about duplicate symbols. Headers declare; `.c` files define.
- Omitting the include guard: including a header twice re-declares its contents.
- Forgetting to compile/link every `.c` file gives an "undefined reference" at link time.
- The declaration and definition signatures must match exactly.

## Compared to C++
The mechanism is the same. C++ often adds `#pragma once` as a shorter guard and
leans on headers more heavily for templates and inline code. See the
[C++ version](../../cpp/31-header-source-separation/).

## Build & run
```sh
make run-c-app-29-header-source-separation
```
