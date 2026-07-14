// tinykaboom — a raymarched, procedurally-noisy fireball. No deps, PPM out.
// Our own implementation; the tutorial that inspired it is at
// https://github.com/ssloy/tinykaboom/wiki .
//
// Idea: instead of intersecting rays with geometry (as the ray TRACER does), we
// "march" each ray forward in steps whose length is the distance to the nearest
// surface — a signed distance function (SDF). The surface here is a sphere whose
// radius is pushed in and out by fractal noise, so it looks like a ball of fire.
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <vector>

constexpr double kPI = 3.14159265358979323846;
constexpr double sphere_radius = 1.5;
constexpr double noise_amplitude = 1.0;

struct Vec3 {
    double x = 0, y = 0, z = 0;
    Vec3() = default;
    Vec3(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {}
    Vec3 operator+(const Vec3& o) const { return {x + o.x, y + o.y, z + o.z}; }
    Vec3 operator-(const Vec3& o) const { return {x - o.x, y - o.y, z - o.z}; }
    Vec3 operator*(double s) const { return {x * s, y * s, z * s}; }
    double dot(const Vec3& o) const { return x * o.x + y * o.y + z * o.z; }
    double norm() const { return std::sqrt(dot(*this)); }
    Vec3 normalized() const { double n = norm(); return {x / n, y / n, z / n}; }
};

static double clamp01(double v) { return std::max(0.0, std::min(1.0, v)); }
static double lerp(double a, double b, double t) { return a + (b - a) * clamp01(t); }
static Vec3   lerp(const Vec3& a, const Vec3& b, double t) { return a + (b - a) * clamp01(t); }

// Cheap hash + value noise + fractal brownian motion (classic demoscene noise).
static double hash(double n) { double t = std::sin(n) * 43758.5453123; return t - std::floor(t); }

static double noise(const Vec3& x) {
    Vec3 p{std::floor(x.x), std::floor(x.y), std::floor(x.z)};
    Vec3 f{x.x - p.x, x.y - p.y, x.z - p.z};
    f = Vec3{f.x * f.x * (3 - 2 * f.x), f.y * f.y * (3 - 2 * f.y), f.z * f.z * (3 - 2 * f.z)};
    double n = p.dot(Vec3{1, 57, 113});
    return lerp(lerp(lerp(hash(n +   0), hash(n +   1), f.x),
                     lerp(hash(n +  57), hash(n +  58), f.x), f.y),
                lerp(lerp(hash(n + 113), hash(n + 114), f.x),
                     lerp(hash(n + 170), hash(n + 171), f.x), f.y), f.z);
}

static Vec3 rotate(const Vec3& v) {   // fixed rotation to decorrelate octaves
    return {Vec3{0.00, 0.80, 0.60}.dot(v),
            Vec3{-0.80, 0.36, -0.48}.dot(v),
            Vec3{-0.60, -0.48, 0.64}.dot(v)};
}

static double fbm(const Vec3& x) {
    Vec3 p = x;
    double f = 0;
    f += 0.5000 * noise(p); p = rotate(p) * 2.32;
    f += 0.2500 * noise(p); p = rotate(p) * 3.03;
    f += 0.1250 * noise(p); p = rotate(p) * 2.61;
    f += 0.0625 * noise(p);
    return f / 0.9375;
}

// Negative inside the fireball, positive outside; zero at the surface.
static double signed_distance(const Vec3& p) {
    double displacement = -fbm(p * 3.4) * noise_amplitude;
    return p.norm() - (sphere_radius + displacement);
}

// March a ray; on a hit, write the surface point into pos.
static bool sphere_trace(const Vec3& orig, const Vec3& dir, Vec3& pos) {
    // Skip rays that miss the fireball's bounding sphere entirely.
    if (orig.dot(orig) - std::pow(orig.dot(dir), 2) >
        std::pow(sphere_radius + noise_amplitude, 2))
        return false;
    pos = orig;
    for (int i = 0; i < 128; ++i) {
        double d = signed_distance(pos);
        if (d < 0) return true;
        pos = pos + dir * std::max(d * 0.1, 0.01);
    }
    return false;
}

static Vec3 surface_normal(const Vec3& pos) {   // numerical gradient of the SDF
    const double eps = 0.1;
    double d = signed_distance(pos);
    return Vec3{signed_distance(pos + Vec3{eps, 0, 0}) - d,
                signed_distance(pos + Vec3{0, eps, 0}) - d,
                signed_distance(pos + Vec3{0, 0, eps}) - d}.normalized();
}

static Vec3 fire_palette(double d) {
    const Vec3 yellow{1.7, 1.3, 1.0}, orange{1.0, 0.6, 0.0}, red{1.0, 0.0, 0.0},
               dark{0.2, 0.2, 0.2}, gray{0.4, 0.4, 0.4};
    double x = clamp01(d);
    if (x < 0.25) return lerp(gray, dark, x * 4);
    if (x < 0.50) return lerp(dark, red, x * 4 - 1);
    if (x < 0.75) return lerp(red, orange, x * 4 - 2);
    return lerp(orange, yellow, x * 4 - 3);
}

int main() {
    const int W = 640, H = 480;
    const double fov = kPI / 3.0;
    std::vector<Vec3> framebuffer(static_cast<std::size_t>(W) * H);

    for (int j = 0; j < H; ++j) {
        for (int i = 0; i < W; ++i) {
            double dx =  (i + 0.5) - W / 2.0;
            double dy = -(j + 0.5) + H / 2.0;
            double dz = -H / (2.0 * std::tan(fov / 2.0));
            Vec3 dir = Vec3{dx, dy, dz}.normalized();
            Vec3 hit;
            Vec3 color{0.05, 0.05, 0.10};   // night-sky background
            if (sphere_trace(Vec3{0, 0, 3}, dir, hit)) {
                Vec3 light_dir = (Vec3{10, 10, 10} - hit).normalized();
                double light = std::max(0.4, light_dir.dot(surface_normal(hit)));
                double heat = (sphere_radius - hit.norm()) / noise_amplitude;
                color = fire_palette((-0.2 + heat) * 2.0) * light;
            }
            framebuffer[static_cast<std::size_t>(j) * W + i] = color;
        }
    }

    std::ofstream ofs("out.ppm", std::ios::binary);
    ofs << "P6\n" << W << " " << H << "\n255\n";
    for (const Vec3& c : framebuffer) {
        ofs.put(static_cast<char>(255 * clamp01(c.x)));
        ofs.put(static_cast<char>(255 * clamp01(c.y)));
        ofs.put(static_cast<char>(255 * clamp01(c.z)));
    }
    std::cout << "wrote out.ppm (" << W << "x" << H << ")\n";
    return 0;
}
