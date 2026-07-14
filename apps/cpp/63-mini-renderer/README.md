# 61-mini-renderer (capstone)

## Concept
A software renderer: it turns 3D triangles into a 2D image, running the same
**graphics pipeline a GPU runs** — project → shade → rasterize → depth-test —
entirely on the CPU, with zero dependencies. This capstone ties together
almost everything in the course: headers/sources, classes, RAII, `std::vector`,
operator overloading, file I/O, and a real multi-file project structure.

Inspired by [ssloy/tinyrenderer](https://github.com/ssloy/tinyrenderer); see
`references/going-further-gpu.md` for where to go next (real GPUs, shaders).

## Project layout
```
61-mini-renderer/
  include/
    vec.hpp      # Vec3f + dot/cross/normalize — the math the pipeline moves
    image.hpp    # Image: an RGB framebuffer that saves itself as PPM
    render.hpp   # drawLine + z-buffered drawTriangle
  src/
    image.cpp    # framebuffer + PPM writer
    render.cpp   # Bresenham line, barycentric triangle rasterizer + z-buffer
    main.cpp     # builds a scene, renders it, writes render.ppm
```
Each `.cpp` is a separate translation unit; the Makefile compiles and links them
(lesson 32). Headers expose the interface (lesson 31).

## The pipeline, stage by stage
1. **Framebuffer** (`Image`) — a `std::vector<Color>`; `writePPM` saves it. Proves
   the "image is just an array of pixels" idea.
2. **Vector math** (`vec.hpp`) — `Vec3f`, `dot`, `cross`, `normalize`; the data
   every stage passes around.
3. **Projection** (`toScreen` in `main.cpp`) — map 3D model coordinates to pixel
   coordinates (orthographic; Y flipped so up is up).
4. **Lines** (`drawLine`) — Bresenham's integer algorithm; the wireframe.
5. **Rasterization** (`drawTriangle`) — for each pixel in the triangle's bounding
   box, compute **barycentric coordinates** to test "inside?" and to interpolate.
6. **Z-buffer** — one depth per pixel; a fragment is drawn only if it's closer
   than what's already there. This is how the green triangle correctly hides the
   red one where they overlap — hidden-surface removal, the core GPU idea.
7. **Shading** (`shade`) — flat shading: brightness = how much the triangle's
   normal faces the light.

## Build it step by step
Don't type the whole thing at once. Build it in stages — each one compiles,
runs, and shows visible progress, so you understand every piece before adding
the next. Run `make run app=61-mini-renderer` after each stage.

1. **A colored image on disk.** Write only `image.hpp` + `image.cpp` (the
   `Image` class and `writePPM`) and a `main` that makes a 400×400 image, fills
   a few pixels with `img.set(...)`, and calls `writePPM("render.ppm")`. Open it.
   *Goal: "an image is just an array of pixels I can save."*
2. **Vectors.** Add `vec.hpp` (`Vec3f`, `dot`, `cross`, `normalize`). Nothing
   visible yet — it's the data the next steps use.
3. **A line.** Add `drawLine` (get the simple case working, then handle steep
   lines and direction). Draw one line corner-to-corner. *Goal: first geometry.*
4. **A wireframe.** Add `toScreen` (projection) in `main`, define three points,
   and draw the triangle's three edges with `drawLine`. *Goal: 3D → screen.*
5. **A filled triangle.** Add `drawTriangle` with the bounding-box + barycentric
   test **but skip the z-buffer for now** — just fill inside-pixels with a color.
   *Goal: rasterization.*
6. **The z-buffer.** Add the `zbuffer` vector in `main`, pass it in, and add the
   `z > zbuffer[idx]` depth test to `drawTriangle`. Draw two overlapping
   triangles at different `z`. *Goal: the closer one correctly hides the other.*
7. **Shading.** Add `shade()` (normal · light) and color each triangle by it.
   *Goal: lighting.*

Each stage is a small, testable change — exactly how the finished files here
were built.

## Build & run
```sh
make run app=61-mini-renderer
```
It writes `render.ppm` in this folder. PPM is the simplest image format to write
by hand. To view it, open it in an image viewer that supports PPM (GIMP,
IrfanView, many editors) or convert it:
```sh
magick render.ppm render.png     # ImageMagick
# or: pnmtopng render.ppm > render.png
```
You should see a red and a green triangle (green in front) with a white
wireframe triangle on top.

## Ideas to extend it (stretch)
- **Model loading:** parse a Wavefront `.obj` (vertices `v` + faces `f`) and draw
  its wireframe, then fill it.
- **Texturing:** interpolate texture coordinates with the same barycentric
  weights and sample an image.
- **Perspective:** replace the orthographic projection with a perspective divide.
- **Gouraud shading:** shade per-vertex and interpolate the intensity.

## Where this maps to a real GPU
Your `drawTriangle` inner loop is a *fragment shader*; `toScreen` is a *vertex
shader*; the z-buffer is hardware on a GPU. See
`references/going-further-gpu.md`.
