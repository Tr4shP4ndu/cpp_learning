#pragma once
#include "image.hpp"
#include "geometry.hpp"
#include <vector>

struct IShader;  // defined in shader.hpp; triangle() drives it per-pixel

// Maps NDC coordinates in [-1,1]^3 to screen space [x, x+w] x [y, y+h] x
// [0,255] (the last being a depth range suitable for a z-buffer). Because
// Image (see image.hpp) is row-major with the origin at the TOP-LEFT, while
// NDC/camera space is right-handed with +y "up", a naive port of the classic
// (tinyrenderer) viewport formula would render the model upside down. This
// viewport folds the needed Y-flip in here (by negating the y-scale term
// while keeping the same y-offset), so callers never need to flip anything
// themselves: NDC y=+1 (up) lands at the smaller/top screen row, NDC y=-1
// (down) lands at the larger/bottom screen row.
Matrix viewport(int x, int y, int w, int h);

// Builds a perspective-projection matrix: identity except m[3][2] = coeff.
// Multiplying a view-space point by this matrix leaves x/y/z untouched but
// sets w = coeff*z + 1; the later perspective divide (proj3) by that w is
// what produces foreshortening. coeff = 0 recovers the identity (orthographic,
// no divide-driven distortion). Per this lesson's convention, coeff = -1/eye.z
// for a camera placed at distance eye.z along +z from a center at the origin.
Matrix projection(float coeff);

// Builds a view matrix that transforms world-space points into the camera's
// coordinate frame: camera at `eye`, looking toward `center`, with `up`
// defining "up" (need not be exactly perpendicular to the view direction).
// Convention (tinyrenderer): the camera basis is z = normalize(eye-center),
// x = normalize(cross(up,z)), y = cross(z,x) (already unit since x,z are
// orthonormal); these become the ROWS of the rotation part Minv, and the
// result is Minv * Tr, where Tr translates by -center. (Applying Tr first
// moves the world so `center` sits at the origin; Minv then rotates into the
// camera's axes.)
Matrix lookAt(Vec3f eye, Vec3f center, Vec3f up);

// Draws a straight line from (x0,y0) to (x1,y1) into img using color c.
// Integer Bresenham: no floating point, handles all octants (steep slopes
// and either endpoint ordering).
void drawLine(int x0, int y0, int x1, int y1, Image& img, Color c);

// Barycentric weights of p with respect to triangle (a,b,c): w[0] belongs to
// a, w[1] to b, w[2] to c. All weights sum to 1; p is inside the triangle iff
// all three are >= 0. A degenerate (zero-area) triangle returns a weight with
// a negative component so callers treat it as "outside".
Vec3f barycentric(Vec2f a, Vec2f b, Vec2f c, Vec2f p);

// Rasterizes one triangle whose three corners are given in CLIP space (the
// output of IShader::vertex, i.e. after projection*view but before the
// perspective divide and viewport). triangle() owns those two final steps: it
// does the w-divide (proj3) on each corner, then maps NDC to screen with a
// viewport(0,0,img.width(),img.height()) matrix. It then scans the clamped
// bounding box, and for every pixel inside the triangle (all barycentric
// weights >= 0) interpolates depth (screen[i][2], larger = closer) and z-tests
// against zbuf (indexed x + y*img.width()). Only on passing the z-test does it
// invoke shader.fragment(bary, color); the pixel is drawn and the z-buffer
// updated iff the shader keeps the fragment (returns false, i.e. no discard).
void triangle(const Vec4f clip[3], IShader& shader, Image& img, std::vector<float>& zbuf);
