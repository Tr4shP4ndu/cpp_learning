# Going further: real GPU programming

The `63-mini-renderer` capstone runs the graphics pipeline **on the CPU** so you
can learn how it works with zero dependencies. A real GPU runs that exact
pipeline in massively parallel hardware. Here's how what you built maps to the
real thing, and where to go next.

## Your CPU renderer → the GPU pipeline
| In the capstone | On a GPU |
|---|---|
| `toScreen()` — transform each vertex | **Vertex shader** |
| `drawTriangle()` bounding-box loop | Fixed-function **rasterizer** |
| the per-pixel body (color/shade) | **Fragment (pixel) shader** |
| the `zbuffer` vector + `z > zbuffer[i]` test | Hardware **depth buffer** |
| your `for` over triangles | Thousands of cores running in parallel |

The key mental shift: on a GPU you don't loop over pixels — you write a small
program (a *shader*) that runs *once per vertex* and *once per fragment*, and the
hardware runs millions of those in parallel.

## Shaders (the programmable stages)
- **GLSL** (OpenGL/Vulkan) and **HLSL** (DirectX) and **MSL** (Metal) are the
  shader languages. A fragment shader looks a lot like your per-pixel code.
- Start here: **learnopengl.com** — the best free, modern OpenGL tutorial;
  teaches shaders, transforms, lighting, texturing.

## Graphics APIs (talking to the GPU)
- **OpenGL** — oldest, easiest to start, widely supported. Good first API.
- **Vulkan** — modern, explicit, verbose; maximum control and performance.
- **Metal** (Apple) / **Direct3D** (Windows) — platform-native.
- **WebGPU** — the emerging cross-platform standard (also runs in browsers).

These need SDKs/drivers and a windowing library (GLFW/SDL), which is why this
course teaches the *concepts* on the CPU instead — no setup, no dependencies.

## GPGPU — using the GPU for non-graphics work
- **CUDA** (NVIDIA) and **compute shaders** (Vulkan/Metal/WebGPU) run general
  parallel computation (ML, simulation, image processing) on the GPU.

## Go deeper on software rendering first
- **[ssloy/tinyrenderer](https://github.com/ssloy/tinyrenderer)** — the course
  that inspired the capstone. Builds a full renderer (OBJ models, textures,
  perspective, shaders) step by step, all on the CPU. The natural next step
  before touching a real GPU.
- **Scratchapixel** (scratchapixel.com) — the math and theory of rendering.

## Suggested path
1. Extend the capstone: OBJ loading → texturing → perspective (see its README).
2. Work through tinyrenderer for the full picture.
3. Then learnopengl.com to run the same ideas on real GPU hardware.
