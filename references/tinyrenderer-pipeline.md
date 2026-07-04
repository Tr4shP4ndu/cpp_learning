# The tinyrenderer pipeline (shared theory)

This is the math and theory behind the four renderer lessons — **53** (C++
`include/`+`src/`), **54** (single C++ file), **55** (C `.h`/`.c`), and **56**
(single C file). All four implement the *same* pipeline; only the file layout
differs. Read this once; each app's README links back here instead of repeating
it.

The renderer is a CPU software rasterizer in the style of
[ssloy/tinyrenderer](https://github.com/ssloy/tinyrenderer) and the excellent
walkthrough at [haqr.eu/tinyrenderer](https://haqr.eu/tinyrenderer/). The prose
here is original; the algorithms are the classic ones those references teach.
For where to go after a software rasterizer (real GPUs, shading languages), see
[`going-further-gpu.md`](going-further-gpu.md).

> **Conventions used throughout** (they matter — several are non-textbook):
> - **Vectors** index with `[]` (`v[0]`, `v[1]`, `v[2]`), not `.x/.y/.z`.
> - **Matrices** are row-major; a point is a column vector on the right:
>   `M * v`. Composition reads right-to-left: `proj * view * model`.
> - **The image origin is top-left**, row 0 at the top (that's how PPM stores
>   pixels). NDC and camera space are right-handed with **+y up**. The single
>   Y-flip that reconciles them is folded into the **viewport** matrix.
> - **Depth: larger z = nearer the camera.** The z-buffer starts at `-inf` and a
>   fragment wins the depth test when `z > zbuf`. (Many tutorials use the
>   opposite sign; we picked one and kept it everywhere.)

---

## 0. Framebuffer & PPM

An image is just an array of pixels. We store `width*height` RGB triples
(`Color{r,g,b}`, one byte each) row-major with the origin top-left, and write
them as **binary PPM (P6)** — the simplest real image format:

```
P6\n
<width> <height>\n
255\n
<width*height*3 raw bytes: R G B, R G B, ...>
```

`writePPM` emits that header then the raw bytes. `readPPM` parses the header
(magic `P6`, width, height, maxval), consumes the single whitespace byte after
`maxval`, then reads `width*height*3` bytes. `set(x,y,c)` ignores out-of-range
writes; `get(x,y)` **clamps** to the edge, which makes it safe to use directly
as a texture sampler.

To view a PPM, open it in an image viewer that supports it (GIMP, many editors)
or convert it: `magick render.ppm render.png`.

---

## 1. Lines — Bresenham

Drawing a straight line between two pixels with only integer arithmetic. The
trick is to always step along the **major axis** (the longer of Δx, Δy) one
pixel at a time, and accumulate an integer error term that decides when to step
the minor axis:

- If the line is *steep* (`|Δy| > |Δx|`), transpose x↔y so we always iterate x.
- If the endpoints are right-to-left, swap them so x increases.
- Walk `x` from `x0` to `x1`; keep an error accumulator; when it goes negative,
  step `y` by ±1 and add `Δx` back.

No floating point, no gaps. This draws the wireframe; the filled triangle uses a
different method (below).

---

## 2. Triangles — barycentric coordinates

To *fill* a triangle we scan its bounding box and ask, for each pixel: **is it
inside, and if so, how do the three vertices' attributes blend here?** Both
answers come from **barycentric coordinates** `(w0, w1, w2)` — weights on the
three vertices A, B, C such that the point `P = w0·A + w1·B + w2·C` and
`w0 + w1 + w2 = 1`.

- **Inside test:** P is inside the triangle **iff all three weights are ≥ 0**.
- **Interpolation:** any per-vertex attribute (depth, UV, normal, colour) at P
  is `w0·attr(A) + w1·attr(B) + w2·attr(C)`.

We compute them with a cross product. For the vertices' x- and y-components:

```
u = cross( (Cx-Ax, Bx-Ax, Ax-Px),
           (Cy-Ay, By-Ay, Ay-Py) )
```

`u[2]` is twice the signed triangle area. If `|u[2]| < 1` the triangle is
degenerate on the pixel grid, so we return a weight with a negative component
("outside") rather than divide by ~0. Otherwise:

```
(w0, w1, w2) = ( 1 - (u0+u1)/u2 ,  u1/u2 ,  u0/u2 )
```

---

## 3. Z-buffer (hidden-surface removal)

Triangles arrive in arbitrary order, so "paint the last one on top" is wrong —
a far triangle drawn late would cover a near one. The fix: keep one **depth per
pixel** in a `zbuf` array the same size as the image, initialised to `-inf`.

For each covered pixel, interpolate its depth from the barycentric weights and
the three vertices' depths, then draw **only if it is nearer than what's already
there** (`z > zbuf[idx]`), updating `zbuf` when it wins. `idx = x + y*width`.

This is the single idea that turns a pile of triangles into a solid,
correctly-occluded object. On a real GPU it's dedicated hardware.

---

## 4. The coordinate chain: model → world → view → clip → screen

A vertex travels through a sequence of spaces, each a matrix multiply:

```
model space  --(model matrix)-->  world space
world space  --(view = lookAt)-->  camera/eye space
eye space    --(projection)------>  clip space
clip space   --(÷ w, "proj3")---->  NDC (normalized device coords, [-1,1]^3)
NDC          --(viewport)-------->  screen space (pixels + depth 0..255)
```

The programmable split (stage 8 below) draws the line **at clip space**: a
vertex shader returns clip-space coordinates; the rasterizer owns the last two
steps (perspective divide + viewport).

### `lookAt(eye, center, up)` — the view matrix

Builds an orthonormal camera basis and the matrix that expresses world points in
it:

```
z = normalize(eye - center)     // camera looks along -z toward center
x = normalize(cross(up, z))     // right
y = cross(z, x)                 // true up (already unit)
```

Those become the **rows** of the rotation part `Minv`; the view matrix is
`Minv * Tr` where `Tr` translates by `-center`. (Translate so `center` sits at
the origin, then rotate into the camera's axes.) In these lessons the camera is
placed on **+z** looking at a `center` at the origin.

### `projection(coeff)` — perspective

A deliberately minimal perspective matrix: identity except `m[3][2] = coeff`.
Multiplying an eye-space point by it leaves x/y/z alone but sets
`w = coeff·z + 1`. The **perspective divide** later (`proj3`, which divides
x/y/z by w) is what makes nearer things bigger. `coeff = -1/eye.z` for a camera
at distance `eye.z`; `coeff = 0` gives `w = 1` — orthographic, no foreshortening.

> There is no separate focal-length/FOV term here, so whether an object fits the
> `[-1,1]` NDC box depends on its own eye-space size (moving the eye back scales
> `coeff` to compensate and cancels out). Models that aren't pre-normalized to a
> unit sphere — like our procedural cube, whose corners sit at distance √3 —
> need a small uniform **model scale** (≈0.6) to stay framed. That scale is a
> model matrix; it never touches normals (uniform scaling preserves direction).

### `viewport(x, y, w, h)` — NDC → screen

Scales NDC `[-1,1]` to the pixel rectangle `[x, x+w] × [y, y+h]` and depth to
`[0, 255]`:

```
screen_x =  w/2 · ndc_x + (x + w/2)
screen_y = -h/2 · ndc_y + (y + h/2)      // NEGATED: folds in the Y-flip
depth    =  255/2 · ndc_z + 255/2
```

The negated y-scale is the whole Y-flip: NDC `y=+1` (up) maps to the *smaller*
(top) screen row, matching the top-left image origin. Do the flip here and
nowhere else.

---

## 5. Texture mapping & UV interpolation

Each vertex carries a 2D **texture coordinate** (UV) in `[0,1]`. Per covered
pixel, interpolate it with the barycentric weights,

```
U = w0·U0 + w1·U1 + w2·U2
```

and sample the diffuse texture at `(U·(texW-1), (1-V)·(texH-1))` — the `1-V`
because texture V=1 is conventionally the top. With no texture loaded, we sample
a **procedural checkerboard** so the mapping is visible without any asset.

> **Perspective-correctness caveat.** Plain barycentric interpolation of UV is
> affine — correct in screen space only for an orthographic view. Under
> perspective it's subtly wrong (textures "swim" on steep surfaces); the exact
> fix is to interpolate `U/w` and `1/w` and divide. These lessons keep the
> simple affine interpolation; the error is small at our angles and the fix is a
> noted stretch goal.

---

## 6. Programmable shading (Flat / Gouraud / Phong)

The rasterizer is made generic by a tiny **shader interface** mirroring a GPU's
vertex/fragment split:

- `vertex(face, nth)` — transform corner `nth` of a face to **clip space** and
  stash that corner's *varyings* (UV, normal, precomputed intensity, …).
- `fragment(bary, out)` — interpolate the stashed varyings with the barycentric
  weights, compute the final colour into `out`, and return whether to **discard**
  the fragment.

The rasterizer calls `vertex` three times, does the divide + viewport, then for
each inside, z-passing pixel calls `fragment`. Three classic lighting models
drop in without touching the rasterizer:

- **Flat** — one normal for the whole triangle → one constant Lambert
  intensity. Shows facet edges on a low-poly mesh.
- **Gouraud** — Lambert intensity computed per vertex, interpolated across the
  face. Smooth gradients, but highlights between vertices can be missed.
- **Phong** — the *normal* is interpolated per pixel and lit per fragment.
  Smoothest; correct highlights.

**Lambert** diffuse intensity is `clamp(dot(normalize(n), lightDir), 0, 1)` —
how squarely the surface faces the light; back-facing fragments clamp to 0.

> **Normals are directions, not points.** When only the camera moves (no model
> transform beyond a uniform scale), model space *is* world space and normals
> are used as-is — never pushed through the projection/view matrix, which carries
> translation and the perspective term and would corrupt them. With a real model
> transform you'd use its inverse-transpose (the "normal matrix").

---

## 7. Tangent-space normal mapping

A **normal map** is a texture whose RGB encodes a per-texel surface normal in
*tangent space* (a frame local to the surface), each channel packed as
`c = (n + 1)/2 · 255`. Decoding is `n = c/255 · 2 − 1`. A flat map is the
constant `(128,128,255)` → `(0,0,1)`, meaning "no perturbation."

To use it we need the **TBN basis** — Tangent, Bitangent, Normal — that maps
tangent space into world space. Per triangle, from the edge vectors and the
matching UV deltas:

```
e1 = P1 - P0,   e2 = P2 - P0
(du1,dv1) = UV1 - UV0,   (du2,dv2) = UV2 - UV0
det = du1·dv2 - du2·dv1
T = ( dv2·e1 - dv1·e2) / det
B = (-du2·e1 + du1·e2) / det
```

Take the interpolated per-fragment normal `N`, orthonormalize the tangent
against it (Gram-Schmidt: `T = normalize(T - dot(T,N)·N)`, `B = cross(N,T)`),
then transform the sampled tangent-space normal into world space via `[T B N]`
and light with the result. This adds fine surface detail (bumps, grooves) that
varies the lighting **independently of the geometry** — a low-poly surface can
look richly detailed. Degenerate UVs (`det ≈ 0`) fall back to the smooth `N`.

---

## The pipeline, end to end

```
for each face (triangle):
    for nth in 0,1,2:
        clip[nth] = shader.vertex(face, nth)        // model→world→view→clip
    triangle(clip, shader, image, zbuf):
        for i in 0,1,2:
            screen[i] = viewport( proj3(clip[i]) )   // ÷w (NDC) then →pixels
        for each pixel in bounding box:
            bary = barycentric(screen, pixel)
            if any weight < 0: continue              // outside
            z = interpolate depth with bary
            if z <= zbuf[idx]: continue              // occluded
            if shader.fragment(bary, color): continue // discarded
            image.set(pixel, color); zbuf[idx] = z
```

That is a complete, if minimal, GPU pipeline running on the CPU.

---

## Attribution

- [ssloy/tinyrenderer](https://github.com/ssloy/tinyrenderer) — the original
  course this pipeline follows.
- [haqr.eu/tinyrenderer](https://haqr.eu/tinyrenderer/) — a clear modern
  walkthrough of the same material.
- [`going-further-gpu.md`](going-further-gpu.md) — how these stages map onto real
  GPU hardware and shading languages, and what to learn next.
