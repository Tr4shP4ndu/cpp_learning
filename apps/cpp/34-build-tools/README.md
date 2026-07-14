# 34-build-tools

## Concept
A build turns source into a binary through a compiler (here `clang++`/`g++`)
driven by **flags**: the language standard (`-std=c++23`), warnings
(`-Wall -Wextra`), optimization (`-O0` vs `-O2`), debug info (`-g`), and defines
(`-DNDEBUG`). A build tool like `make` just runs those commands for you. This
program reads a few compiler-set macros to *see* how it was built.

## Minimal example
See `src/main.cpp`.

## Line-by-line
- `__cplusplus` — an integer the compiler defines from `-std`. `202302` means
  C++23, `201703` means C++17, and so on.
- `__clang__` / `__GNUC__` — defined by clang and gcc respectively, so the same
  source can identify its compiler and print the version macros.
- `NDEBUG` — conventionally defined by release builds (`-DNDEBUG`). It also turns
  off `assert()` (see lesson 36). Its presence is a proxy for "release build".

## Why this matters
This repo is built by one top-level `Makefile` with no per-lesson build files.
Understanding the flags behind `make run` — standard, warnings, sanitizers —
lets you read any project's build and change how yours compiles.

## Common pitfalls
- Different `-std` values genuinely change what compiles: try `STD=c++14` on a
  lesson that uses `std::span` and it won't build. That's the point of the flag.
- `-O0` (this repo's default) keeps debugging easy but is slow; production uses
  `-O2`/`-O3`. Never benchmark an `-O0` build.
- `-Wall` does **not** mean "all warnings"; `-Wall -Wextra -Wpedantic` (what this
  repo uses) catches far more.

## Build & run
```sh
make run app=34-build-tools
make run app=34-build-tools STD=c++17   # watch __cplusplus change to 201703
```

## Try it yourself
Run it under two standards and diff the output. Then add a block that prints
`__DATE__` and `__TIME__` — the timestamp baked in when the file was compiled.
