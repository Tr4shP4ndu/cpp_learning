# tinyrenderer as four file-organization variants — design

**Date:** 2026-07-04
**Status:** approved (design), pending implementation plan

## Summary

Build one tinyrenderer-style software renderer tutorial, expressed four ways.
The renderer content is identical across all four apps — only the file
organization differs, and that difference *is* the lesson. Each app teaches the
same graphics pipeline while contrasting a way to structure C or C++ code.

Inspired by [ssloy/tinyrenderer](https://github.com/ssloy/tinyrenderer) and the
lesson notes at [haqr.eu/tinyrenderer](https://haqr.eu/tinyrenderer/). Code and
prose are original; the tutorials are attributed and linked, not copied.

## The four apps

| App | Language | Layout | Contrast it teaches |
|---|---|---|---|
| `apps/cpp/53-renderer-hpp` | C++ | `include/*.hpp` + `src/*.cpp` + `main.cpp`, multi-TU | interface vs implementation, ODR, `#pragma once`, classes / operator overloading / RAII, an `IShader` abstraction |
| `apps/cpp/54-renderer-flat` | C++ | single `main.cpp`, no headers | what the header / multi-TU split *buys* you — felt by its absence |
| `apps/c/55-renderer-h` | C | `.h` (structs + declarations) + `.c` | C's declaration/definition split; `vec3_add(a,b)` instead of `a+b`; manual `malloc`/`free` |
| `apps/c/56-renderer-flat` | C | single `.c`, `static` functions | one translation unit in C |

- **53 is the reference implementation.** 54, 55, 56 are derived from it.
- **No code is shared between apps.** The duplication is deliberate — you cannot
  contrast layouts if the apps share files. Each app is fully self-contained.
- **The central Makefile is unchanged.** It already handles `include/`+`src/`,
  single-file layouts, and the `-c` tree (`make run app=53-renderer-hpp`,
  `make run-c app=55-renderer-h`, etc.). No per-app Makefiles.

## The tutorial arc

Every README is a "build it up" walkthrough. Each stage compiles and renders a
visible result, so the learner understands each piece before adding the next.

0. **Framebuffer + PPM save** — an image is just an array of pixels.
1. **Bresenham line → wireframe** — first geometry; the integer line algorithm.
2. **Triangle rasterizer** — bounding box + barycentric coordinates; inside test;
   back-face culling; flat shading (brightness = normal · light).
3. **Z-buffer** — one depth per pixel; hidden-surface removal.
4. **Geometry input** — a procedural mesh (cube/sphere) that always works, **plus
   an optional `loadObj()`** parsing `v` / `vt` / `vn` / `f` lines. No model file
   is committed; the loader is ready for a model the learner downloads.
5. **Texture mapping** — UV coordinates interpolated across the triangle with the
   same barycentric weights (`U = α₀U₀ + α₁U₁ + α₂U₂`); per-fragment diffuse
   lookup. Procedural checkerboard by default; can load a PPM texture. *(This is
   the haqr.eu "textures" lesson the request pointed at.)*
6. **Perspective projection** — the projection matrix and the w-divide.
7. **Camera** — `lookAt` (view) + viewport matrices; model → world → view → clip
   → screen.
8. **Shaders** — Gouraud, then Phong-style per-fragment shading; the
   vertex → fragment abstraction (`IShader` in C++, function pointers / a
   dispatch struct in C).
9. **Tangent-space normal mapping** — the TBN matrix; sampling a normal map to
   perturb per-fragment normals. Uses a procedural normal map by default; can
   load a PPM normal map.

**Deliberately skipped**, listed under "going further" in each README: shadow
mapping, ambient occlusion, specular / glow maps.

## Shared technical decisions

- **Output format: PPM (P6)** — same as the rest of the repo and lesson 52; zero
  new dependencies.
- **Textures / normal maps: procedural by default.** A checkerboard diffuse and a
  flat/procedural normal map render out of the box. A PPM image can be loaded
  instead. READMEs document converting a downloaded `.tga`/`.png` → `.ppm` with a
  standard tool (e.g. ImageMagick), so no image codec is vendored.
- **Assets: none committed.** Procedural geometry and textures keep the repo
  vendor-free. `loadObj()` and PPM texture loading are documented for learners who
  want to point the renderer at a real downloaded model (e.g. the tinyrenderer
  African head).
- **Math primitives.** `Vec2` / `Vec3` / `Vec4` and a 4×4 matrix.
  - C++ (53, 54): templated `Vec<N,T>` with operator overloading; `Matrix`
    with `operator*`.
  - C (55, 56): explicit structs (`Vec3f`, `Mat4`) and named functions
    (`vec3_add`, `mat4_mul`, ...). This contrast — operators vs named calls — is
    itself a teaching point.
- **The shader abstraction.**
  - C++: an `IShader` interface (`vertex()` / `fragment()`), so swapping shaders
    is polymorphism.
  - C: a struct of function pointers, or an explicit `enum`+`switch` dispatch —
    showing how C expresses the same idea without virtual functions.
- **No shared code across apps.** Repeated on purpose (see above).
- **Theory written once.** A new `references/tinyrenderer-pipeline.md` explains
  the math and pipeline in depth (barycentric coordinates, z-buffer, projection
  and viewport matrices, perspective, UV interpolation, the TBN matrix). The four
  READMEs link it instead of repeating the deep theory four times; each README
  focuses on its file layout and the stage-by-stage walkthrough. Cross-link the
  existing `references/going-further-gpu.md`.

## Per-app README shape

Each README follows the repo's established lesson shape, plus:

1. **Concept** — what this app renders and the pipeline it runs.
2. **Layout contrast** — the section unique to each variant: `.hpp` vs no-`.hpp`
   (C++) or `.h` vs no-`.h` (C). What the split buys (declaration/definition
   separation, ODR, translation units, faster incremental builds, reuse) versus
   single-file simplicity. This is the reason four apps exist.
3. **Project layout** — the file tree and what each file owns.
4. **The pipeline, stage by stage** — links `references/tinyrenderer-pipeline.md`
   for the math.
5. **Build it up** — the numbered stage walkthrough (arc above), each stage a
   `make run`-able checkpoint.
6. **Going further** — skipped stages, links to ssloy/tinyrenderer and haqr.eu.
7. **Attribution.**

## Verification

- The rendered `.ppm` is the primary, visible check at every stage.
- Each app includes a small `assert`-based self-check on the math primitives
  (e.g. `cross`, `normalize`, barycentric coordinates, `mat4_mul` identity) so a
  broken primitive fails loudly rather than producing a subtly wrong image.
- All four apps must build warning-clean (`-Wall -Wextra -Wpedantic`) under the
  repo's default standards (C++23, C17) and run under AddressSanitizer +
  UndefinedBehaviorSanitizer (the Debug default).

## Implementation phasing (for the plan)

Full course × four apps is large. The plan should:

1. Build **53-renderer-hpp** end-to-end through all ten stages, and write
   `references/tinyrenderer-pipeline.md` alongside it. Verify it renders and is
   warning/sanitizer-clean.
2. **Pause for review** before replicating — so the design is validated once, not
   four times.
3. Derive **54-renderer-flat** (flatten 53 into one C++ TU).
4. Port to **55-renderer-h** (C with `.h`/`.c`).
5. Flatten to **56-renderer-flat** (single C file).
6. Update the root `README.md` learning path and the C/C++ tree READMEs to list
   53–56 and cross-link them.

## Non-goals

- No new build-system machinery, submodules, or third-party dependencies.
- No committed model or texture assets.
- No image formats beyond PPM in-tree (conversion documented instead).
- No shadow mapping, ambient occlusion, or specular/glow maps (listed as "going
  further").
