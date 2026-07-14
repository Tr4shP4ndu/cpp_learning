// main.cpp — assemble a tiny scene and render it, the way a GPU would:
// project 3D -> screen, shade, rasterize with a z-buffer, save the image.
#include <iostream>
#include <limits>
#include <vector>
#include "image.hpp"
#include "render.hpp"
#include "vec.hpp"

// Orthographic projection: model space [-1,1] -> pixel coordinates.
// Y is flipped so +y points up on screen. z (depth) is carried through.
static Vec3f toScreen(Vec3f v, int w, int h) {
    return {(v.x * 0.5f + 0.5f) * (w - 1),
            (1.0f - (v.y * 0.5f + 0.5f)) * (h - 1),
            v.z};
}

// Flat shading: brightness = how much the triangle faces the light.
static Color shade(Vec3f a, Vec3f b, Vec3f c, Vec3f light, Color base) {
    Vec3f n = normalize(cross(b - a, c - a));
    float intensity = dot(n, light);
    if (intensity < 0) intensity = 0;                 // facing away -> dark
    return {static_cast<std::uint8_t>(base.r * intensity),
            static_cast<std::uint8_t>(base.g * intensity),
            static_cast<std::uint8_t>(base.b * intensity)};
}

int main() {
    const int W = 400, H = 400;
    Image img(W, H);

    // z-buffer: one depth per pixel, initialized to "infinitely far away".
    std::vector<float> zbuffer(static_cast<std::size_t>(W) * H,
                               -std::numeric_limits<float>::infinity());

    Vec3f light = normalize({0.0f, 0.0f, 1.0f});  // pointing at the camera

    // Two overlapping triangles at different depths. The green one is in FRONT
    // (larger z), so the z-buffer must draw it over the red one where they meet.
    Vec3f red[3]   = {{-0.6f, -0.4f, 0.2f}, {0.4f, -0.5f, 0.2f}, {-0.1f, 0.5f, 0.2f}};
    Vec3f green[3] = {{-0.4f, -0.5f, 0.6f}, {0.6f, -0.3f, 0.6f}, { 0.2f, 0.6f, 0.6f}};

    auto draw = [&](Vec3f t[3], Color base) {
        Color col = shade(t[0], t[1], t[2], light, base);
        drawTriangle(img, zbuffer,
                     toScreen(t[0], W, H), toScreen(t[1], W, H), toScreen(t[2], W, H),
                     col);
    };
    draw(red,   Color{220, 60, 60});
    draw(green, Color{60, 200, 90});

    // A white wireframe triangle on top, drawn with line primitives.
    Vec3f wire[3] = {{-0.8f, 0.7f, 0}, {0.0f, -0.8f, 0}, {0.8f, 0.7f, 0}};
    Vec3f s0 = toScreen(wire[0], W, H), s1 = toScreen(wire[1], W, H), s2 = toScreen(wire[2], W, H);
    drawLine(img, (int)s0.x, (int)s0.y, (int)s1.x, (int)s1.y, Color{255, 255, 255});
    drawLine(img, (int)s1.x, (int)s1.y, (int)s2.x, (int)s2.y, Color{255, 255, 255});
    drawLine(img, (int)s2.x, (int)s2.y, (int)s0.x, (int)s0.y, Color{255, 255, 255});

    const std::string path = "render.ppm";
    if (!img.writePPM(path)) {
        std::cerr << "failed to write " << path << "\n";
        return 1;
    }
    std::cout << "wrote " << path << " (" << W << "x" << H << ")\n";
    std::cout << "open it with an image viewer, or: magick render.ppm render.png\n";
    return 0;
}
