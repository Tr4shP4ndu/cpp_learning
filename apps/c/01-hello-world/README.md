# 01-hello-world (C)

The C version of [`apps/cpp/01-hello-world`](../../cpp/01-hello-world/). Same
goal — print a line and exit — the C way.

## Concept
Every C program has an `#include`, a `main`, and a return value. Output goes
through `printf` (from `<stdio.h>`), not streams.

## Line-by-line
- `#include <stdio.h>` — declares `printf`.
- `int main(void)` — entry point; `void` explicitly says "no parameters".
- `printf("Hello, World!\n");` — write formatted text to standard output.
- `return 0;` — success.

## Compared to C++
C uses `printf` (a format-string function) where C++ uses `std::cout` (a
stream). See the [C++ version](../../cpp/01-hello-world/) for the contrast.

## Build & run
```sh
make run-c app=01-hello-world
```
