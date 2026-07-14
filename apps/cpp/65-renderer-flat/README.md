# 63-renderer-flat

## Concept
The **exact same renderer as [62-renderer-hpp](../62-renderer-hpp/)** — every
pipeline stage, byte-for-byte the same output — but living in **one
translation unit**: a single `src/main.cpp`, no headers. Same code, opposite
layout. That contrast *is* the lesson.

The theory (all the math) is in
[`references/tinyrenderer-pipeline.md`](../../../references/tinyrenderer-pipeline.md).
Read 53's README first for the stage-by-stage build-up; this page is only about
what changes when you collapse that project into one file.

## Layout contrast: one file vs `.hpp` + `.cpp`
53 splits the renderer into `geometry/image/model/gl/shader` headers and
sources. 54 pastes them all into `main.cpp`, in dependency order, with:

- `#pragma once` and every `#include "..."` (the cross-file wiring) **removed**;
- all standard-library `#include <...>` lines **hoisted and deduplicated** into
  one block at the top;
- declarations first (geometry → image → model → shader → gl), then all
  definitions — so, e.g., `IShader` is fully defined before `triangle()` uses it.

### What one TU costs
- **Every edit recompiles everything.** Change one line of the rasterizer and
  the whole file rebuilds — there are no separate object files to keep. 53
  recompiles only the `.cpp` you touched, then relinks.
- **No reuse.** Nothing here can be `#include`d by another program; it's one
  sealed blob. 53's headers are a reusable interface.
- **One giant scope.** Every name is visible to every other part; nothing is
  encapsulated behind a header. Naming discipline is on you.
- **Harder to navigate** — ~1100 lines in one file vs five focused ones.

### What it saves
- **Nothing to wire up.** No headers, no include guards, no include-order
  puzzles, no Makefile listing multiple sources.
- **Trivially shareable.** The whole renderer is one file you can drop into a
  compiler explorer, a gist, or a forum post. That's genuinely useful for a
  self-contained demo — which is exactly why this layout exists.

The takeaway: the split in 53 buys incremental builds, encapsulation, and reuse
at the cost of wiring; the single file here buys portability and zero setup at
the cost of all of the above. Neither is "right" — the project's size and how
it's shared decide it.

## Project layout
```
63-renderer-flat/
  src/main.cpp     # the entire renderer — geometry, image, model, gl, shaders, main
  README.md
```

## Build & run
```sh
make run app=63-renderer-flat
```
Writes `render.ppm` here. It takes the **same command-line arguments as 53**
(`[model] [shader] [diffuse.ppm] [normalmap.ppm]`) and the same
[`assets/tinyrenderer/`](../../../assets/tinyrenderer/) test models:
```sh
make run app=63-renderer-flat ARGS="assets/tinyrenderer/african_head/african_head.obj normal assets/tinyrenderer/african_head/african_head_diffuse.ppm assets/tinyrenderer/african_head/african_head_nm_tangent.ppm"
```
For identical arguments its `render.ppm` is **byte-identical** to 53's (verify
with `cmp`) — it is the same renderer, only the file layout differs.

## Attribution
Inspired by [ssloy/tinyrenderer](https://github.com/ssloy/tinyrenderer) and the
walkthrough at [haqr.eu/tinyrenderer](https://haqr.eu/tinyrenderer/). The prose
here is original; the algorithms are the classic ones they teach.
