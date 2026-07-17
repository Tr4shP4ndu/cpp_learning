# tinyraytracer

A from-scratch **ray tracer** — no libraries, ~200 lines, writes a PPM image.
Where the renderer capstone (lesson 63) *rasterizes* triangles onto the
screen, this traces rays of light backwards from the eye into the scene. Two
opposite ways to make the same kind of picture.

Inspired by Dmitry Sokolov's tutorial:
<https://github.com/ssloy/tinyraytracer/wiki> (our implementation is our own).

**Builds on:** operators/math (06), structs (43), recursion (30), files/streams
(23), and the vector-math + PPM ideas from the renderer capstone (62).

## Run
```sh
make run app=tinyraytracer                     # writes out.ppm (debug + sanitizers)
make run app=tinyraytracer BUILD_TYPE=Release   # much faster for the render loop
```
Open `out.ppm` in an image viewer that supports PPM (GIMP, or convert with
ImageMagick: `convert out.ppm out.png`). `out.ppm` is git-ignored.

## Build it step by step
Each stage adds one idea; the file is small enough to grow a piece at a time.

1. **A PPM image.** Fill a framebuffer with a gradient and write the binary `P6`
   header + pixels. You now have "hello world" for images.
2. **Rays & spheres.** Shoot one ray per pixel from the camera; colour a pixel
   if its ray hits a sphere (`Sphere::ray_intersect`). Silhouettes appear.
3. **Lighting.** Add point lights: Lambert **diffuse** (`max(0, N·L)`) plus a
   **specular** Phong highlight. The spheres gain shape.
4. **Shadows.** Before adding a light's contribution, shoot a *shadow ray*
   toward it; if it hits something first, skip that light.
5. **Reflection.** Recursively `cast_ray` along the mirror direction and blend
   by the material's reflect weight (`albedo[2]`). Bounded by a depth limit.
6. **Refraction.** Bend rays through glass with Snell's law (`refract`), blended
   by `albedo[3]`. The glass sphere now shows what's behind it.
7. **A floor.** Intersect a horizontal plane and tint it as a checkerboard.

## Try it yourself
- Add a fifth sphere, or change the four materials' `albedo` weights.
- Anti-alias: shoot several jittered rays per pixel and average them.
- Add a second checkerboard colour or a textured background (sky gradient).
