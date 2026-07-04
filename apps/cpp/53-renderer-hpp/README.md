# 53-renderer-hpp

## Concept
A full software renderer — the same **graphics pipeline a GPU runs** (project →
rasterize → depth-test → shade), on the CPU, with zero dependencies — built the
way a real C++ project is: an `include/` of headers (the *interface*) and a
`src/` of sources (the *implementation*), compiled as separate translation units
and joined by the linker.

This is the **reference** implementation of one renderer expressed in four file
layouts. The others are the *same code, different organization*:

| Lesson | Language | Layout |
|--------|----------|--------|
| **53-renderer-hpp** (this) | C++ | `include/` + `src/` — headers & sources |
| 54-renderer-flat | C++ | one `main.cpp`, no headers |
| 55-renderer-h | C | `.h` + `.c` |
| 56-renderer-flat | C | one `main.c` |

The theory (all the math) lives once in
[`references/tinyrenderer-pipeline.md`](../../../references/tinyrenderer-pipeline.md).
This README is about *this layout* and how to build the renderer up stage by
stage.

## Layout contrast: `.hpp` vs no-`.hpp`
The lesson of **53** is the split itself. Declarations go in headers; definitions
go in sources; consumers `#include` the header and never see the `.cpp`.

- **`geometry.hpp`** is header-only (templates + small `inline` functions must be
  visible wherever they're used — see lesson 30's note on the ODR).
- **`image` / `model` / `gl` / `shader`** each have a `.hpp` (what exists) and a
  `.cpp` (how it works). `main.cpp` includes only the headers.

What the split buys you (contrast 54, which pastes everything into one file):
- **Incremental builds** — edit `shader.cpp` and only it recompiles, then relink.
  In 54, every edit recompiles the whole renderer.
- **Encapsulation** — a header is a contract; `Model`'s private vectors are
  invisible to `main`. In 54 everything is in one scope.
- **Navigability & reuse** — five focused files instead of one 1000-line wall.

The cost is the wiring: headers, include guards (`#pragma once`), and getting the
`#include` order right. 54 pays none of that and can be shared as a single file —
which is exactly *its* lesson.

## Project layout
```
53-renderer-hpp/
  include/
    geometry.hpp   # Vec<N,T>, Matrix, dot/cross/normalize/embed/proj3, operators
    image.hpp      # Color, Image (RGB framebuffer), PPM read/write
    model.hpp      # Model: procedural cube/sphere + OBJ loader; diffuse + normal-map sampling
    gl.hpp         # viewport/projection/lookAt; drawLine; barycentric; triangle() rasterizer
    shader.hpp     # IShader + Flat/Gouraud/Phong/NormalMap shaders
  src/
    image.cpp  model.cpp  gl.cpp  shader.cpp
    main.cpp       # selfCheck(); build scene; pick shader; render; write render.ppm
  README.md
```
The repo's central Makefile compiles every `src/*.cpp` and links them — there are
no per-app Makefiles.

## The pipeline, stage by stage
The full theory (framebuffer/PPM, Bresenham, barycentric, z-buffer, the
model→world→view→clip→screen matrix chain, UV interpolation, TBN normal mapping)
is in [`references/tinyrenderer-pipeline.md`](../../../references/tinyrenderer-pipeline.md).
Read it alongside the code — it's shared by all four renderer lessons.

## Build it up (ten stages)
The finished files were built one stage at a time; that's how to *read* them too.
Each stage compiles, runs, and shows visible progress. Run after each:
```sh
make run app=53-renderer-hpp     # writes render.ppm in this folder
```

0. **Framebuffer + PPM** — `image.{hpp,cpp}`; `main` fills a square and saves it.
   *An image is just an array of pixels you can write to disk.*
1. **Vector & matrix math** — `geometry.hpp`; `Vec`, `Matrix`, `dot/cross/…`.
   No visible change; it's the data every stage moves.
2. **Line → wireframe** — `drawLine` (Bresenham); draw a triangle outline.
3. **Triangle + barycentric + flat shading** — bounding-box scan, inside test,
   fill a shaded triangle.
4. **Z-buffer** — one depth per pixel; two overlapping triangles, the near one
   correctly hides the far one.
5. **Model** — procedural cube/sphere + OBJ loader; render a mesh.
6. **Texture mapping** — interpolate UV, sample `diffuse()` (checkerboard by
   default).
7. **Perspective + camera** — `viewport`, `projection`, `lookAt`; the mesh shows
   foreshortening from a chosen camera.
8. **Shaders** — the `IShader` split; pick Flat / Gouraud / Phong.
9. **Tangent-space normal mapping** — `NormalMapShader` + a TBN basis; surface
   detail from a normal map, independent of geometry.

Each numerically-checkable stage adds `assert`s to `selfCheck()` (run at the top
of `main`, Debug build) — vector ops, barycentric weights summing to 1, matrix
identities, normal-map decode. Break one and the program aborts; that's the
"failing test" for a renderer whose real output is visual.

## Build & run
```sh
make run app=53-renderer-hpp
```
Writes `render.ppm` here. View it in a PPM-capable viewer, or convert:
```sh
magick render.ppm render.png        # ImageMagick
# or: pnmtopng render.ppm > render.png
```
Default: the procedural cube, checkerboard diffuse, `NormalMapShader` (which, with
no normal map, matches Phong). The cube has one normal per face, so Flat/Gouraud/
Phong look identical on it — swap in `Model::sphere(...)` to see them differ.

### Optional command-line arguments
```sh
./53-renderer-hpp [diffuse.ppm] [shader] [normal.ppm]
```
- **`argv[1]`** — a **P6 PPM** diffuse texture (else the checkerboard). Pass `-`
  to keep the checkerboard while still choosing a shader.
- **`argv[2]`** — `flat` | `gouraud` | `phong` | `normal` (default `normal`).
- **`argv[3]`** — a **P6 PPM** tangent-space normal map (else a flat map).

Only **PPM (P6)** is read in-tree — no image libraries. To use a downloaded
`.tga`/`.png` texture, convert it first (not committed to the repo):
```sh
magick texture.png texture.ppm      # ImageMagick handles the P6 conversion
```
Run it through the Makefile-built binary, e.g.
`make run app=53-renderer-hpp` builds it; the binary lands under
`build/apps/cpp/53-renderer-hpp/<config>/bin/` and takes the args above.

## Going further
- **Perspective-correct texturing** — interpolate `U/w` and `1/w` (see the caveat
  in the reference doc).
- **Specular** (Blinn-Phong), **shadow mapping**, **ambient occlusion** — the
  next shaders in the original tinyrenderer course.
- **Where this maps to a real GPU** — your `fragment` is a fragment shader,
  `vertex` a vertex shader, the z-buffer is hardware:
  [`references/going-further-gpu.md`](../../../references/going-further-gpu.md).

## Attribution
Inspired by [ssloy/tinyrenderer](https://github.com/ssloy/tinyrenderer) and the
walkthrough at [haqr.eu/tinyrenderer](https://haqr.eu/tinyrenderer/). The prose
here is original; the algorithms are the classic ones they teach.
