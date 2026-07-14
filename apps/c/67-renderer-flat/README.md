# 65-renderer-flat

## Concept
The **same renderer as [64-renderer-h](../64-renderer-h/)** — the whole C
pipeline — collapsed into **one `main.c`**. Same code, one file. It is to 55
what [63-renderer-flat](../../cpp/63-renderer-flat/) is to 53: the flatten is the
lesson.

55's README covers the C port and the C-vs-C++ contrasts. This page is only about
collapsing the `.h`/`.c` split into a single translation unit.

## Layout contrast: one C file vs `.h` + `.c`
55 splits the renderer across `geometry/image/model/gl/shader` headers and
sources. 56 concatenates them into `main.c`, in dependency order, with:

- the `#ifndef X_H / #define X_H / #endif` **include guards removed** (nothing is
  included twice — there's only one file);
- every `#include "..."` (the cross-file wiring) **removed**;
- the standard-library `#include <...>` lines **hoisted and deduplicated** at the
  top;
- declarations first (geometry → image → model → shader → gl), then all
  definitions, so e.g. `struct Shader` is complete before `gl_triangle` uses it.

### What one TU costs
- **Every edit recompiles the whole file** — no per-`.o` incremental builds.
- **No reuse** — nothing here can be `#include`d by another C program.
- **One namespace** — every function and type is visible everywhere. C has no
  namespaces, so in a real multi-file C project the header split *is* your only
  boundary; collapsing it removes even that. (File-local helpers can at least be
  marked `static` to keep them internal — as they are here.)
- **~1000 lines in one file** to navigate.

### What it saves
- **Zero wiring** — no headers, no include guards, no linker juggling multiple
  objects, no build list.
- **One portable file** — drop it into a compiler, a gist, or an email. For a
  small self-contained C program that's a real convenience — the reason this
  layout exists.

## Project layout
```
65-renderer-flat/
  src/main.c     # the entire renderer in one C translation unit
  README.md
```

## Build & run
```sh
make run-c app=65-renderer-flat
```
Writes `render.ppm` here. Takes the **same arguments as 55**
(`[model] [shader] [diffuse.ppm] [normalmap.ppm]`) and the same
[`assets/tinyrenderer/`](../../../assets/tinyrenderer/) models:
```sh
make run-c app=65-renderer-flat ARGS="assets/tinyrenderer/diablo3_pose/diablo3_pose.obj phong assets/tinyrenderer/diablo3_pose/diablo3_pose_diffuse.ppm"
```
For identical arguments its `render.ppm` is **byte-identical** to 55's (`cmp`) —
same renderer, one file.

## Attribution
Inspired by [ssloy/tinyrenderer](https://github.com/ssloy/tinyrenderer) and the
walkthrough at [haqr.eu/tinyrenderer](https://haqr.eu/tinyrenderer/). The prose
here is original; the algorithms are the classic ones they teach.
