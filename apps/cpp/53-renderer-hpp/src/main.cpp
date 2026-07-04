#include "image.hpp"
#include "geometry.hpp"
#include "gl.hpp"
#include <cassert>
#include <cmath>

static void selfCheck() {
    Image t(4, 4);
    t.set(1, 2, Color{10, 20, 30});
    Color c = t.get(1, 2);
    assert(c.r == 10 && c.g == 20 && c.b == 30);
    assert(t.get(-5, -5).r == 0);                  // clamp returns edge pixel (black here)

    Vec3f a{1,0,0}, b{0,1,0};
    assert(dot(a, b) == 0.0f);
    Vec3f v3 = cross(a, b);                        // z = +1
    assert(v3[0]==0 && v3[1]==0 && v3[2]==1);
    assert(std::abs(norm(normalize(Vec3f{3,4,0})) - 1.0f) < 1e-6f);
    Matrix I = Matrix::identity();
    Vec4f v = I * Vec4f{2,3,4,1};
    assert(v[0]==2 && v[1]==3 && v[2]==4 && v[3]==1);

    Image line(10, 10);
    drawLine(0, 0, 9, 0, line, Color{255, 255, 255});
    assert(line.get(9, 0).r == 255);               // last endpoint got drawn

    Vec2f A{0,0}, B{4,0}, C{0,4};
    Vec3f w = barycentric(A, B, C, Vec2f{1,1});     // inside
    assert(w[0]>0 && w[1]>0 && w[2]>0);
    assert(std::abs(w[0]+w[1]+w[2] - 1.0f) < 1e-5f);
    Vec3f wo = barycentric(A, B, C, Vec2f{5,5});    // outside -> a negative weight
    assert(wo[0]<0 || wo[1]<0 || wo[2]<0);
}

int main() {
    selfCheck();
    const int W = 400, H = 400;
    Image img(W, H);

    // One hardcoded 3D triangle (world space).
    const Vec3f v0{100, 100, 0}, v1{300, 150, 0}, v2{200, 350, 0};

    // Project to screen space, flipping Y so +y (up in world/model space)
    // lands correctly on the top-down pixel grid.
    Vec2f screen[3] = {
        Vec2f{v0[0], static_cast<float>(H - 1) - v0[1]},
        Vec2f{v1[0], static_cast<float>(H - 1) - v1[1]},
        Vec2f{v2[0], static_cast<float>(H - 1) - v2[1]},
    };

    // Flat shading: one normal for the whole face, lit by a fixed directional light.
    const Vec3f normal = normalize(cross(v2 - v0, v1 - v0));
    const Vec3f lightDir = normalize(Vec3f{0, 0, -1});
    const float intensity = dot(normal, lightDir);

    if (intensity > 0) {
        const Color base{255, 80, 80};
        const Color shaded{
            static_cast<unsigned char>(base.r * intensity),
            static_cast<unsigned char>(base.g * intensity),
            static_cast<unsigned char>(base.b * intensity),
        };
        triangleFlat(screen, img, shaded);
    }

    img.writePPM("render.ppm");
    return 0;
}
