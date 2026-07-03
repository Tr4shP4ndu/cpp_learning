# C/C++ Learning Curriculum — Design

Date: 2026-07-03
Status: Approved (curriculum + approach)

## Goal

Turn this C++-only workspace into a self-contained learning path that takes a
reader who does not know C or C++ from first program to advanced modern C++ and
a real multi-file project. Every lesson is standalone, builds and runs with one
`make` command, and explains the concept in plain English. C appears wherever
the C-vs-C++ contrast actually teaches something (memory, strings, structs,
I/O, files); it is not duplicated where it would add nothing.

The path ends with a **capstone**: a small software renderer, built from
scratch with no dependencies, that teaches the GPU rasterization pipeline on
the CPU, vector/matrix math, file I/O, and senior-dev project structure.

## Non-goals

- Not a build-system showcase. `make` stays minimal and vendor-free (no CMake).
- No external dependencies, package managers, or git submodules.
- No real-GPU SDK code (CUDA/Vulkan/OpenGL) — it breaks the vendor-free design.
  GPU is taught as *concepts on the CPU*, with a reference doc pointing to real
  GPU paths as "where to go next."
- `00-testing` stays as-is, outside the numbered sequence.

## Mechanics

### Dual-language lessons
A lesson folder may contain both `src/main.cpp` and `src/main.c`. Each compiles
into its own binary so two `main()` symbols never collide:
- `build/<app>/bin/<app>`   ← from `main.cpp`
- `build/<app>/bin/<app>-c` ← from `main.c`

`make run app=<app>` runs the C++ binary; `make run-c app=<app>` runs the C
binary. If a lesson has only `.cpp`, `run-c` is a no-op with a clear message.

### Multi-file projects (headers)
Once headers are taught (lesson 30), lessons and the capstone use real
structure:
```
<app>/
  include/   *.hpp / *.h   (declarations)
  src/       *.cpp / *.c   (definitions, incl. main)
```
The per-app Makefile:
- globs all `src/*.cpp` (and `src/*.c`) — already links multiple TUs into one binary,
- adds `-Iinclude -Isrc` so headers resolve,
- rebuilds objects when their headers change (header dependency tracking via `-MMD -MP`).

Lessons before 30 stay single-file (`src/main.cpp`) to avoid distraction.

### Standard switching (the "old vs new" showcase)
Both the top-level and per-app Makefiles accept:
- `STD=c++98|c++11|c++14|c++17|c++20|c++23` (default `c++23`)
- `CSTD=c99|c11|c17` (default `c17`)

So a learner can run `make run app=21-strings STD=c++98` and compare behavior
against the default. READMEs call out where standard choice changes behavior.

### Warnings and safety
- Always: `-Wall -Wextra -Wpedantic`.
- Debug (default): `-O0 -g -fsanitize=address,undefined`.
- Release: `-O2` (or `-O3`), no sanitizers.

### Naming fix
`09-std::arrays` → `19-std-array`. A `::` in a filesystem path is a hazard and
breaks on some tooling; rename during the renumber.

## Standard lesson README shape

Every lesson README follows the same sections (scaled to the topic):

1. **Concept** — plain-English, why it exists, when to use it.
2. **Minimal example** — the smallest program that shows it.
3. **Line-by-line** — annotate the non-obvious lines.
4. **Common pitfalls** — the mistakes beginners actually hit.
5. **C vs C++** — only on lessons where it teaches something.
6. **Build & run** — the exact `make` commands, including `STD=` variants where relevant.
7. **Try it yourself** — one small exercise.

## Curriculum (renumbered single sequence)

`C` = ships a C counterpart. `✚` = new lesson. `H` = multi-file (`include/`+`src/`).

### Beginner
| #  | Lesson | New | C |
|----|--------|-----|---|
| 01 | hello-world | | C |
| 02 | comments | | |
| 03 | variables-datatypes | | C |
| 04 | constants | | |
| 05 | io-basics | ✚ | C |
| 06 | operators | | |
| 07 | conditionals | ✚ | |
| 08 | switch | ✚ | |
| 09 | scope | | |
| 10 | namespaces | | |
| 11 | typedef-aliases | | |

