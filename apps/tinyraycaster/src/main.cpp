// tinyraycaster — a Wolfenstein-style raycasting engine, no deps, PPM out.
// Our own implementation; the tutorial that inspired it is at
// https://github.com/ssloy/tinyraycaster/wiki .
//
// Idea: the world is a 2D grid of walls, but we draw it in fake-3D. For each
// vertical COLUMN of the screen, march a ray across the map until it hits a
// wall; the closer the wall, the taller the slice we draw. Sweep the rays
// across a field of view and the flat grid looks like a first-person corridor.
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main() {
    const int W = 1024, H = 512;
    const double PI = 3.14159265358979323846;

    std::vector<std::uint8_t> fb(static_cast<std::size_t>(W) * H * 3, 0);
    auto put = [&](int x, int y, std::uint8_t r, std::uint8_t g, std::uint8_t b) {
        if (x < 0 || y < 0 || x >= W || y >= H) return;
        std::size_t o = (static_cast<std::size_t>(y) * W + x) * 3;
        fb[o] = r; fb[o + 1] = g; fb[o + 2] = b;
    };

    // The map: '#' generic wall, digits = coloured walls, space = empty. Built
    // in code so the borders and dimensions are always correct.
    const int mapW = 16, mapH = 16;
    std::vector<std::string> world(mapH, std::string(mapW, ' '));
    for (int x = 0; x < mapW; ++x) { world[0][x] = '#'; world[mapH - 1][x] = '#'; }
    for (int y = 0; y < mapH; ++y) { world[y][0] = '#'; world[y][mapW - 1] = '#'; }
    world[4][4] = world[4][5] = world[4][6] = '1';
    world[9][10] = world[10][10] = world[11][10] = '2';
    world[11][4] = world[12][4] = '3';

    auto wall_color = [](char c, std::uint8_t& r, std::uint8_t& g, std::uint8_t& b) {
        switch (c) {
            case '1': r = 190; g = 70;  b = 70;  break;
            case '2': r = 70;  g = 170; b = 90;  break;
            case '3': r = 70;  g = 100; b = 200; break;
            default:  r = 160; g = 160; b = 160; break;
        }
    };

    const double px = 3.5, py = 3.5;   // player position, in map cells
    const double dir = PI / 3.0;       // where the player looks
    const double fov = PI / 3.0;       // 60-degree field of view

    // Background: sky on top, floor on the bottom.
    for (int y = 0; y < H; ++y)
        for (int x = 0; x < W; ++x)
            put(x, y, (y < H / 2) ? 60 : 50, (y < H / 2) ? 90 : 50, (y < H / 2) ? 120 : 50);

    // One ray per screen column.
    for (int col = 0; col < W; ++col) {
        double angle = dir - fov / 2 + fov * col / static_cast<double>(W);
        for (double t = 0; t < 25; t += 0.02) {
            double cx = px + t * std::cos(angle);
            double cy = py + t * std::sin(angle);
            int mx = static_cast<int>(cx), my = static_cast<int>(cy);
            if (mx < 0 || my < 0 || mx >= mapW || my >= mapH) break;
            if (world[my][mx] == ' ') continue;

            double dist = t * std::cos(angle - dir);              // fisheye fix
            int colh = static_cast<int>(H / std::max(dist, 1e-3));
            std::uint8_t r, g, b; wall_color(world[my][mx], r, g, b);
            double shade = 1.0 / (1.0 + dist * dist * 0.04);      // fade with distance
            r = static_cast<std::uint8_t>(r * shade);
            g = static_cast<std::uint8_t>(g * shade);
            b = static_cast<std::uint8_t>(b * shade);
            int y0 = H / 2 - colh / 2, y1 = H / 2 + colh / 2;
            for (int y = std::max(0, y0); y < std::min(H, y1); ++y) put(col, y, r, g, b);
            break;
        }
    }

    // Minimap overlay in the top-left corner (10 px per cell), plus a player dot.
    const int cell = 10;
    for (int my = 0; my < mapH; ++my)
        for (int mx = 0; mx < mapW; ++mx)
            if (world[my][mx] != ' ') {
                std::uint8_t r, g, b; wall_color(world[my][mx], r, g, b);
                for (int yy = 0; yy < cell; ++yy)
                    for (int xx = 0; xx < cell; ++xx)
                        put(mx * cell + xx, my * cell + yy, r, g, b);
            }
    for (int yy = -2; yy <= 2; ++yy)
        for (int xx = -2; xx <= 2; ++xx)
            put(static_cast<int>(px * cell) + xx, static_cast<int>(py * cell) + yy, 255, 255, 0);

    std::ofstream ofs("out.ppm", std::ios::binary);
    ofs << "P6\n" << W << " " << H << "\n255\n";
    ofs.write(reinterpret_cast<const char*>(fb.data()), static_cast<std::streamsize>(fb.size()));
    std::cout << "wrote out.ppm (" << W << "x" << H << ")\n";
    return 0;
}
