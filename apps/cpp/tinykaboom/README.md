# tinykaboom

A **ray-marched fireball** in ~180 lines — no libraries, writes a PPM. This is
the third way to make a picture: not rasterizing triangles (the capstone), not
intersecting rays with geometry (the ray tracer), but *marching* each ray
forward in steps sized by a **signed distance function**, where the surface is a
sphere pushed around by fractal noise.

Inspired by Dmitry Sokolov's tutorial:
<https://github.com/ssloy/tinykaboom/wiki> (our implementation is our own).

**Builds on:** operators/math (06), structs (43), recursion-ish helpers, files
(23). Completes the "graphics from scratch" set alongside the renderer (62),
ray tracer, and raycaster.

## Run
```sh
make run app=tinykaboom                     # writes out.ppm (debug + sanitizers)
make run app=tinykaboom BUILD_TYPE=Release   # much faster for the render loop
```
Open `out.ppm` in a PPM-capable viewer (GIMP, or `convert out.ppm out.png`).
`out.ppm` is git-ignored.

## Build it step by step
1. **A sphere SDF.** `signed_distance(p) = |p| − r` — negative inside, zero on
   the surface. Verify by ray-marching a plain sphere.
2. **March the rays.** From the camera, step forward by the SDF value until it
   goes negative (a hit) or you give up. That's *sphere tracing*.
3. **Noise.** Add value noise + fractal Brownian motion (`fbm`) — a cheap way to
   get natural-looking turbulence.
4. **Displace the surface.** Subtract `fbm(p)·amplitude` from the radius so the
   sphere grows flame-like bumps.
5. **Shade it.** Estimate the normal from the SDF gradient and add a light.
6. **Colour by heat.** Map how far the surface bulged through a fire palette
   (gray → red → orange → yellow).

## Try it yourself
- Animate it: add a `time` offset to the noise input and render a few frames.
- Change `noise_amplitude` / the palette for smoke, plasma, or a planet.
- Add a second, smaller fireball (a second SDF, take the `min` of the two).