### Core (control flow & data)
| #  | Lesson | New | C |
|----|--------|-----|---|
| 12 | for-loop | | |
| 13 | range-for-loop | | |
| 14 | while-loop | | |
| 15 | do-while-loop | | |
| 16 | break-statement | | |
| 17 | continue-statement | | |
| 18 | arrays | | C |
| 19 | std-array | | |
| 20 | std-fill | | |
| 21 | strings | ✚ | C |
| 22 | vectors | ✚ | |
| 23 | containers-map-set | ✚ | |
| 24 | files-and-filesystem | ✚ | C |

### Functions
| #  | Lesson | New | C |
|----|--------|-----|---|
| 25 | functions | | |
| 26 | void-functions | | |
| 27 | functions-with-parameters | | |
| 28 | function-overloading | | |
| 29 | recursive-function | | |

### Project structure & pipeline
| #  | Lesson | New | Notes |
|----|--------|-----|-------|
| 30 | header-source-separation | ✚ | H — .hpp/.cpp, include guards, `#pragma once`, decl vs def |
| 31 | build-pipeline | ✚ | preprocess→compile→assemble→link, translation units, object files, `-I` filepaths |

### Memory & semantics
| #  | Lesson | New | C |
|----|--------|-----|---|
| 32 | value-semantics | | |
| 33 | references | | |
| 34 | pointers | ✚ | C |
| 35 | dereferencing | | |
| 36 | dynamic-memory | ✚ | C |
| 37 | smart-pointers | ✚ | |

### Object-oriented programming (multi-file from here)
| #  | Lesson | New | Notes |
|----|--------|-----|-------|
| 38 | structs | ✚ | C — C struct ↔ C++ struct |
| 39 | enums | ✚ | |
| 40 | classes-basics | ✚ | H |
| 41 | constructors-destructors | ✚ | H — RAII |
| 42 | encapsulation | ✚ | H |
| 43 | inheritance | ✚ | H |
| 44 | polymorphism | ✚ | H — virtual |
| 45 | operator-overloading | ✚ | H |

### Advanced (modern C++)
| #  | Lesson | New | Notes |
|----|--------|-----|-------|
| 46 | templates | ✚ | H |
| 47 | concepts | ✚ | C++20 |
| 48 | move-semantics | ✚ | |
| 49 | lambdas | ✚ | |
| 50 | ranges-and-algorithms | ✚ | |
| 51 | error-handling | ✚ | exceptions, `std::optional`, `std::expected` (C++23) |

### Capstone
| #  | Lesson | New | Notes |
|----|--------|-----|-------|
| 52 | mini-renderer | ✚ | H — full `include/`+`src/` project |

### Renumber map (existing → new)

| existing | new |
|----------|-----|
| 01-hello-world | 01-hello-world |
| 02-comments | 02-comments |
| 03-variables-datatypes | 03-variables-datatypes |
| 04-constants | 04-constants |
| 22-operators | 06-operators |
| 05-scope | 09-scope |
| 06-namespaces | 10-namespaces |
| 07-typedef-aliases | 11-typedef-aliases |
| 10-for-loop | 12-for-loop |
| 11-range-for-loop | 13-range-for-loop |
| 12-while-loop | 14-while-loop |
| 13-do-while-loop | 15-do-while-loop |
| 15-break-statement | 16-break-statement |
| 16-continue-statement | 17-continue-statement |
| 08-arrays | 18-arrays |
| 09-std::arrays | 19-std-array |
| 14-std::fill | 20-std-fill |
| 17-functions | 25-functions |
| 18-void-functions | 26-void-functions |
| 19-functions-with-parameters | 27-functions-with-parameters |
| 20-function-overloading | 28-function-overloading |
| 21-recursive-function | 29-recursive-function |
| 23-value-semantics | 32-value-semantics |
| 24-references | 33-references |
| 25-dereferencing | 35-dereferencing |

