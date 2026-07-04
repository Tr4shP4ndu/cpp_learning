#include "image.hpp"
#include <cassert>

static void selfCheck() {
    Image t(4, 4);
    t.set(1, 2, Color{10, 20, 30});
    Color c = t.get(1, 2);
    assert(c.r == 10 && c.g == 20 && c.b == 30);
    assert(t.get(-5, -5).r == 0);                  // clamp returns edge pixel (black here)
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
