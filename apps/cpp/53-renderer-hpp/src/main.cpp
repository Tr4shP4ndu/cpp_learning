#include "image.hpp"
#include "geometry.hpp"
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
}

int main() {
    selfCheck();
    const int W = 400, H = 400;
    Image img(W, H);
    for (int x = 50; x < 150; ++x)
        for (int y = 50; y < 150; ++y)
            img.set(x, y, Color{255, 128, 0});     // an orange square
    img.writePPM("render.ppm");
    return 0;
}
