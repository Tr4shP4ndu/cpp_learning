#pragma once
#include "image.hpp"
#include "geometry.hpp"
#include "model.hpp"
#include <vector>

// Draws a straight line from (x0,y0) to (x1,y1) into img using color c.
// Integer Bresenham: no floating point, handles all octants (steep slopes
// and either endpoint ordering).
void drawLine(int x0, int y0, int x1, int y1, Image& img, Color c);

// Barycentric weights of p with respect to triangle (a,b,c): w[0] belongs to
// a, w[1] to b, w[2] to c. All weights sum to 1; p is inside the triangle iff
// all three are >= 0. A degenerate (zero-area) triangle returns a weight with
// a negative component so callers treat it as "outside".
Vec3f barycentric(Vec2f a, Vec2f b, Vec2f c, Vec2f p);

// Fills the triangle defined by screen[3] (screen-space, Y-down; screen[i][2]
// is depth, larger = closer to the camera) with a flat color c, using a
// clamped bounding-box scan and barycentric inside-tests over the .xy of the
// three vertices. For each covered pixel the depth is interpolated from the
// barycentric weights and compared against zbuf (indexed x + y*img.width());
// the pixel is drawn and the buffer updated only when the new depth is
// greater (i.e. closer) than what's already stored there.
void triangleFlat(Vec3f screen[3], Image& img, std::vector<float>& zbuf, Color c);

// Like triangleFlat, but instead of a flat color, interpolates uv[3] (one UV
// per screen[] vertex, same winding) via the covered pixel's barycentric
// weights and samples model.diffuse() at that UV. The sampled color is then
// scaled by intensity (flat per-face lighting) before z-testing and drawing.
// Temporary: Task 1.9's shader abstraction will subsume both triangleFlat and
// this into a single per-pixel-programmable rasterizer.
void triangleTextured(Vec3f screen[3], Vec2f uv[3], const Model& model, float intensity, Image& img,
                       std::vector<float>& zbuf);
