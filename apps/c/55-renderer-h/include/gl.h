#ifndef GL_H
#define GL_H

#include "geometry.h"
#include "image.h"

// C port of 53's gl.hpp. Same pipeline helpers as named functions; the z-buffer
// is a plain float* the caller owns. Conventions match 53 exactly: the viewport
// folds the single Y-flip; depth is [0,255] with LARGER z = nearer; the z-test
// is `z > zbuf`.

typedef struct Shader Shader;  // defined in shader.h; gl_triangle drives it

Mat4  gl_viewport(int x, int y, int w, int h);  // NDC -> screen, depth 0..255 (Y-flipped)
Mat4  gl_projection(float coeff);               // identity + m[3][2]=coeff
Mat4  gl_lookat(Vec3f eye, Vec3f center, Vec3f up);

void  gl_line(int x0, int y0, int x1, int y1, Image* img, Color c);  // Bresenham
Vec3f gl_barycentric(Vec2f a, Vec2f b, Vec2f c, Vec2f p);

// Rasterize a triangle whose corners are in CLIP space (from Shader.vertex):
// does the w-divide + viewport, scans the bounding box, z-tests, and calls
// sh->fragment per kept pixel. zbuf is img->w*img->h floats.
void  gl_triangle(const Vec4f clip[3], Shader* sh, Image* img, float* zbuf);

#endif // GL_H
