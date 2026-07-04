# tinyrenderer Four-Variant Lessons (53–56) Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Build one tinyrenderer-style software renderer as a step-by-step tutorial, expressed in four file-organization variants — `apps/cpp/53-renderer-hpp` (C++ headers+sources), `apps/cpp/54-renderer-flat` (single C++ file), `apps/c/55-renderer-h` (C headers+sources), `apps/c/56-renderer-flat` (single C file) — where the file layout itself is the lesson.

**Architecture:** Build **53** end-to-end through ten pipeline stages (framebuffer → line → triangle → z-buffer → geometry/OBJ → texture → perspective → camera → shaders → normal mapping), writing the shared theory doc alongside it. Then **pause for review**, then mechanically derive 54 (flatten to one TU), 55 (port to C with `.h`/`.c`), and 56 (flatten the C). No code is shared between apps — the duplication is the teaching point. Everything renders to PPM; geometry and textures are procedural by default with an optional `loadObj()` + PPM texture path.

**Tech Stack:** C++23 / C17, the repo's central Makefile (no per-app Makefiles, no CMake), no third-party dependencies, PPM (P6) output.

## Global Constraints

- **No new dependencies, submodules, or build machinery.** The central `Makefile` is unchanged; it already builds `include/`+`src/`, single-file, and the `-c` tree.
- **No committed model or texture assets.** Procedural geometry/textures render out of the box; `loadObj()` + PPM loading are for models the learner downloads.
- **Warning-clean** under `-Wall -Wextra -Wpedantic` at the repo defaults (C++23, C17).
- **Sanitizer-clean** under AddressSanitizer + UndefinedBehaviorSanitizer (the Debug build default). No leaks in the C variants (`malloc` is paired with `free`).
- **PPM (P6) only** in-tree. Converting a downloaded `.tga`/`.png` → `.ppm` is documented, never vendored.
- **Attribution:** every app README and the reference doc link [ssloy/tinyrenderer](https://github.com/ssloy/tinyrenderer) and [haqr.eu/tinyrenderer](https://haqr.eu/tinyrenderer/). Prose is original.
- **No `using namespace std;` in headers.** C++ uses smart pointers over raw `new`/`delete`, `static_cast` over C-casts, const-correct signatures.
- **Build/run commands:** `make run app=53-renderer-hpp` and `make run app=54-renderer-flat` (C++); `make run-c app=55-renderer-h` and `make run-c app=56-renderer-flat` (C). Binaries land under `build/apps/<tree>/<name>/<SIG>/bin/<name>` and each writes `render.ppm` into the cwd.

## Verification model for a renderer

Pure "write a failing unit test first" does not fit visual output. Each task uses this loop instead, and it is mandatory:

1. **`assert` self-check first** for anything numerically checkable (vector ops, matrix identity/multiply, barycentric weights summing to 1, in/out triangle tests). These asserts live in the app (a `selfCheck()` called at the top of `main`, compiled in Debug) and MUST fail before the implementation exists and pass after.
2. **Build warning-clean:** the exact `make` command, expected to finish with no warnings.
3. **Render + inspect:** run it, confirm `render.ppm` is written and describe the expected visible result (e.g. "a white wireframe triangle on black"). Open with any PPM viewer or convert: `magick render.ppm render.png`.

A task is not done until (1) asserts pass, (2) the build is warning/sanitizer-clean, and (3) the render matches the description.

## File structure

### `apps/cpp/53-renderer-hpp` (reference implementation)
```
include/
  geometry.hpp   # Vec<N,T> + Vec2f/3f/4f, Matrix (4x4), math ops (operators)
  image.hpp      # Color, Image (RGB framebuffer), PPM read/write
  model.hpp      # Model: procedural cube/sphere + loadObj; per-face vert/uv/normal access; diffuse+normal sampling
  gl.hpp         # viewport/projection/lookAt matrices; drawLine; triangle() rasterizer (barycentric + z-buffer + shader)
  shader.hpp     # IShader interface + Flat/Gouraud/Phong/NormalMap shaders
src/
  image.cpp
  model.cpp
  gl.cpp
  shader.cpp
  main.cpp       # selfCheck(); build scene; pick shader; render; write render.ppm
README.md
```

### `apps/cpp/54-renderer-flat`
```
src/main.cpp     # every stage above, one translation unit, no headers
README.md
```

### `apps/c/55-renderer-h`
```
include/
  geometry.h  image.h  model.h  gl.h  shader.h
src/
  geometry.c  image.c  model.c  gl.c  shader.c  main.c
README.md
```

### `apps/c/56-renderer-flat`
```
src/main.c       # every stage above, one translation unit, static functions
README.md
```

### Shared docs
```
references/tinyrenderer-pipeline.md   # the math/theory, linked by all four READMEs (new)
README.md, apps/cpp/README.md, apps/c/README.md   # add 53–56 to the learning path
```

## Canonical C++ interfaces (agreed signatures — later tasks depend on these)

These are the exact names/types every C++ task in Phase 1 relies on. The C variants mirror them with named functions (Phase 3).

```cpp
// geometry.hpp
template <int N, typename T> struct Vec { T data[N]{}; T& operator[](int i); T operator[](int i) const; };
using Vec2f = Vec<2,float>; using Vec3f = Vec<3,float>; using Vec4f = Vec<4,float>;
// free operators: + - , scalar * , Vec3f cross(a,b), float dot(a,b), Vec3f normalize(v), float norm(v)
// Vec4f embed(Vec3f, float w=1);  Vec3f proj3(Vec4f);   // homogeneous helpers
struct Matrix { float m[4][4]{}; static Matrix identity(); };
Matrix operator*(const Matrix&, const Matrix&);
Vec4f  operator*(const Matrix&, const Vec4f&);

// image.hpp
struct Color { unsigned char r,g,b; };                       // 24-bit RGB
class Image {
public:
  Image(int w, int h);
  int width() const; int height() const;
  void set(int x, int y, Color c);
  Color get(int x, int y) const;                             // clamped; used for texture sampling
  void writePPM(const std::string& path) const;
  static Image readPPM(const std::string& path);             // P6; throws on failure
};

// model.hpp
class Model {
public:
  static Model cube();                                        // procedural, always works
  static Model sphere(int lat, int lon);
  static Model loadObj(const std::string& path);              // parses v/vt/vn/f
  int nfaces() const;
  Vec3f vert(int face, int nth) const;                        // model-space position
  Vec2f uv(int face, int nth) const;                          // texture coord (0..1)
  Vec3f normal(int face, int nth) const;                      // vertex normal
  void setDiffuse(const Image& tex);                          // optional; else checkerboard
  void setNormalMap(const Image& nm);                         // optional; else flat map
  Color diffuse(Vec2f uv) const;                              // samples texture or checkerboard
  Vec3f normalMap(Vec2f uv) const;                            // samples map or (0,0,1)
};

// gl.hpp
Matrix viewport(int x, int y, int w, int h);                  // NDC -> screen, depth 0..255
Matrix projection(float coeff);                               // coeff = -1/camera_z (0 = orthographic)
Matrix lookAt(Vec3f eye, Vec3f center, Vec3f up);
void drawLine(int x0,int y0,int x1,int y1, Image& img, Color c);   // Bresenham
struct IShader;                                                // fwd; defined in shader.hpp
void triangle(const Vec4f clip[3], IShader& shader, Image& img, std::vector<float>& zbuf);

// shader.hpp
struct IShader {
  virtual ~IShader() = default;
  virtual Vec4f vertex(int face, int nth) = 0;                // returns clip-space vertex; stashes varyings
  virtual bool  fragment(Vec3f bary, Color& out) = 0;         // return true to DISCARD the fragment
};
// concrete: FlatShader, GouraudShader, PhongShader, NormalMapShader (ctors take Model&, uniforms)
```

---

# Phase 1 — Build `53-renderer-hpp` end to end

Each task adds one pipeline stage to `apps/cpp/53-renderer-hpp` and ends with a warning-clean render. Scaffold the app once in Task 1.1.

### Task 1.1: Scaffold + framebuffer + PPM (stage 0)

**Files:**
- Create: `apps/cpp/53-renderer-hpp/include/image.hpp`
- Create: `apps/cpp/53-renderer-hpp/src/image.cpp`
- Create: `apps/cpp/53-renderer-hpp/src/main.cpp`

**Interfaces:**
- Produces: `Color`, `Image` (see canonical interfaces).

- [ ] **Step 1: Create the app directory** — `mkdir -p apps/cpp/53-renderer-hpp/include apps/cpp/53-renderer-hpp/src`

- [ ] **Step 2: Write `include/image.hpp`**

```cpp
#pragma once
#include <string>

struct Color { unsigned char r{}, g{}, b{}; };

class Image {
public:
    Image(int w, int h);
    int width()  const { return w_; }
    int height() const { return h_; }
    void  set(int x, int y, Color c);
    Color get(int x, int y) const;                 // clamps to edge (safe for sampling)
    void  writePPM(const std::string& path) const;
    static Image readPPM(const std::string& path); // P6 only; throws std::runtime_error
private:
    int w_, h_;
    std::vector<Color> pixels_;                     // row-major, origin top-left
};
```
Add `#include <vector>` above.

- [ ] **Step 3: Write `src/image.cpp`** — constructor allocates `w*h` black pixels; `set` bounds-checks and stores; `get` clamps `x,y` into range then returns; `writePPM` writes `P6\n<w> <h>\n255\n` then raw RGB bytes (open file with `std::ios::binary`, throw `std::runtime_error` if it won't open); `readPPM` parses the P6 header (skip the magic, width, height, maxval, one whitespace byte) then reads `w*h*3` bytes. Use `static_cast`, no C-casts.

- [ ] **Step 4: Write `src/main.cpp` with a self-check**

```cpp
#include "image.hpp"
#include <cassert>

static void selfCheck() {
    Image t(4, 4);
    t.set(1, 2, Color{10, 20, 30});
    Color c = t.get(1, 2);
    assert(c.r == 10 && c.g == 20 && c.b == 30);
    assert(t.get(-5, -5).r == 0);                  // clamp returns edge pixel (black here)
}

int main() {
    selfCheck();
    const int W = 400, H = 400;
    Image img(W, H);
    for (int x = 50; x < 150; ++x)
        for (int y = 50; y < 150; ++y)
            img.set(x, y, Color{255, 128, 0});     // an orange square
    img.writePPM("render.ppm");
    return 0;
}
```

- [ ] **Step 5: Build warning-clean** — `make run app=53-renderer-hpp`
  Expected: compiles with no warnings, runs, and (because `selfCheck` passes) writes `render.ppm`. Temporarily break a self-check assert and confirm it aborts, then restore it.

- [ ] **Step 6: Inspect** — open `render.ppm` (or `magick render.ppm out.png`). Expected: an orange square near the top-left of a 400×400 black image.

- [ ] **Step 7: Commit** — `git add apps/cpp/53-renderer-hpp && git commit -m "feat(53): framebuffer + PPM output (stage 0)"`

### Task 1.2: Vector & matrix math (`geometry.hpp`)

**Files:**
- Create: `apps/cpp/53-renderer-hpp/include/geometry.hpp`
- Modify: `apps/cpp/53-renderer-hpp/src/main.cpp` (extend `selfCheck`)

**Interfaces:**
- Produces: `Vec2f/3f/4f`, `Matrix`, `dot`, `cross`, `normalize`, `norm`, `embed`, `proj3`, `operator*` (matrix·matrix, matrix·vec4). Consumed by every later task.

- [ ] **Step 1: Write `include/geometry.hpp`** — header-only. A small template `template<int N, typename T> struct Vec { T data[N]{}; T& operator[](int); T operator[](int) const; };`, aliases `Vec2f/3f/4f`, free `operator+`, `operator-`, scalar `operator*` (both orders), `dot`, `cross` (Vec3f), `norm` (= `std::sqrt(dot(v,v))`), `normalize`, `embed(Vec3f,float w=1)→Vec4f`, `proj3(Vec4f)→Vec3f` (divides by w). `struct Matrix { float m[4][4]{}; static Matrix identity(); };` with `operator*` for matrix·matrix and matrix·Vec4f. Keep it under ~120 lines.

- [ ] **Step 2: Extend `selfCheck()` in `main.cpp`** (write these asserts, run, watch them FAIL to compile/link until geometry exists — that is the failing-test step):

```cpp
#include "geometry.hpp"
// inside selfCheck():
Vec3f a{1,0,0}, b{0,1,0};
assert(dot(a, b) == 0.0f);
Vec3f c = cross(a, b);                       // z = +1
assert(c[0]==0 && c[1]==0 && c[2]==1);
assert(std::abs(norm(normalize(Vec3f{3,4,0})) - 1.0f) < 1e-6f);
Matrix I = Matrix::identity();
Vec4f v = I * Vec4f{2,3,4,1};
assert(v[0]==2 && v[1]==3 && v[2]==4 && v[3]==1);
```
Add `#include <cmath>`.

- [ ] **Step 3: Run to verify the new asserts pass** — `make run app=53-renderer-hpp`. Expected: builds clean, runs, `render.ppm` still the orange square.

- [ ] **Step 4: Commit** — `git commit -am "feat(53): vector + 4x4 matrix math (geometry.hpp)"`

### Task 1.3: Bresenham line → wireframe triangle (stage 1)

**Files:**
- Create: `apps/cpp/53-renderer-hpp/include/gl.hpp`
- Create: `apps/cpp/53-renderer-hpp/src/gl.cpp`
- Modify: `src/main.cpp`

**Interfaces:**
- Produces: `void drawLine(int,int,int,int, Image&, Color)`.

- [ ] **Step 1: Declare `drawLine` in `gl.hpp`** (`#pragma once`, include `image.hpp`).

- [ ] **Step 2: Implement Bresenham in `gl.cpp`** — the standard integer algorithm: swap to make it x-major when `|dx|<|dy|`, step x from x0→x1, accumulate error, step y by ±1. Handle x0>x1 by swapping endpoints.

- [ ] **Step 3: Draw a triangle outline in `main.cpp`** — replace the orange square with three `drawLine` calls forming a triangle in white, e.g. (20,20)-(380,50)-(200,380)-(20,20).

- [ ] **Step 4: Build + render** — `make run app=53-renderer-hpp`. Expected: warning-clean; `render.ppm` shows a white triangle outline on black. (Bresenham has no float asserts; the render is the check — confirm all three edges are continuous with no gaps.)

- [ ] **Step 5: Commit** — `git commit -am "feat(53): Bresenham line + wireframe (stage 1)"`

### Task 1.4: Triangle rasterizer + barycentric + flat shading (stage 2)

**Files:**
- Modify: `include/gl.hpp`, `src/gl.cpp`, `src/main.cpp`

**Interfaces:**
- Produces: `Vec3f barycentric(Vec2f a, Vec2f b, Vec2f c, Vec2f p)`; a first `triangleFlat(Vec2f screen[3], Image&, Color)` used only this task (superseded by the shader `triangle()` in Task 1.9).

- [ ] **Step 1: Write the barycentric self-check first** (add to `selfCheck`, watch it fail until implemented):

```cpp
Vec2f A{0,0}, B{4,0}, C{0,4};
Vec3f w = barycentric(A, B, C, Vec2f{1,1});   // inside
assert(w[0]>0 && w[1]>0 && w[2]>0);
assert(std::abs(w[0]+w[1]+w[2] - 1.0f) < 1e-5f);
Vec3f wo = barycentric(A, B, C, Vec2f{5,5});  // outside -> a negative weight
assert(wo[0]<0 || wo[1]<0 || wo[2]<0);
```

- [ ] **Step 2: Implement `barycentric`** — via the cross product of `(C.x-A.x, B.x-A.x, A.x-P.x)` and the y-analog; if `|z|<1` (degenerate) return a weight with a negative component. Return `{1-(u+v), v, u}`... spell out: `Vec3f u = cross(Vec3f{C.x-A.x,B.x-A.x,A.x-P.x}, Vec3f{C.y-A.y,B.y-A.y,A.y-P.y}); if (std::abs(u[2])<1) return {-1,1,1}; return {1.f-(u[0]+u[1])/u[2], u[1]/u[2], u[0]/u[2]};`

- [ ] **Step 3: Implement `triangleFlat`** — compute the screen bounding box (clamp to image bounds), loop pixels, keep those whose barycentric weights are all ≥ 0, `set` them to the color.

- [ ] **Step 4: In `main.cpp`, render a flat-shaded triangle** — project a single hardcoded 3D triangle to screen (Y flipped), compute its normal via `cross`, flat intensity `= dot(normal, lightDir)` clamped ≥0, scale a color by it, fill with `triangleFlat`. Back-face cull: skip if intensity ≤ 0.

- [ ] **Step 5: Build + render** — Expected: warning-clean; a solid shaded triangle. Barycentric asserts pass.

- [ ] **Step 6: Commit** — `git commit -am "feat(53): barycentric triangle rasterizer + flat shading (stage 2)"`

### Task 1.5: Z-buffer (stage 3)

**Files:**
- Modify: `include/gl.hpp`, `src/gl.cpp`, `src/main.cpp`

- [ ] **Step 1: Extend the rasterizer to take a z-buffer** — `triangleFlat(Vec3f screen[3], Image&, std::vector<float>& zbuf, Color)` where `screen[i]` now carries interpolated depth in `.z`. For each covered pixel, interpolate `z = dot(bary, {s0.z,s1.z,s2.z})`; write the pixel only if `z > zbuf[x + y*width]`, then update the buffer.

- [ ] **Step 2: In `main.cpp`, allocate `std::vector<float> zbuf(W*H, -inf)`** and render two overlapping triangles at different depths (e.g. a red one behind, a green one in front).

- [ ] **Step 3: Build + render** — Expected: warning-clean; the green triangle correctly occludes the red one where they overlap — no z-fighting seams. This is the visible proof of hidden-surface removal.

- [ ] **Step 4: Commit** — `git commit -am "feat(53): z-buffer / hidden-surface removal (stage 3)"`

### Task 1.6: Model — procedural mesh + OBJ loader (stage 4)

**Files:**
- Create: `apps/cpp/53-renderer-hpp/include/model.hpp`
- Create: `apps/cpp/53-renderer-hpp/src/model.cpp`
- Modify: `src/main.cpp`

**Interfaces:**
- Produces: `Model` (see canonical interfaces). This task implements `cube()`, `sphere()`, `loadObj()`, `nfaces()`, `vert()`, `uv()`, `normal()`. Texture methods land in Task 1.7.

- [ ] **Step 1: Write `model.hpp`** — store `std::vector<Vec3f> verts_, norms_; std::vector<Vec2f> uvs_;` and `std::vector<std::array<int,3>> faceV_, faceUV_, faceN_;` (one index triple per face corner). Declare the full interface.

- [ ] **Step 2: Implement `cube()` and `sphere(lat,lon)`** — generate positions, per-vertex normals (for the sphere, the normalized position; for the cube, face normals), and planar UVs. These make the app render with zero external files.

- [ ] **Step 3: Implement `loadObj(path)`** — parse lines: `v x y z` → `verts_`; `vt u v` → `uvs_`; `vn x y z` → `norms_`; `f a/b/c d/e/f g/h/i` → push the three `(v,uv,n)` index triples (OBJ is 1-based → subtract 1; tolerate missing `vt`/`vn` as index 0). Use `std::ifstream` + `std::istringstream`. Throw `std::runtime_error` if the file won't open.

- [ ] **Step 4: Self-check** — add `Model m = Model::cube(); assert(m.nfaces() == 12);` (12 triangles for a cube). Watch fail, then pass.

- [ ] **Step 5: In `main.cpp`, render the procedural cube (or sphere) wireframe-then-filled** using the z-buffer rasterizer, projecting every face's three `vert()`s to screen. Keep the hardcoded triangles out now.

- [ ] **Step 6: Build + render** — Expected: warning-clean; a recognizable shaded cube/sphere. Cube face-count assert passes.

- [ ] **Step 7: Commit** — `git commit -am "feat(53): Model — procedural mesh + OBJ loader (stage 4)"`

### Task 1.7: Texture mapping (stage 5 — the haqr lesson)

**Files:**
- Modify: `include/model.hpp`, `src/model.cpp`, `src/gl.cpp`, `src/main.cpp`

**Interfaces:**
- Produces: `Model::setDiffuse`, `Model::diffuse(Vec2f)`; UV interpolation inside the rasterizer.

- [ ] **Step 1: Implement `diffuse(Vec2f uv)`** — if a diffuse `Image` was set, sample it at `(uv.u * (w-1), (1-uv.v) * (h-1))` via `get`; else return a **procedural checkerboard** (`((int)(uv.u*8) + (int)(uv.v*8)) % 2` → two colors). Implement `setDiffuse(const Image&)` to store a copy.

- [ ] **Step 2: Thread UVs through the rasterizer** — add a variant that, per covered pixel, interpolates `Vec2f uv = bary·(uv0,uv1,uv2)` and calls `model.diffuse(uv)` for the base color (still multiplied by flat intensity + z-tested). *(This is temporary until the shader in Task 1.9 owns interpolation.)*

- [ ] **Step 3: In `main.cpp`, render the mesh textured** — default checkerboard; if `argv[1]` is given, `model.setDiffuse(Image::readPPM(argv[1]))`.

- [ ] **Step 4: Build + render** — Expected: warning-clean; the cube/sphere wrapped in a checkerboard, shaded. Document in the README how to pass a real PPM texture.

- [ ] **Step 5: Commit** — `git commit -am "feat(53): UV texture mapping (stage 5)"`

### Task 1.8: Perspective + camera (stages 6–7)

**Files:**
- Modify: `include/gl.hpp`, `src/gl.cpp`, `src/main.cpp`

**Interfaces:**
- Produces: `viewport`, `projection`, `lookAt` (see canonical interfaces).

- [ ] **Step 1: Matrix self-checks first** — `assert` that `viewport(0,0,W,H)` maps NDC `(-1,-1)`→screen `(0,H-?)` corner and `(1,1)`→`(W-?,0)` (write exact expected pixel coords for your viewport formula); `projection(0)` == identity; `lookAt(eye,center,up)` is orthonormal (rows unit length). Watch fail, implement, pass.

- [ ] **Step 2: Implement `viewport(x,y,w,h)`** — scales NDC [-1,1] to screen [x, x+w] × [y, y+h] and depth to [0,255], as a Matrix.

- [ ] **Step 3: Implement `projection(coeff)`** — identity with `m[3][2] = coeff` (`coeff = -1/eye.z`); the later w-divide (`proj3`) produces perspective foreshortening.

- [ ] **Step 4: Implement `lookAt(eye,center,up)`** — build the orthonormal camera basis (`z=normalize(eye-center)`, `x=normalize(cross(up,z))`, `y=cross(z,x)`) into a view matrix that translates by `-center`.

- [ ] **Step 5: In `main.cpp`, compose `Viewport * Projection * View`** and transform each vertex through it before the w-divide. Rotate the model or offset the eye so perspective is visibly non-orthographic.

- [ ] **Step 6: Build + render** — Expected: warning-clean; the model shows perspective foreshortening and is viewed from the chosen camera. Matrix asserts pass.

- [ ] **Step 7: Commit** — `git commit -am "feat(53): perspective projection + camera lookAt (stages 6-7)"`

### Task 1.9: Shader abstraction — Flat/Gouraud/Phong (stage 8)

**Files:**
- Create: `apps/cpp/53-renderer-hpp/include/shader.hpp`
- Create: `apps/cpp/53-renderer-hpp/src/shader.cpp`
- Modify: `include/gl.hpp`, `src/gl.cpp`, `src/main.cpp`

**Interfaces:**
- Produces: `IShader`, `FlatShader`, `GouraudShader`, `PhongShader`; the final `void triangle(const Vec4f clip[3], IShader&, Image&, std::vector<float>& zbuf)`.

- [ ] **Step 1: Define `IShader`** in `shader.hpp` — `vertex(face,nth)→Vec4f` (transforms the vertex, stashes per-vertex varyings like UV/normal/intensity as members) and `fragment(Vec3f bary, Color& out)→bool` (interpolates varyings with `bary`, computes color, returns `true` to discard).

- [ ] **Step 2: Rewrite `triangle()` to be shader-driven** — take the three clip-space `Vec4f`, do the w-divide + viewport to screen, compute the bounding box, and for each pixel: barycentric test, z-interpolate + z-test, call `shader.fragment(bary,color)`; if not discarded, `set` the pixel. This replaces `triangleFlat`/the temporary textured rasterizer — delete those.

- [ ] **Step 3: Implement the three shaders in `shader.cpp`** — `FlatShader` (one normal per face → constant intensity), `GouraudShader` (per-vertex intensity interpolated), `PhongShader` (interpolate the normal per fragment, then `intensity = dot(n, light)`; sample `model.diffuse(uv)` for base color). Each holds a `const Model&`, the composed MVP `Matrix`, and the light direction.

- [ ] **Step 4: In `main.cpp`, select a shader** (e.g. `PhongShader`), loop faces calling `vertex` three times then `triangle(...)`. Optionally switch shader by `argv[2]`.

- [ ] **Step 5: Build + render** — Expected: warning-clean; smooth Phong shading on the model (compare visually against flat: no facet edges). z-test still correct.

- [ ] **Step 6: Commit** — `git commit -am "feat(53): IShader abstraction + Flat/Gouraud/Phong (stage 8)"`

### Task 1.10: Tangent-space normal mapping (stage 9)

**Files:**
- Modify: `include/model.hpp`, `src/model.cpp`, `include/shader.hpp`, `src/shader.cpp`, `src/main.cpp`

**Interfaces:**
- Produces: `Model::setNormalMap`, `Model::normalMap(Vec2f)`; `NormalMapShader`.

- [ ] **Step 1: Implement `normalMap(Vec2f uv)`** — if a normal-map `Image` was set, sample it and map each channel `c → c/255*2 - 1` into a `Vec3f`; else return `{0,0,1}` (flat, no perturbation). `setNormalMap` stores a copy.

- [ ] **Step 2: Implement `NormalMapShader`** — in `fragment`, build the TBN basis per triangle from the vertex positions and UVs (edge vectors `e1,e2`; UV deltas; solve for tangent/bitangent), interpolate the interpolated normal `n`, transform the sampled tangent-space normal into world space via `[T B N]`, then light with it and modulate by `diffuse(uv)`.

- [ ] **Step 3: Self-check** — `assert` that `normalMap` on an unset model returns `{0,0,1}` and that with a flat `(128,128,255)`-filled map it returns approximately `{0,0,1}` (within tolerance). Watch fail, implement, pass.

- [ ] **Step 4: In `main.cpp`, use `NormalMapShader`** with a procedural or loaded normal map (`argv[3]`).

- [ ] **Step 5: Build + render** — Expected: warning-clean; surface detail from the normal map visible as lighting variation independent of geometry. Normal-map asserts pass.

- [ ] **Step 6: Commit** — `git commit -am "feat(53): tangent-space normal mapping (stage 9)"`

### Task 1.11: `references/tinyrenderer-pipeline.md` + the 53 README

**Files:**
- Create: `references/tinyrenderer-pipeline.md`
- Create: `apps/cpp/53-renderer-hpp/README.md`

- [ ] **Step 1: Write `references/tinyrenderer-pipeline.md`** — the deep theory, once: framebuffer/PPM; Bresenham; barycentric coordinates and the inside test; z-buffer; the model→world→view→clip→screen chain with the viewport, projection (w-divide), and lookAt matrices; UV interpolation (`U = α₀U₀+α₁U₁+α₂U₂`) and the perspective-correctness caveat; the TBN matrix and normal mapping. Attribution + links to ssloy/tinyrenderer and haqr.eu. Cross-link `references/going-further-gpu.md`.

- [ ] **Step 2: Write `apps/cpp/53-renderer-hpp/README.md`** in the repo's lesson shape (Concept / **Layout contrast: `.hpp` vs no-`.hpp`** / Project layout / Pipeline stage-by-stage (link the reference) / **Build it up** (the ten stages, each a `make run` checkpoint) / Going further (shadow maps, AO, specular — links) / Attribution). Document the optional `argv` texture/normal-map paths and the `.tga`→`.ppm` conversion.

- [ ] **Step 3: Commit** — `git commit -am "docs(53): pipeline reference + README"`

---

## ⏸ REVIEW CHECKPOINT

**Stop here. 53-renderer-hpp is complete: it renders through all ten stages, builds warning/sanitizer-clean, and its README + the shared reference doc are written.** Request review before replicating into 54/55/56. Adjust the reference implementation if review surfaces issues — cheaper to fix once than four times.

---

# Phase 2 — Derive `54-renderer-flat` (single C++ TU)

**This is a mechanical flatten of 53. The source of truth is the committed 53 code — read it, do not re-derive the algorithms.** The only real change is file organization, and *that change is the lesson*.

### Task 2.1: Flatten 53 into one `main.cpp`

**Files:**
- Create: `apps/cpp/54-renderer-flat/src/main.cpp`

- [ ] **Step 1: `mkdir -p apps/cpp/54-renderer-flat/src`**
- [ ] **Step 2: Concatenate 53's translation units into one file** — paste, in dependency order, the contents of `geometry.hpp`, `image.hpp`+`image.cpp`, `model.hpp`+`model.cpp`, `gl.hpp`+`gl.cpp`, `shader.hpp`+`shader.cpp`, then `main.cpp`. Drop `#pragma once` and the cross-`#include "…"` lines; keep the standard-library includes (dedup them at the top). Everything becomes one TU.
- [ ] **Step 3: Build + render** — `make run app=54-renderer-flat`. Expected: warning-clean; **byte-identical `render.ppm` to 53** for the same args (verify with `cmp`). It must produce the same image — same renderer, different layout.
- [ ] **Step 4: Commit** — `git commit -m "feat(54): single-file C++ renderer (flatten of 53)"`

### Task 2.2: The 54 README (the contrast)

**Files:**
- Create: `apps/cpp/54-renderer-flat/README.md`

- [ ] **Step 1: Write the README** — same lesson shape, but the **Layout contrast** section is the point: what living in one TU costs (every edit recompiles everything; no reuse; names leak; hard to navigate) and what it saves (nothing to wire up; trivial to share as one file). Explicitly point back to 53 as the same renderer split into headers+sources. Link `references/tinyrenderer-pipeline.md` for the theory (do not repeat it).
- [ ] **Step 2: Commit** — `git commit -m "docs(54): single-file C++ README + layout contrast"`

---

# Phase 3 — Port to `55-renderer-h` (C with `.h`/`.c`)

**Port 53's algorithms to C.** Same pipeline, same stages, same render. The differences are the language contrast points: **named functions instead of operators** (`vec3_add(a,b)`), **structs instead of classes**, **manual `malloc`/`free`**, and **a function-pointer shader struct instead of a virtual interface**. Build/run with `make run-c app=55-renderer-h`.

### C interface mirror (agreed signatures)
```c
// geometry.h
typedef struct { float x,y; }     Vec2f;
typedef struct { float x,y,z; }   Vec3f;
typedef struct { float x,y,z,w; } Vec4f;
typedef struct { float m[4][4]; } Mat4;
Vec3f vec3_add(Vec3f,Vec3f); Vec3f vec3_sub(Vec3f,Vec3f); Vec3f vec3_scale(Vec3f,float);
float vec3_dot(Vec3f,Vec3f);  Vec3f vec3_cross(Vec3f,Vec3f); Vec3f vec3_normalize(Vec3f);
Mat4  mat4_identity(void); Mat4 mat4_mul(Mat4,Mat4); Vec4f mat4_mulv(Mat4,Vec4f);
// image.h
typedef struct { unsigned char r,g,b; } Color;
typedef struct { int w,h; Color* px; } Image;      // px = malloc(w*h)
Image img_create(int w,int h); void img_free(Image*);
void  img_set(Image*,int,int,Color); Color img_get(const Image*,int,int);
int   img_write_ppm(const Image*,const char* path);   // returns 0 ok, -1 fail
int   img_read_ppm(Image* out,const char* path);
// model.h
typedef struct { /* dynamic arrays of Vec3f/Vec2f + int index triples + optional textures */ } Model;
Model model_cube(void); Model model_sphere(int,int); int model_load_obj(Model* out,const char* path);
void  model_free(Model*);
int   model_nfaces(const Model*);
Vec3f model_vert(const Model*,int face,int nth); Vec2f model_uv(const Model*,int,int); Vec3f model_normal(const Model*,int,int);
Color model_diffuse(const Model*,Vec2f); Vec3f model_normalmap(const Model*,Vec2f);
// gl.h
Mat4 gl_viewport(int,int,int,int); Mat4 gl_projection(float); Mat4 gl_lookat(Vec3f,Vec3f,Vec3f);
void gl_line(int,int,int,int,Image*,Color);
typedef struct Shader Shader;                          // fwd
void gl_triangle(Vec4f clip[3], Shader*, Image*, float* zbuf);
// shader.h
struct Shader {
    void* uniforms;                                    // points to shader-specific state
    Vec4f (*vertex)(struct Shader*, int face, int nth);
    int   (*fragment)(struct Shader*, Vec3f bary, Color* out);  // return 1 = discard
};
```

### Task 3.1: `geometry.[h|c]` + scaffold + math self-check
- [ ] Create `apps/c/55-renderer-h/{include,src}`. Write `geometry.h`/`geometry.c` mirroring 53's math as named functions. In `main.c`, port `selfCheck()` using `assert` (dot/cross/normalize/`mat4_identity`·vec). Build `make run-c app=55-renderer-h`, confirm asserts pass, commit `feat(55): C vector/matrix math + scaffold`.

### Task 3.2: `image.[h|c]` + framebuffer/PPM (stage 0)
- [ ] Port `Image` to a struct with `malloc`'d `px`; `img_create`/`img_free` (pair every malloc with free — sanitizer-clean); `img_set`/`img_get` (clamped); `img_write_ppm`/`img_read_ppm` returning status codes (no exceptions in C). Render the orange square, commit `feat(55): C framebuffer + PPM (stage 0)`.

### Task 3.3: `gl.[h|c]` line + triangle + z-buffer (stages 1–3)
- [ ] Port `gl_line` (Bresenham), a barycentric helper, the triangle rasterizer with a `float* zbuf`. Add the barycentric assert to `selfCheck`. Render wireframe → flat triangle → two z-tested triangles across three commits (`feat(55): ... stage 1/2/3`), or one commit per stage matching Phase 1's granularity.

### Task 3.4: `model.[h|c]` procedural + OBJ (stage 4)
- [ ] Port `Model` with hand-managed dynamic arrays (grow with `realloc`; `model_free` releases all). Port `model_cube`/`model_sphere`/`model_load_obj` (parse with `fgets`+`sscanf`). Assert `model_nfaces(cube)==12`. Render the cube. Commit `feat(55): C Model — procedural + OBJ (stage 4)`.

### Task 3.5: texture mapping (stage 5)
- [ ] Port `model_diffuse` (checkerboard default / sampled PPM) and UV interpolation in the rasterizer. Render textured. Commit `feat(55): C texture mapping (stage 5)`.

### Task 3.6: perspective + camera (stages 6–7)
- [ ] Port `gl_viewport`/`gl_projection`/`gl_lookat` with the matrix asserts. Compose and transform. Render with perspective. Commit `feat(55): C perspective + camera (stages 6-7)`.

### Task 3.7: shader struct — Flat/Gouraud/Phong (stage 8)
- [ ] Implement the `Shader` function-pointer struct and three concrete shaders (each with its own `uniforms` struct + `vertex`/`fragment` functions). Rewrite `gl_triangle` to be shader-driven; delete the temporary rasterizers. Render Phong. Commit `feat(55): C shader dispatch + Flat/Gouraud/Phong (stage 8)`.

### Task 3.8: normal mapping (stage 9)
- [ ] Port `model_normalmap` + a normal-map shader (TBN in C). Assert flat-map → `~{0,0,1}`. Render. Commit `feat(55): C tangent-space normal mapping (stage 9)`.

### Task 3.9: 55 README
- [ ] Write `apps/c/55-renderer-h/README.md`: lesson shape; **Layout contrast: `.h` vs no-`.h`** (declarations in headers, definitions in `.c`, why C splits them; how the compiler+linker use them); note the C-vs-C++ contrasts (named funcs vs operators, function-pointer struct vs virtual `IShader`, `malloc`/`free` vs RAII) and cross-link 53. Link the reference doc. Commit `docs(55): C .h/.c README + contrast`.

---

# Phase 4 — Flatten to `56-renderer-flat` (single C file)

### Task 4.1: Flatten 55 into one `main.c`
- [ ] `mkdir -p apps/c/56-renderer-flat/src`. Concatenate 55's `.h`+`.c` files in dependency order into one `main.c`; drop include guards and cross-`#include "…"`; mark file-local functions `static`; dedup standard includes. Build `make run-c app=56-renderer-flat`; verify `render.ppm` is byte-identical to 55 (`cmp`). Commit `feat(56): single-file C renderer (flatten of 55)`.

### Task 4.2: 56 README
- [ ] Write `apps/c/56-renderer-flat/README.md`: same shape; **Layout contrast** = one C TU with `static` functions — what it costs/saves vs 55's `.h`/`.c` split. Point back to 55; link the reference doc. Commit `docs(56): single-file C README + contrast`.

---

# Phase 5 — Wire into the curriculum

### Task 5.1: Update the learning-path READMEs
**Files:** Modify `README.md`, `apps/cpp/README.md`, `apps/c/README.md`.
- [ ] Add 53–56 to each learning path with one-line descriptions and the "same renderer, four layouts" framing; cross-link the four apps and `references/tinyrenderer-pipeline.md`. Confirm `.gitignore` already ignores `render.ppm`/`*.png`/`build/` (it does — verify, don't duplicate). Commit `docs: add renderer lessons 53-56 to the learning path`.

### Task 5.2: Full-build verification
- [ ] Run `make build` (builds every lesson, both trees) and confirm 53–56 build warning-clean alongside everything else. Run each of the four (`make run app=53-renderer-hpp`, `run app=54-renderer-flat`, `run-c app=55-renderer-h`, `run-c app=56-renderer-flat`) and confirm all four write a `render.ppm`. Spot-check the two C++ renders are identical and the two C renders are identical (`cmp`). No commit unless a fix was needed.

---

## Self-review

**Spec coverage:** four apps at the agreed paths/names ✓; full arc stages 0–9 incl. perspective, camera, shaders, normal mapping ✓; procedural default + optional `loadObj()`/PPM textures ✓; no bundled assets, no new deps, PPM only ✓; `references/tinyrenderer-pipeline.md` written once + linked ✓; per-app layout-contrast README section ✓; assert self-checks + render + warning/sanitizer-clean verification ✓; phasing builds 53 first then pauses ✓; README wiring ✓.

**Placeholder scan:** no TBD/TODO; algorithms either given inline (Bresenham, barycentric, projection, viewport, lookAt, UV interp, TBN described with the exact formula) or explicitly delegated to "read the committed 53 source" for the mechanical ports (54/56) — a deliberate, stated choice to avoid pasting the whole renderer four times, not a placeholder.

**Type consistency:** the C++ canonical-interfaces block and the C interface-mirror block fix every signature the tasks reference (`triangle`/`gl_triangle`, `IShader`/`Shader`, `diffuse`/`model_diffuse`, `viewport`/`gl_viewport`, etc.); later tasks use exactly those names.

**Note on TDD deviation:** a software renderer's output is visual; the plan front-loads `assert` self-checks on every numerically-checkable primitive (vectors, matrices, barycentric, normal-map decode) as the fail-first tests, and uses build-clean + render-inspection for the inherently visual stages. This is the honest test cycle for the domain, stated up front in "Verification model."
