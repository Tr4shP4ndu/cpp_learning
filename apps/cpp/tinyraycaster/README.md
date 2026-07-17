# tinyraycaster

A from-scratch **raycasting engine** — the trick that made Wolfenstein 3D run on
1992 hardware. The world is a flat 2D grid of walls, but by drawing one vertical
slice per screen column (taller = closer) it looks like a first-person corridor.
No libraries, writes a PPM image with a minimap in the corner.

Inspired by Dmitry Sokolov's tutorial:
<https://github.com/ssloy/tinyraycaster/wiki> (our implementation is our own).

**Builds on:** arrays/strings/vectors (18–21), for-loop (12), operators & trig
(06), files/streams (23). A nice companion to the ray *tracer* and the
rasterizing renderer capstone (62).

## Run
```sh
make run-cpp-app-tinyraycaster                     # writes out.ppm (debug + sanitizers)
make run-cpp-app-tinyraycaster BUILD_TYPE=Release   # much faster for the render loop
```
Open `out.ppm` in a PPM-capable viewer (GIMP, or `convert out.ppm out.png`).
`out.ppm` is git-ignored.

## Build it step by step
1. **A framebuffer + PPM.** Fill sky on the top half, floor on the bottom, and
   write the `P6` image.
2. **The map.** A 16×16 grid (`'#'`/digits = walls, space = empty). Draw it as a
   small **minimap** so you can see the world from above.
3. **Cast one ray per column.** Step a ray out from the player across the field
   of view; stop at the first wall cell and record the distance `t`.
4. **Draw wall slices.** Slice height = `screen_height / distance` — closer walls
   fill more of the column. The flat grid becomes 3D.
5. **Fix the fisheye.** Straight walls bow outward until you use the
   *perpendicular* distance: multiply by `cos(ray_angle − view_angle)`.
6. **Shade & colour.** Fade slices with distance and colour them by wall type.

## Try it yourself
- Turn the player: make `dir` a command-line argument and render a few angles.
- Textured walls: sample a column of a texture instead of a flat colour.
- Draw the rays on the minimap to *see* what the caster is doing.
