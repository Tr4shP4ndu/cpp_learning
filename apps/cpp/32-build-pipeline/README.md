# 32-build-pipeline

## Concept
`make run` hides four distinct steps that turn your source into a running
program: **preprocess → compile → assemble → link**. Understanding them
demystifies the Makefile, explains linker errors, and shows why we split code
into headers and sources.

## The four stages
```
main.cpp ──preprocess──▶ main.i ──compile──▶ main.s ──assemble──▶ main.o ─┐
                                                                          ├─link─▶ 32-build-pipeline
adder.cpp ─preprocess──▶ adder.i ─compile──▶ adder.s ─assemble─▶ adder.o ─┘
```

1. **Preprocess** — handles the `#` directives: pastes in `#include`d headers,
   expands macros, strips comments. Output is still C++, just expanded.
2. **Compile** — translates that C++ into assembly for your CPU.
3. **Assemble** — turns assembly into machine code in an *object file* (`.o`).
4. **Link** — stitches all object files (and libraries) into one executable,
   resolving each call to its definition.

Each `.cpp` is compiled **independently** into its own `.o` — that unit is a
*translation unit*. The linker is what connects `main.o`'s call to
`sum_of_squares` with the definition in `adder.o`.

## See each stage yourself
Run these from **inside this folder** (`apps/32-build-pipeline`):
```sh
# 1. Preprocess only (-E): see headers pasted in. Huge output — page it.
c++ -Iinclude -E src/main.cpp | less

# 2. Compile to assembly (-S): produces main.s
c++ -std=c++23 -Iinclude -S src/main.cpp -o main.s

# 3. Assemble to an object file (-c): produces main.o (not runnable yet)
c++ -std=c++23 -Iinclude -c src/main.cpp -o main.o
c++ -std=c++23 -Iinclude -c src/adder.cpp -o adder.o

# 4. Link the objects into the final executable
c++ main.o adder.o -o demo && ./demo

# clean up
rm -f main.s main.o adder.o demo
```

## Why the split matters
- Change `adder.cpp` and only *it* recompiles; `main.o` is reused. That's why
  large projects build fast.
- The `-I` flag tells the preprocessor **where to search** for headers — that's
  how `#include "adder.hpp"` finds `include/adder.hpp`. Paths in, filepaths
  resolved.

## Reading the two error families
- **Compile error** — a single file is malformed (syntax, type mismatch). Named
  with a file and line.
- **Linker error** — files compiled fine, but the linker can't find a
  definition ("undefined reference to `sum_of_squares`") or found two
  ("duplicate symbol"). This is the ODR (lesson 31) showing up at link time.

## Build & run
```sh
make run-cpp-app-32-build-pipeline   # the Makefile runs all four stages for you
```

## Try it yourself
Delete the body of `sum_of_squares` in `adder.cpp` (keep the declaration) and
build. You'll get an "undefined reference" — a **linker** error, not a compile
error. Put it back to fix it.
