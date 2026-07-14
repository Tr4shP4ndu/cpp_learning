# 64-renderer-h

## Concept
The **same renderer as [62-renderer-hpp](../../cpp/62-renderer-hpp/)**, ported to
**C** with a `.h` / `.c` split: headers declare the interface, `.c` files define
it, the linker joins them. Same pipeline, same stages, same image — the
interesting differences are what C makes you do by hand that C++ did for you.

Read 53's README for the stage-by-stage build-up; this page covers the C port.

## Layout: `.h` vs no-`.h`
Just like C++'s header/source split (lesson 31), but with C's tools:

- **`.h` headers** declare types (`struct`/`typedef`) and function *prototypes*,
  guarded with `#ifndef X_H / #define X_H / #endif` (the classic include guard;
  C has no `#pragma once` guarantee, though most compilers accept it).
- **`.c` sources** `#include` their header and provide the one definition of each
  function. The compiler builds each `.c` to an `.o`; the linker resolves the
  calls between them. Change one `.c`, recompile only it, relink.

## C vs C++ — the real contrast points
Same algorithms, different language. Porting 53 surfaces exactly where C++'s
conveniences were doing work:

| Concern | 53 (C++) | 55 (C) |
|---|---|---|
| Vector math | operator overloading: `a + b`, `dot(a,b)` | **named functions**: `vec3_add(a,b)`, `vec3_dot(a,b)` |
| Vector type | generic `Vec<N,T>` template, `v[i]` | plain structs `Vec3f{float x,y,z;}`, `v.x` |
| Framebuffer / arrays | `std::vector` (grows itself, frees itself) | **`malloc`/`realloc`/`free`** by hand; a `PUSH` macro for growth |
| Optional texture | `std::optional<Image>` | an `Image` + a `has_diffuse` flag |
| Errors | `throw` / `try`-`catch` | **return codes** (`0` ok, `-1` fail) |
| Shader polymorphism | virtual `IShader` (vtable) | **a struct of function pointers** + a `void* state` blob |
| Cleanup | RAII (destructors) | explicit `img_free` / `model_free`; every `malloc` paired with a `free` |

That last row is why this variant is built and tested under
AddressSanitizer + LeakSanitizer: in C, forgetting a `free` is a leak the
language won't catch for you.

## Project layout
```
64-renderer-h/
  include/
    geometry.h  image.h  model.h  gl.h  shader.h
  src/
    geometry.c  image.c  model.c  gl.c  shader.c  main.c
  README.md
```

## Build & run
```sh
make run-c app=64-renderer-h
```
Writes `render.ppm` here. Same command-line arguments as 53
(`[model] [shader] [diffuse.ppm] [normalmap.ppm]`) and the same
[`assets/tinyrenderer/`](../../../assets/tinyrenderer/) test models:
```sh
make run-c app=64-renderer-h ARGS="assets/tinyrenderer/african_head/african_head.obj normal assets/tinyrenderer/african_head/african_head_diffuse.ppm assets/tinyrenderer/african_head/african_head_nm_tangent.ppm"
```
`model` may be `cube` (default), `sphere`, or a `.obj` path; the model is
auto-scaled to fit. Only **PPM (P6)** is read (convert with
`magick tex.png tex.ppm`).

## Attribution
Inspired by [ssloy/tinyrenderer](https://github.com/ssloy/tinyrenderer) and the
walkthrough at [haqr.eu/tinyrenderer](https://haqr.eu/tinyrenderer/). The prose
here is original; the algorithms are the classic ones they teach.
