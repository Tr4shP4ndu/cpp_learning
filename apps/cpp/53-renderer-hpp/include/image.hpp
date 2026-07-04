#pragma once
#include <string>
#include <vector>

struct Color { unsigned char r{}, g{}, b{}; };

class Image {
public:
    Image(int w, int h);
    int width()  const { return w_; }
    int height() const { return h_; }
    void  set(int x, int y, Color c);
    Color get(int x, int y) const;                 // clamps to edge (safe for sampling)
    void  writePPM(const std::string& path) const;
    static Image readPPM(const std::string& path); // P6 only; throws std::runtime_error
private:
    int w_, h_;
    std::vector<Color> pixels_;                     // row-major, origin top-left
};
