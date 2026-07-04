#pragma once
#include "image.hpp"
#include "geometry.hpp"

// Draws a straight line from (x0,y0) to (x1,y1) into img using color c.
// Integer Bresenham: no floating point, handles all octants (steep slopes
// and either endpoint ordering).
void drawLine(int x0, int y0, int x1, int y1, Image& img, Color c);

// Barycentric weights of p with respect to triangle (a,b,c): w[0] belongs to
// a, w[1] to b, w[2] to c. All weights sum to 1; p is inside the triangle iff
// all three are >= 0. A degenerate (zero-area) triangle returns a weight with
// a negative component so callers treat it as "outside".
Vec3f barycentric(Vec2f a, Vec2f b, Vec2f c, Vec2f p);

// Fills the triangle defined by screen[3] (screen-space, Y-down) with a flat
// color c, using a clamped bounding-box scan and barycentric inside-tests.
void triangleFlat(Vec2f screen[3], Image& img, Color c);