New lessons (no existing source): 05, 07, 08, 21, 22, 23, 24, 30, 31, 34, 36,
37, 38–52.

## Capstone: mini-renderer (lesson 52)

A dependency-free software renderer, own arc inspired by tinyrenderer (linked as
"go deeper"). Real project layout:
```
52-mini-renderer/
  include/  vec.hpp  mat.hpp  image.hpp  model.hpp
  src/      image.cpp  model.cpp  main.cpp
  assets/   (a tiny .obj + notes; or generated procedurally to avoid binaries)
```
Milestones, explained stage by stage in the README:
1. Write a color image to disk (PPM/TGA) — proves file I/O + the "framebuffer" idea.
2. `vec2/vec3` math header — the data the pipeline moves.
3. Draw a line (Bresenham) — first thing on screen.
4. Wireframe from a simple model — geometry → screen.
5. Triangle rasterizer (barycentric fill).
6. Z-buffer — hidden-surface removal (the core GPU idea).
7. Flat/Gouraud shading — lighting.

Stretch (README notes, optional): OBJ loader, texture mapping, perspective.
Keep assets tiny or procedural so no large binaries enter the repo.

## Reference docs (not numbered lessons)

Under a top-level `references/` (or `docs/`):
- `debugging-and-tooling.md` — gdb/lldb, the sanitizers, valgrind, Compiler
  Explorer (godbolt), `clang-tidy`, `clang-format`.
- `going-further-gpu.md` — how real GPUs run the pipeline; pointers to shaders
  (GLSL), CUDA, Vulkan/OpenGL, and the ssloy/tinyrenderer course.

## Root README + top-level docs

- Root `README.md` gains a **learning path** table (the curriculum above) with
  each lesson linked, a "how to use this repo" section, and the `STD=`/`run-c`
  usage. Keep the existing quick-start.
- `Guide-to-Install-C-CPP.md` gets a short check that a C compiler is available
  too (`cc`/`gcc`/`clang`).

## Templates

`templates/app/` updates so scaffolded apps match the new lesson shape:
- `main.cpp.tpl` — minimal, with a one-line concept comment.
- `README.md.tpl` — the standard 7-section shape with placeholders.
- `Makefile.tpl` — the new per-app Makefile (STD switch, C support, `-Iinclude
  -Isrc`, header dep tracking, sanitizers).
- `main.c.tpl` — for scaffolding C counterparts (`make app app=x lang=c`).

## Testing / verification

Example code, not a library — "tests" means: every lesson compiles clean
(`-Wall -Wextra -Wpedantic`) under the default standard and runs without
sanitizer errors. Per-phase verification: `make build` (all) and spot-run. A
lesson with non-trivial logic keeps an `assert`-based sanity check in `main`
where it makes sense (recursion, operators, the renderer's math header).

## Implementation phasing

Each phase ends with a full `make build` and a commit, so review happens at
tier boundaries.

1. **Foundation** — new per-app + top-level Makefiles (STD/C/sanitizers/`-I`/dep
   tracking), templates, root README learning-path scaffold, `references/` stubs.
   Renumber existing folders (`git mv`) incl. the `::` fix. Verify all builds.
2. **Beginner** — deepen 01–11 to the standard shape; add 05, 07, 08; add C parts (01, 03, 05).
3. **Core** — 12–24; add strings (21, +C), vectors (22), containers (23), files (24, +C).
4. **Functions** — 25–29.
5. **Project structure & pipeline** — 30 header-source-separation (multi-file), 31 build-pipeline.
6. **Memory** — 32–37; add pointers (+C), dynamic-memory (+C), smart-pointers.
7. **OOP** — 38–45 (multi-file); add structs (+C) through operator-overloading.
8. **Advanced** — 46–51; templates, concepts, move, lambdas, ranges, error-handling.
9. **Capstone + references** — 52 mini-renderer; `debugging-and-tooling.md`, `going-further-gpu.md`.
