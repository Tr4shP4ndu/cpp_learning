#include "gl.hpp"
#include <algorithm>
#include <cmath>
#include <cstdlib>

void drawLine(int x0, int y0, int x1, int y1, Image& img, Color c) {
    // Steep lines (|dy| > |dx|) are transposed so we always step along the
    // major axis; this keeps the error accumulator integer-only and gap-free.
    const bool steep = std::abs(y1 - y0) > std::abs(x1 - x0);
    if (steep) {
        std::swap(x0, y0);
        std::swap(x1, y1);
    }
    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    const int dx = x1 - x0;
    const int dy = std::abs(y1 - y0);
    const int yStep = (y0 < y1) ? 1 : -1;

    int error = dx / 2;
    int y = y0;
    for (int x = x0; x <= x1; ++x) {
        if (steep) {
            img.set(y, x, c);
        } else {
            img.set(x, y, c);
        }
        error -= dy;
        if (error < 0) {
            y += yStep;
            error += dx;
        }
    }
}

Vec3f barycentric(Vec2f a, Vec2f b, Vec2f c, Vec2f p) {
    // Cross the edge vectors' x- and y-components; u.z is (twice) the signed
    // triangle area. |u.z| < 1 means the triangle is degenerate on the pixel
    // grid, so we report "outside" via a negative weight rather than divide
    // by (near) zero.
    Vec3f u = cross(Vec3f{c[0] - a[0], b[0] - a[0], a[0] - p[0]},
                     Vec3f{c[1] - a[1], b[1] - a[1], a[1] - p[1]});
    if (std::abs(u[2]) < 1) return Vec3f{-1, 1, 1};
    return Vec3f{1.f - (u[0] + u[1]) / u[2], u[1] / u[2], u[0] / u[2]};
}

void triangleFlat(Vec3f screen[3], Image& img, std::vector<float>& zbuf, Color c) {
    const Vec2f a{screen[0][0], screen[0][1]};
    const Vec2f b{screen[1][0], screen[1][1]};
    const Vec2f cc{screen[2][0], screen[2][1]};

    const float minXf = std::min({a[0], b[0], cc[0]});
    const float maxXf = std::max({a[0], b[0], cc[0]});
    const float minYf = std::min({a[1], b[1], cc[1]});
    const float maxYf = std::max({a[1], b[1], cc[1]});

    const int x0 = std::clamp(static_cast<int>(std::floor(minXf)), 0, img.width() - 1);
    const int x1 = std::clamp(static_cast<int>(std::ceil(maxXf)), 0, img.width() - 1);
    const int y0 = std::clamp(static_cast<int>(std::floor(minYf)), 0, img.height() - 1);
    const int y1 = std::clamp(static_cast<int>(std::ceil(maxYf)), 0, img.height() - 1);

    for (int y = y0; y <= y1; ++y) {
        for (int x = x0; x <= x1; ++x) {
            const Vec2f p{static_cast<float>(x), static_cast<float>(y)};
            const Vec3f w = barycentric(a, b, cc, p);
            if (w[0] >= 0 && w[1] >= 0 && w[2] >= 0) {
                const float z = w[0] * screen[0][2] + w[1] * screen[1][2] + w[2] * screen[2][2];
                const std::size_t idx =
                    static_cast<std::size_t>(x) + static_cast<std::size_t>(y) * static_cast<std::size_t>(img.width());
                if (z > zbuf[idx]) {
                    zbuf[idx] = z;
                    img.set(x, y, c);
                }
            }
        }
    }
}

void triangleTextured(Vec3f screen[3], Vec2f uv[3], const Model& model, float intensity, Image& img,
                       std::vector<float>& zbuf) {
    const Vec2f a{screen[0][0], screen[0][1]};
    const Vec2f b{screen[1][0], screen[1][1]};
    const Vec2f cc{screen[2][0], screen[2][1]};

    const float minXf = std::min({a[0], b[0], cc[0]});
    const float maxXf = std::max({a[0], b[0], cc[0]});
    const float minYf = std::min({a[1], b[1], cc[1]});
    const float maxYf = std::max({a[1], b[1], cc[1]});

    const int x0 = std::clamp(static_cast<int>(std::floor(minXf)), 0, img.width() - 1);
    const int x1 = std::clamp(static_cast<int>(std::ceil(maxXf)), 0, img.width() - 1);
    const int y0 = std::clamp(static_cast<int>(std::floor(minYf)), 0, img.height() - 1);
    const int y1 = std::clamp(static_cast<int>(std::ceil(maxYf)), 0, img.height() - 1);

    for (int y = y0; y <= y1; ++y) {
        for (int x = x0; x <= x1; ++x) {
            const Vec2f p{static_cast<float>(x), static_cast<float>(y)};
            const Vec3f w = barycentric(a, b, cc, p);
            if (w[0] >= 0 && w[1] >= 0 && w[2] >= 0) {
                const float z = w[0] * screen[0][2] + w[1] * screen[1][2] + w[2] * screen[2][2];
                const std::size_t idx =
                    static_cast<std::size_t>(x) + static_cast<std::size_t>(y) * static_cast<std::size_t>(img.width());
                if (z > zbuf[idx]) {
                    zbuf[idx] = z;
                    const Vec2f texUV = w[0] * uv[0] + w[1] * uv[1] + w[2] * uv[2];
                    const Color base = model.diffuse(texUV);
                    const Color shaded{
                        static_cast<unsigned char>(static_cast<float>(base.r) * intensity),
                        static_cast<unsigned char>(static_cast<float>(base.g) * intensity),
                        static_cast<unsigned char>(static_cast<float>(base.b) * intensity),
                    };
                    img.set(x, y, shaded);
                }
            }
        }
    }
}
