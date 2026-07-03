// image.hpp — an RGB framebuffer you can draw into and save to disk.
#pragma once
#include <cstdint>
#include <string>
#include <vector>

struct Color {
    std::uint8_t r = 0, g = 0, b = 0;
};

class Image {
public:
    Image(int width, int height);

    int width()  const { return width_; }
    int height() const { return height_; }

    // Set a pixel. (0,0) is top-left. Out-of-range coordinates are ignored.
    void set(int x, int y, Color c);

    // Write a binary PPM (P6) file — the simplest image format, no libraries.
    bool writePPM(const std::string& path) const;

private:
    int width_;
    int height_;
    std::vector<Color> pixels_;   // row-major, size = width*height
};
