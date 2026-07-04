#include "image.hpp"
#include "geometry.hpp"
#include "gl.hpp"
#include "model.hpp"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <limits>
#include <vector>

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

    assert(Model::cube().nfaces() == 12);           // 6 faces * 2 triangles
}

// Rotation matrices about the Y and X axes (right-handed, radians).
static Matrix rotateY(float angle) {
    Matrix r = Matrix::identity();
    const float c = std::cos(angle), s = std::sin(angle);
    r.m[0][0] = c;  r.m[0][2] = s;
    r.m[2][0] = -s; r.m[2][2] = c;
    return r;
}

static Matrix rotateX(float angle) {
    Matrix r = Matrix::identity();
    const float c = std::cos(angle), s = std::sin(angle);
    r.m[1][1] = c;  r.m[1][2] = -s;
    r.m[2][1] = s;  r.m[2][2] = c;
    return r;
}

// Rotates a model-space point/direction by `rot`, then maps it to screen
// space: scale to pixel size, recenter on the image, and flip Y so +y (up in
// model space) lands correctly on the top-down pixel grid. Depth (z) is left
// unscaled-but-monotonic: triangleFlat only needs "larger = closer".
static Vec3f toScreen(const Vec3f& modelPoint, const Matrix& rot, float scale, int width, int height) {
    const Vec3f r = proj3(rot * embed(modelPoint));
    const float sx = r[0] * scale + static_cast<float>(width) / 2.0f;
    const float sy = r[1] * scale + static_cast<float>(height) / 2.0f;
    return Vec3f{sx, static_cast<float>(height - 1) - sy, r[2]};
}

int main() {
    selfCheck();
    const int W = 400, H = 400;
    Image img(W, H);
    std::vector<float> zbuf(static_cast<std::size_t>(W) * static_cast<std::size_t>(H),
                             -std::numeric_limits<float>::infinity());

    const Model model = Model::cube();
    // A 3/4 view so three cube faces are visible instead of a flat square.
    const Matrix rot = rotateY(0.6f) * rotateX(0.35f);
    const float scale = 100.0f;
    const Vec3f lightDir = normalize(Vec3f{0.3f, 0.5f, 1.0f});

    for (int f = 0; f < model.nfaces(); ++f) {
        Vec3f screen[3];
        Vec3f normalSum{0, 0, 0};
        for (int k = 0; k < 3; ++k) {
            screen[k] = toScreen(model.vert(f, k), rot, scale, W, H);
            normalSum = normalSum + proj3(rot * embed(model.normal(f, k)));
        }
        // Flat shading: rotate the (averaged) face normal along with the
        // mesh, then Lambert-light it. Rotation is orthogonal so it needs no
        // inverse-transpose to keep normals correct.
        const float intensity = std::clamp(dot(normalize(normalSum), lightDir), 0.0f, 1.0f);
        const auto shade = static_cast<unsigned char>(intensity * 255.0f);

        triangleFlat(screen, img, zbuf, Color{shade, shade, shade});
    }

    img.writePPM("render.ppm");
    return 0;
}
