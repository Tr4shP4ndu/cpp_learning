#include "gl.hpp"
#include <algorithm>
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
