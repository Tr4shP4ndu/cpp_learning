# tinyrenderer test assets

Sample models + textures for the renderer lessons (**53** / **54** / **55** /
**56**). Pass an `.obj` (and optionally a diffuse / normal-map `.ppm`) on the
command line to render a real mesh instead of the built-in procedural cube.

```
assets/tinyrenderer/
  african_head/   african_head.obj + *_diffuse.ppm + *_nm_tangent.ppm
  diablo3_pose/   diablo3_pose.obj + *_diffuse.ppm + *_nm_tangent.ppm
```

Textures are **PPM (P6)** — converted from the upstream `.tga` with
`magick in.tga out.ppm` — because the renderers read PPM only (no image
libraries in-tree). The `.obj` files are the upstream text meshes, unchanged.

## Using them

```sh
# make run-cpp-app-63-renderer ARGS="<model> [shader] [diffuse] [normalmap]"
make run-cpp-app-63-renderer ARGS="assets/tinyrenderer/african_head/african_head.obj phong assets/tinyrenderer/african_head/african_head_diffuse.ppm"
make run-cpp-app-63-renderer ARGS="assets/tinyrenderer/african_head/african_head.obj normal assets/tinyrenderer/african_head/african_head_diffuse.ppm assets/tinyrenderer/african_head/african_head_nm_tangent.ppm"

# another model:
make run-cpp-app-63-renderer ARGS="assets/tinyrenderer/diablo3_pose/diablo3_pose.obj phong assets/tinyrenderer/diablo3_pose/diablo3_pose_diffuse.ppm"
```

`model` may also be `cube` or `sphere` for the built-in procedural meshes.

## Attribution

These assets are from [ssloy/tinyrenderer](https://github.com/ssloy/tinyrenderer)
and are **not** original to this course. See each model's `ATTRIBUTION.txt`:

- **african_head** — © 2007 Vidar Rapp, used with permission.
- **diablo3_pose** — credits to Samuel (arshlevon) Sharit, used with permission.
