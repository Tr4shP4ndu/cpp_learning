#include "image.hpp"
#include <algorithm>
#include <fstream>
#include <stdexcept>

Image::Image(int w, int h)
    : w_(w), h_(h), pixels_(static_cast<std::size_t>(w) * static_cast<std::size_t>(h)) {}

void Image::set(int x, int y, Color c) {
    if (x < 0 || x >= w_ || y < 0 || y >= h_) return; // out-of-range: ignore
    pixels_[static_cast<std::size_t>(y) * static_cast<std::size_t>(w_) + static_cast<std::size_t>(x)] = c;
}

Color Image::get(int x, int y) const {
    const int cx = std::clamp(x, 0, w_ - 1);
    const int cy = std::clamp(y, 0, h_ - 1);
    return pixels_[static_cast<std::size_t>(cy) * static_cast<std::size_t>(w_) + static_cast<std::size_t>(cx)];
}

void Image::writePPM(const std::string& path) const {
    std::ofstream out(path, std::ios::binary);
    if (!out) throw std::runtime_error("Image::writePPM: cannot open " + path);
    out << "P6\n" << w_ << ' ' << h_ << "\n255\n";
    for (const Color& c : pixels_) {
        out.put(static_cast<char>(c.r));
        out.put(static_cast<char>(c.g));
        out.put(static_cast<char>(c.b));
    }
}

Image Image::readPPM(const std::string& path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) throw std::runtime_error("Image::readPPM: cannot open " + path);

    std::string magic;
    int w = 0, h = 0, maxval = 0;
    in >> magic >> w >> h >> maxval;
    if (!in || magic != "P6") throw std::runtime_error("Image::readPPM: not a P6 file: " + path);
    if (w <= 0 || h <= 0) throw std::runtime_error("Image::readPPM: bad dimensions: " + path);
    if (maxval != 255) throw std::runtime_error("Image::readPPM: only 8-bit (maxval 255) P6 is supported: " + path);
    in.get(); // the single whitespace byte separating the header from pixel data

    Image img(w, h);
    const auto bytes = static_cast<std::streamsize>(img.pixels_.size() * 3);
    in.read(reinterpret_cast<char*>(img.pixels_.data()), bytes);
    if (!in) throw std::runtime_error("Image::readPPM: truncated pixel data: " + path);
    return img;
}
