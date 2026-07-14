# cpp_learning

A tiny, vendor-free C and C++ workspace for learning the languages from your
very first program to advanced modern C++ — one small, runnable lesson at a
time.

- A single top-level `Makefile` builds and runs every lesson — no per-lesson
  Makefile needed (though a lesson or project can add one when it outgrows the
  default; see below).
- C++ lessons live under `apps/cpp/NN-name/`; C lessons under `apps/c/NN-name/`.
- No submodules, no external build systems, no dependencies.
- Portable: `make install` sets up a compiler *for this repo* (into a git-ignored
  `./toolchain/`), so you can build on a fresh machine without a system-wide install.

## How to use this repo

First time on a machine, get a compiler (see
[Installing a compiler](#installing-a-compiler) below for details):

```sh
make install   # Linux/Windows: fetch clang into ./toolchain · macOS: verify Xcode CLT
```

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
make run app=20-strings STD=c++98    # C++98 .. C++23 (default c++23)
make run-c app=39-pointers CSTD=c99  # c99 | c11 | c17 (default c17)
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
| 19 | std-array (with std::iota / std::fill) | |
| 20 | strings | C |
| 21 | vectors | |
| 22 | containers-map-set | |
| 23 | files-and-filesystem | C |
| 24 | string-view-span | |
| 25 | iterators | |

### Functions
| # | Lesson |
|---|--------|
| 26 | functions |
| 27 | void-functions |
| 28 | functions-with-parameters |
| 29 | function-overloading |
| 30 | recursive-function |

### Project structure, build & tooling
| # | Lesson | |
|---|--------|---|
| 31 | header-source-separation | H |
| 32 | build-pipeline | |
| 33 | preprocessor-macros | |
| 34 | build-tools | |
| 35 | testing | |
| 36 | debugging | |

### Memory & semantics
| # | Lesson | |
|---|--------|---|
| 37 | value-semantics | |
| 38 | references | |
| 39 | pointers (address-of, dereferencing) | C |
| 40 | dynamic-memory | C |
| 41 | smart-pointers | |
| 42 | static-storage | |

### Object-oriented programming
| # | Lesson | |
|---|--------|---|
| 43 | structs | C |
| 44 | enums | |
| 45 | classes-basics | H |
| 46 | constructors-destructors | H |
| 47 | encapsulation | H |
| 48 | const-correctness | |
| 49 | inheritance | H |
| 50 | polymorphism | H |
| 51 | operator-overloading | H |

### Advanced — modern C++
| # | Lesson |
|---|--------|
| 52 | casting |
| 53 | constexpr |
| 54 | templates |
| 55 | concepts |
| 56 | move-semantics |
| 57 | lambdas |
| 58 | ranges-and-algorithms |
| 59 | variant-tuple |
| 60 | format |
| 61 | error-handling |
| 62 | concurrency |

### Capstone
| # | Lesson | |
|---|--------|---|
| 63 | mini-renderer — a from-scratch software renderer (the GPU pipeline on the CPU) | H |

### Renderer variants — one renderer, four layouts (64–67)
The **same** tinyrenderer-style software renderer (framebuffer → line → triangle
→ z-buffer → OBJ mesh → texture → perspective → camera → Flat/Gouraud/Phong
shaders → tangent-space normal mapping), written four ways so the **file layout
itself is the lesson** (lessons 64–67). The shared theory is in
[`references/tinyrenderer-pipeline.md`](references/tinyrenderer-pipeline.md).

| # | Lesson | |
|---|--------|---|
| 64 | renderer-hpp — C++, `include/` + `src/` (the reference implementation) | H |
| 65 | renderer-flat — C++, one file; byte-identical output to 64 | H |
| 66 | renderer-h — C port, `.h` + `.c` (named funcs, `malloc`/`free`, function-pointer shaders) | C |
| 67 | renderer-flat — C, one file; byte-identical output to 66 | C |

Run any of them on a real model from [`assets/tinyrenderer/`](assets/tinyrenderer/):
```sh
make run app=64-renderer-hpp ARGS="assets/tinyrenderer/african_head/african_head.obj normal assets/tinyrenderer/african_head/african_head_diffuse.ppm assets/tinyrenderer/african_head/african_head_nm_tangent.ppm"
```

Each row above is a folder under `apps/cpp/` (e.g. `apps/cpp/01-hello-world/`)
with its own README; the **C** lessons also have a counterpart under `apps/c/`.
See also the reference guides:
- [`references/tinyrenderer-pipeline.md`](references/tinyrenderer-pipeline.md) — the renderer pipeline theory shared by lessons 64–67.
- [`references/debugging-and-tooling.md`](references/debugging-and-tooling.md) — debuggers, sanitizers, valgrind, Compiler Explorer, clang-tidy.
- [`references/going-further-gpu.md`](references/going-further-gpu.md) — how the capstone maps to real GPUs, and where to learn shaders/CUDA/Vulkan.

## Quick start

- Set up a compiler for this repo — `make install`
- List lessons — `make list`
- Scaffold a new C++ lesson — `make app app=my-new-app`
- Scaffold a C lesson — `make app app=my-new-app lang=c`
- Build one (or all if `app=` omitted) — `make build app=my-new-app`
- Run — `make run app=my-new-app` (C++) / `make run-c app=my-new-app` (C)
- Clean build artifacts — `make clean`
- Delete a lesson — `make delete-app app=my-new-app`
- `make help` prints every target and variable.

## Your own projects

`apps/c` and `apps/cpp` are the deliberately-uniform lesson trees. For your
**own** work, make a sibling folder under `apps/` with **any name and any
structure you want** — its only job is to bring its own `Makefile`:

```sh
make new-project name=my-thing     # scaffold apps/my-thing (own Makefile + src/)
make project     name=my-thing     # build & run it
make list-projects
```

`apps/my-thing/` can hold as many folders (`src/`, `include/`, `assets/`, …) as
you like; the workspace hands its build off to your `Makefile` (recursive make),
passing `CXX`/`CC`/`STD`/`CSTD`/`BUILD_TYPE` down (and `ARGS` for `run`).

The same escape hatch works for a **lesson**: drop a `Makefile` into
`apps/<tree>/<name>/` and the workspace delegates `build`/`run` to it instead of
the default rules. Minimum by default; your own build when you need it.

The repo already ships worked examples you can run and read — small apps
(`guessing-game`, `cli-todo`, `calc`, `mandelbrot`) and from-scratch graphics
(`tinyraytracer`, `tinyraycaster`, `tinykaboom`). List them with
`make list-projects`, run one with `make project name=<name>`.

## Installing a compiler

You need a C/C++ compiler and `make`. Easiest is to let the repo set one up:

```sh
make install
```

- **Linux / Windows:** nothing pre-installed — `make install` downloads a
  prebuilt LLVM/Clang into a git-ignored `./toolchain/` (no admin, never
  pushed), and the workspace prefers it automatically. On Windows, run under
  **MSYS2 / Git Bash** (that's where `make` lives). Pin a version with
  `make install LLVM_VERSION=20.1.0`.
- **macOS:** the compiler uses Apple's SDK, so `make install` just verifies the
  **Xcode Command Line Tools** (`xcode-select --install`) — that one part can't
  live in the repo.

Prefer a system-wide compiler instead? Any of these work; the workspace uses
whatever `c++`/`cc` resolve to, and an explicit `CXX=`/`CC=` always wins:

- **macOS:** `xcode-select --install` (or `brew install llvm`).
- **Linux:** `sudo apt install build-essential clang make` (or your distro's equivalent).
- **Windows:** MSYS2 (`pacman -S make mingw-w64-ucrt-x86_64-clang`) or WSL2.

Verify with `make run app=01-hello-world` → `Hello, World!`.

`make` and a POSIX shell are assumed (present by default on macOS/Linux).

## Notes

- Source lives in `apps/cpp/NN-name/src/` (and `apps/c/NN-name/src/` for C);
  multi-file lessons add an `include/` folder.
- Binaries land centrally under `build/<cpp|c>/NN-name/<config>/bin/`, where
  `<config>` encodes the build type and standard (e.g. `Debug-c++23-c17`) so
  different standards never collide.
- Compiler: `make install` puts one in `./toolchain/` (git-ignored) and the
  workspace prefers it; override anytime with `CXX=clang++` / `CC=clang` on the
  make command line.
- Minimal by design — focus on C/C++ concepts, not tooling.
