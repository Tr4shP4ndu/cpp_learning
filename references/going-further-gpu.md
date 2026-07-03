# Going further: real GPU programming

The `52-mini-renderer` capstone implements the graphics pipeline **on the CPU**
so you can learn how it works with zero dependencies. Real GPUs run that same
pipeline in massively parallel hardware. This is a stub — it will be filled in
during the capstone phase. Planned contents:

- **How a GPU runs the pipeline** — vertex → rasterize → fragment, mapped onto
  the CPU renderer you built.
- **Shaders (GLSL/HLSL)** — the programmable stages; how they relate to our
  software shading step.
- **APIs** — OpenGL / Vulkan / Metal / DirectX: what each is for, trade-offs.
- **GPGPU** — CUDA and compute shaders for non-graphics parallel work.
- **Further study** — the [ssloy/tinyrenderer](https://github.com/ssloy/tinyrenderer)
  course (the inspiration for our capstone) and learnopengl.com.
