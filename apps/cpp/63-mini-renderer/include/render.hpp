// render.hpp — the drawing primitives: lines and z-buffered triangles.
#pragma once
#include <vector>
#include "image.hpp"
#include "vec.hpp"

// Draw a line between two screen points (Bresenham's algorithm).
void drawLine(Image& img, int x0, int y0, int x1, int y1, Color c);

// Fill a triangle whose vertices are already in SCREEN space:
//   p.x, p.y = pixel coordinates,  p.z = depth (larger = closer).
// The z-buffer (one float per pixel) resolves which triangle is visible where.
void drawTriangle(Image& img, std::vector<float>& zbuffer,
                  Vec3f p0, Vec3f p1, Vec3f p2, Color c);
