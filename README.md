# cpp_learning

A tiny, vendor-free C and C++ workspace for learning the languages from your
very first program to advanced modern C++ — one small, runnable lesson at a
time.

- Every app has its own two-line `Makefile` (`APP := name` + an include of the
  shared `rules.mk`), so any app builds on its own: `cd apps/cpp/calc && make run`.
- A top-level `Makefile` is a convenience launcher — `make run app=<name>` finds
  the app and runs its Makefile for you. See [MAKEFILE.md](MAKEFILE.md) for a
  step-by-step guide.
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
| 63 | renderer — a from-scratch tinyrenderer-style software renderer (the GPU pipeline on the CPU) | H |

The capstone ties the course together: it runs the **same pipeline a GPU does**
(framebuffer → line → triangle → z-buffer → OBJ mesh → texture → perspective →
camera → Flat/Gouraud/Phong shaders → tangent-space normal mapping), on the CPU
with zero dependencies. The illustrated theory — with diagrams — is in
[`apps/cpp/63-renderer/PIPELINE.md`](apps/cpp/63-renderer/PIPELINE.md).

Run it on a real model from [`assets/tinyrenderer/`](assets/tinyrenderer/):
```sh
make run app=63-renderer ARGS="assets/tinyrenderer/african_head/african_head.obj normal assets/tinyrenderer/african_head/african_head_diffuse.ppm assets/tinyrenderer/african_head/african_head_nm_tangent.ppm"
```

Each row above is a folder under `apps/cpp/` (e.g. `apps/cpp/01-hello-world/`)
with its own README; the **C** lessons also have a counterpart under `apps/c/`.

## Quick start

- Set up a compiler for this repo — `make install`
- List apps — `make list` (C++) / `make list-c` (C)
- Scaffold a new C++ app — `make app app=my-new-app`
- Scaffold a C app — `make app app=my-new-app lang=c`
- …inside a named group folder — `make app app=my-new-app group=graphics`
- Build one (or all if `app=` omitted) — `make build app=my-new-app`
- Run — `make run app=my-new-app` (C++) / `make run-c app=my-new-app` (C)
- Clean build artifacts — `make clean`
- Delete an app — `make delete-app app=my-new-app`
- `make help` prints every target and variable.

## Your own apps & groups

Everything lives under `apps/cpp/` (C++) or `apps/c/` (C) and builds with the
same rules — there's no separate "project" kind. An app is just a folder with a
`src/`. It can sit directly under a language, or inside a **named group folder**
you make to keep related apps together:

```sh
make app app=my-thing lang=cpp                 # apps/cpp/my-thing
make app app=my-thing lang=cpp group=graphics  # apps/cpp/graphics/my-thing
make run app=my-thing                          # found by name at any depth
```

Apps are resolved by name, so `run` / `build` / `delete-app` are the same
wherever an app lives — groups are pure organisation, with no bookkeeping. Each
scaffolded app gets a two-line `Makefile` that includes the shared `rules.mk`;
you can also build it directly with `cd apps/cpp/my-thing && make run`.

**Outgrew the defaults?** Replace that two-line `Makefile` with a real one of
your own — as long as it has `build` and `run` targets, the launcher still finds
it by name and calls it. That app can then hold whatever structure and build
steps you want (`include/`, `assets/`, extra libraries, …). Start simple; take
over the build only when you need to. Full walkthrough: [MAKEFILE.md](MAKEFILE.md).

The repo already ships worked examples you can run and read — small C++ apps
(`guessing-game`, `cli-todo`, `calc`, `mandelbrot`), from-scratch graphics
(`tinyraytracer`, `tinyraycaster`, `tinykaboom`), and a tiny POSIX shell at
`apps/c/shell`. Run one with `make run app=<name>` (or `make run-c app=shell`).

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
- Each app builds into its own git-ignored `build/` folder
  (`apps/cpp/NN-name/build/NN-name`). The shared recipe lives in `rules.mk`; see
  [MAKEFILE.md](MAKEFILE.md).
- Compiler: `make install` puts one in `./toolchain/` (git-ignored) and the
  workspace prefers it; override anytime with `CXX=clang++` / `CC=clang` on the
  make command line.
- Minimal by design — focus on C/C++ concepts, not tooling.
