# cpp_learning

A tiny, vendor-free C and C++ workspace for learning the languages from your
very first program to advanced modern C++ — one small, runnable lesson at a
time.

- A single top-level `Makefile` builds and runs every lesson — no per-lesson Makefiles.
- C++ lessons live under `apps/cpp/NN-name/`; C lessons under `apps/c/NN-name/`.
- No submodules, no external build systems, no dependencies.

## How to use this repo

Each numbered folder under `apps/cpp/` is one self-contained C++ lesson. Read
its `README.md`, then build and run it:

```sh
make run app=01-hello-world
```

Lessons are ordered so each builds on the last. Where the C-vs-C++ contrast
teaches something, there's a parallel C lesson in `apps/c/NN-name/` — run it
with `run-c` (same name, C tree):

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

## Practice — follow along in your own copy

Don't edit the reference lessons. To *type along* with a tutorial, make a
personal scratch copy and build it with `PRACTICE=1`:

```sh
make practice app=01-hello-world          # -> practice/cpp/01-hello-world/ (a starter file)
make run      app=01-hello-world PRACTICE=1   # build & run YOUR copy

make practice app=05-io-basics lang=c     # -> practice/c/05-io-basics/
make run-c    app=05-io-basics PRACTICE=1
```

Your copies live under `practice/` (git-ignored, so they never clutter the
repo), and `make practice` never overwrites a file you've already started. Read
the reference lesson's README, write your own version in your practice copy, and
run it to check. The reference solution stays put for when you want to compare.

## Learning path

> Legend: **C** = also has a C version under `apps/c/NN-name/` · **H** = multi-file (`include/` + `src/`).

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

### Renderer variants — one renderer, four layouts (53–56)
The **same** tinyrenderer-style software renderer (framebuffer → line → triangle
→ z-buffer → OBJ mesh → texture → perspective → camera → Flat/Gouraud/Phong
shaders → tangent-space normal mapping), written four ways so the **file layout
itself is the lesson**. The shared theory is in
[`references/tinyrenderer-pipeline.md`](references/tinyrenderer-pipeline.md).

| # | Lesson | |
|---|--------|---|
| 53 | renderer-hpp — C++, `include/` + `src/` (the reference implementation) | H |
| 54 | renderer-flat — C++, one file; byte-identical output to 53 | H |
| 55 | renderer-h — C port, `.h` + `.c` (named funcs, `malloc`/`free`, function-pointer shaders) | C |
| 56 | renderer-flat — C, one file; byte-identical output to 55 | C |

Run any of them on a real model from [`assets/tinyrenderer/`](assets/tinyrenderer/):
```sh
make run app=53-renderer-hpp ARGS="assets/tinyrenderer/african_head/african_head.obj normal assets/tinyrenderer/african_head/african_head_diffuse.ppm assets/tinyrenderer/african_head/african_head_nm_tangent.ppm"
```

Each row above is a folder under `apps/cpp/` (e.g. `apps/cpp/01-hello-world/`)
with its own README; the **C** lessons also have a counterpart under `apps/c/`.
See also the reference guides:
- [`references/tinyrenderer-pipeline.md`](references/tinyrenderer-pipeline.md) — the renderer pipeline theory shared by lessons 53–56.
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

- Source lives in `apps/cpp/NN-name/src/` (and `apps/c/NN-name/src/` for C);
  multi-file lessons add an `include/` folder.
- Binaries land centrally under `build/<cpp|c>/NN-name/<config>/bin/`, where
  `<config>` encodes the build type and standard (e.g. `Debug-c++23-c17`) so
  different standards never collide.
- Override the compiler with `CXX=clang++` / `CC=clang` on the make command line.
- Minimal by design — focus on C/C++ concepts, not tooling.
