# cpp_learning

A tiny, vendor-free C and C++ workspace for learning the languages from your
very first program to advanced modern C++ — one small, runnable lesson at a
time.

- Single top-level `Makefile` to scaffold apps and build/run them.
- Each lesson lives under `apps/NN-name/` with its own `Makefile` and `src/`.
- No submodules, no external build systems, no dependencies.

## How to use this repo

Each numbered folder under `apps/` is one self-contained lesson. Read its
`README.md`, then build and run it:

```sh
make run app=01-hello-world
```

Lessons are ordered so each builds on the last. Where the C-vs-C++ contrast
teaches something, a lesson ships a C version too — run it with:

```sh
make run-c app=01-hello-world
```

You can compile any lesson under an **older** standard to see how the language
has changed:

```sh
make run app=21-strings STD=c++98    # C++98 .. C++23 (default c++23)
make run-c app=30-pointers CSTD=c99  # c99 | c11 | c17 (default c17)
```

Debug builds (the default) run with AddressSanitizer + UBSan on, so memory and
undefined-behavior mistakes are caught while you learn. Use
`BUILD_TYPE=Release` for optimized, sanitizer-free builds.

## Learning path

> Legend: **C** = also has a C version · **H** = multi-file (`include/` + `src/`).
> Folders with a gap in the numbering are lessons still to be written.

### Beginner
| # | Lesson | |
|---|--------|---|
| 01 | hello-world | C |
| 02 | comments | |
| 03 | variables-datatypes | C |
| 04 | constants | |
| 05 | io-basics | C |
| 06 | operators | |
| 07 | conditionals | |
| 08 | switch | |
| 09 | scope | |
| 10 | namespaces | |
| 11 | typedef-aliases | |

### Core — control flow & data
| # | Lesson | |
|---|--------|---|
| 12 | for-loop | |
| 13 | range-for-loop | |
| 14 | while-loop | |
| 15 | do-while-loop | |
| 16 | break-statement | |
| 17 | continue-statement | |
| 18 | arrays | C |
| 19 | std-array | |
| 20 | std-fill | |
| 21 | strings | C |
| 22 | vectors | |
| 23 | containers-map-set | |
| 24 | files-and-filesystem | C |

### Functions
| # | Lesson |
|---|--------|
| 25 | functions |
| 26 | void-functions |
| 27 | functions-with-parameters |
| 28 | function-overloading |
| 29 | recursive-function |

### Project structure & the build pipeline
| # | Lesson | |
|---|--------|---|
| 30 | header-source-separation | H |
| 31 | build-pipeline | |

### Memory & semantics
| # | Lesson | |
|---|--------|---|
| 32 | value-semantics | |
| 33 | references | |
| 34 | pointers | C |
| 35 | dereferencing | |
| 36 | dynamic-memory | C |
| 37 | smart-pointers | |

### Object-oriented programming
| # | Lesson | |
|---|--------|---|
| 38 | structs | C |
| 39 | enums | |
| 40 | classes-basics | H |
| 41 | constructors-destructors | H |
| 42 | encapsulation | H |
| 43 | inheritance | H |
| 44 | polymorphism | H |
| 45 | operator-overloading | H |

### Advanced — modern C++
| # | Lesson |
|---|--------|
| 46 | templates |
| 47 | concepts |
| 48 | move-semantics |
| 49 | lambdas |
| 50 | ranges-and-algorithms |
| 51 | error-handling |

### Capstone
| # | Lesson | |
|---|--------|---|
| 52 | mini-renderer — a from-scratch software renderer (the GPU pipeline on the CPU) | H |

Each row above is a folder under `apps/` (e.g. `apps/01-hello-world/`) with its
own README. See also the reference guides:
- [`references/debugging-and-tooling.md`](references/debugging-and-tooling.md) — debuggers, sanitizers, valgrind, Compiler Explorer, clang-tidy.
- [`references/going-further-gpu.md`](references/going-further-gpu.md) — how the capstone maps to real GPUs, and where to learn shaders/CUDA/Vulkan.

## Quick start

- List lessons — `make list`
- Scaffold a new C++ lesson — `make app app=my-new-app`
- Scaffold a C lesson — `make app app=my-new-app lang=c`
- Build one (or all if `app=` omitted) — `make build app=my-new-app`
- Run — `make run app=my-new-app` (C++) / `make run-c app=my-new-app` (C)
- Clean build artifacts — `make clean`
- Delete a lesson — `make delete-app app=my-new-app`
- `make help` prints every target and variable.

## Requirements

- macOS: Xcode Command Line Tools (`xcode-select --install`) — gives `clang`/`clang++`.
- Linux: `gcc`/`g++` and `clang`/`clang++`.
- Windows: MSVC, MinGW, or Clang.

See `Guide-to-Install-C-CPP.md` for detailed setup.

## Notes

- Source lives in `apps/NN-name/src/`; multi-file lessons add `apps/NN-name/include/`.
- Binaries land centrally under `build/NN-name/<config>/bin/`, where `<config>`
  encodes the build type and standard (e.g. `Debug-c++23-c17`) so different
  standards never collide.
- Override the compiler with `CXX=clang++` / `CC=clang` on the make command line.
- Minimal by design — focus on C/C++ concepts, not tooling.
