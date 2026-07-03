#include "image.hpp"
#include <fstream>

Image::Image(int width, int height)
    : width_(width), height_(height), pixels_(static_cast<std::size_t>(width) * height) {}

void Image::set(int x, int y, Color c) {
    if (x < 0 || x >= width_ || y < 0 || y >= height_) return;  // clip
    pixels_[static_cast<std::size_t>(y) * width_ + x] = c;
}

bool Image::writePPM(const std::string& path) const {
    std::ofstream out(path, std::ios::binary);
    if (!out) return false;
    // PPM header: "P6\n<width> <height>\n255\n", then raw RGB bytes.
    out << "P6\n" << width_ << ' ' << height_ << "\n255\n";
    for (const Color& c : pixels_) {
        out.put(static_cast<char>(c.r));
        out.put(static_cast<char>(c.g));
        out.put(static_cast<char>(c.b));
    }
    return static_cast<bool>(out);
}
