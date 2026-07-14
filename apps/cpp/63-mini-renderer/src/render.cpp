#include "render.hpp"
#include <algorithm>
#include <cmath>

void drawLine(Image& img, int x0, int y0, int x1, int y1, Color c) {
    // Bresenham: step along the longer axis, tracking accumulated error to
    // decide when to step the shorter axis. Integer-only, no floats.
    bool steep = std::abs(y1 - y0) > std::abs(x1 - x0);
    if (steep) { std::swap(x0, y0); std::swap(x1, y1); }   // work in x-major space
    if (x0 > x1) { std::swap(x0, x1); std::swap(y0, y1); }

    int dx = x1 - x0;
    int dy = std::abs(y1 - y0);
    int err = dx / 2;
    int ystep = (y0 < y1) ? 1 : -1;
    int y = y0;
    for (int x = x0; x <= x1; ++x) {
        if (steep) img.set(y, x, c); else img.set(x, y, c);
        err -= dy;
        if (err < 0) { y += ystep; err += dx; }
    }
}

// Signed area of the triangle (a,b,c) in 2D — also the cross product z.
static float edge(const Vec3f& a, const Vec3f& b, const Vec3f& c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

void drawTriangle(Image& img, std::vector<float>& zbuffer,
                  Vec3f p0, Vec3f p1, Vec3f p2, Color c) {
    // Bounding box of the triangle, clipped to the image.
    int minX = std::max(0, (int)std::floor(std::min({p0.x, p1.x, p2.x})));
    int maxX = std::min(img.width()  - 1, (int)std::ceil(std::max({p0.x, p1.x, p2.x})));
    int minY = std::max(0, (int)std::floor(std::min({p0.y, p1.y, p2.y})));
    int maxY = std::min(img.height() - 1, (int)std::ceil(std::max({p0.y, p1.y, p2.y})));

    float area = edge(p0, p1, p2);
    if (area == 0) return;   // degenerate

    for (int y = minY; y <= maxY; ++y) {
        for (int x = minX; x <= maxX; ++x) {
            Vec3f p{(float)x + 0.5f, (float)y + 0.5f, 0};
            // Barycentric weights via signed sub-triangle areas.
            float w0 = edge(p1, p2, p) / area;
            float w1 = edge(p2, p0, p) / area;
            float w2 = edge(p0, p1, p) / area;
            if (w0 < 0 || w1 < 0 || w2 < 0) continue;   // outside the triangle

            // Interpolate depth across the triangle.
            float z = w0 * p0.z + w1 * p1.z + w2 * p2.z;
            std::size_t idx = static_cast<std::size_t>(y) * img.width() + x;
            if (z > zbuffer[idx]) {          // closer than what's already there
                zbuffer[idx] = z;
                img.set(x, y, c);
            }
        }
    }
}
