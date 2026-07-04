#include "image.hpp"
#include "geometry.hpp"
#include "gl.hpp"
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
}

// Projects a world-space triangle (x, y, depth) to screen space, flipping Y
// so +y (up in world/model space) lands correctly on the top-down pixel grid.
// Depth passes through unchanged.
static void toScreen(const Vec3f world[3], int height, Vec3f screen[3]) {
    for (int i = 0; i < 3; ++i) {
        screen[i] = Vec3f{world[i][0], static_cast<float>(height - 1) - world[i][1], world[i][2]};
    }
}

int main() {
    selfCheck();
    const int W = 400, H = 400;
    Image img(W, H);
    std::vector<float> zbuf(static_cast<std::size_t>(W) * static_cast<std::size_t>(H),
                             -std::numeric_limits<float>::infinity());

    // Two overlapping triangles at different depths (z: larger = closer).
    // Red sits behind; green sits in front and should occlude red where the
    // two overlap.
    Vec3f red[3] = {Vec3f{100, 100, 0}, Vec3f{300, 150, 0}, Vec3f{200, 350, 0}};
    Vec3f green[3] = {Vec3f{180, 60, 0.5f}, Vec3f{380, 110, 0.5f}, Vec3f{280, 310, 0.5f}};

    Vec3f redScreen[3];
    Vec3f greenScreen[3];
    toScreen(red, H, redScreen);
    toScreen(green, H, greenScreen);

    // Draw the farther triangle first; the z-test makes draw order irrelevant
    // to the result, but this order also matches "paint the back layer first"
    // intuition.
    triangleFlat(redScreen, img, zbuf, Color{255, 80, 80});
    triangleFlat(greenScreen, img, zbuf, Color{80, 220, 80});

    img.writePPM("render.ppm");
    return 0;
}
