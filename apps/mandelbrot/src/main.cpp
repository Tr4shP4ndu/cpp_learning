// mandelbrot — render the Mandelbrot set as ASCII art in the terminal.
// Concepts: nested loops, floating-point math, std::string (lessons 06, 12, 20).
// The same idea as the renderer capstone: turn math into a picture, pixel by pixel.
// Usage:  mandelbrot [width] [height]      (defaults 78 x 28)
#include <iostream>
#include <string>

int main(int argc, char** argv) {
    const int width   = (argc > 1) ? std::stoi(argv[1]) : 78;
    const int height  = (argc > 2) ? std::stoi(argv[2]) : 28;
    const int maxIter = 100;
    const std::string shades = " .:-=+*#%@";  // dark -> dense = more iterations

    for (int py = 0; py < height; ++py) {
        std::string row;
        for (int px = 0; px < width; ++px) {
            // Map the pixel to a point in the complex plane.
            double x0 = (px / static_cast<double>(width)) * 3.5 - 2.5;   // [-2.5, 1.0]
            double y0 = (py / static_cast<double>(height)) * 2.0 - 1.0;  // [-1.0, 1.0]
            double x = 0.0, y = 0.0;
            int iter = 0;
            while (x * x + y * y <= 4.0 && iter < maxIter) {
                double xt = x * x - y * y + x0;   // z = z^2 + c
                y = 2.0 * x * y + y0;
                x = xt;
                ++iter;
            }
            std::size_t idx = static_cast<std::size_t>(iter) * (shades.size() - 1) / maxIter;
            row += shades[idx];
        }
        std::cout << row << '\n';
    }
    return 0;
}
