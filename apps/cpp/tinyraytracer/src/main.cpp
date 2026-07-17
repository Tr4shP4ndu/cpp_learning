// tinyraytracer — a ray tracer in a few hundred lines, no dependencies, PPM out.
// Our own implementation of the classic technique; the tutorial that inspired it
// is at https://github.com/ssloy/tinyraytracer/wiki .
//
// Idea: for every pixel, shoot a ray from the camera into the scene, find the
// nearest surface, and compute its colour from lighting, shadows, reflection,
// and refraction (recursively). This is the OPPOSITE of the rasterizer capstone
// (lesson 63): there we projected triangles onto the screen; here we trace
// light backwards from the eye.
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <limits>
#include <vector>

constexpr double kPI = 3.14159265358979323846;

struct Vec3 {
    double x = 0, y = 0, z = 0;
    Vec3() = default;
    Vec3(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {}
    Vec3 operator+(const Vec3& o) const { return {x + o.x, y + o.y, z + o.z}; }
    Vec3 operator-(const Vec3& o) const { return {x - o.x, y - o.y, z - o.z}; }
    Vec3 operator*(double s) const { return {x * s, y * s, z * s}; }
    Vec3 operator-() const { return {-x, -y, -z}; }
    double dot(const Vec3& o) const { return x * o.x + y * o.y + z * o.z; }
    double norm() const { return std::sqrt(dot(*this)); }
    Vec3 normalized() const { double n = norm(); return {x / n, y / n, z / n}; }
};

struct Material {
    Vec3 diffuse{0, 0, 0};
    double albedo[4] = {1, 0, 0, 0};  // weights: diffuse, specular, reflect, refract
    double specular_exponent = 0;
    double refractive_index = 1;
};

struct Sphere {
    Vec3 center;
    double radius;
    Material material;
    // True (and sets t, the hit distance) if the ray orig+t*dir hits the sphere.
    bool ray_intersect(const Vec3& orig, const Vec3& dir, double& t) const {
        Vec3 L = center - orig;
        double tca = L.dot(dir);
        double d2 = L.dot(L) - tca * tca;
        double r2 = radius * radius;
        if (d2 > r2) return false;
        double thc = std::sqrt(r2 - d2);
        t = tca - thc;
        if (t < 1e-3) t = tca + thc;   // origin inside/behind: take the far root
        return t >= 1e-3;
    }
};

struct Light { Vec3 position; double intensity; };

static Vec3 reflect(const Vec3& I, const Vec3& N) {
    return I - N * 2.0 * I.dot(N);
}

// Snell's law. eta_t = index of the material entered; eta_i = the one left.
static Vec3 refract(const Vec3& I, const Vec3& N, double eta_t, double eta_i = 1.0) {
    double cosi = -std::max(-1.0, std::min(1.0, I.dot(N)));
    if (cosi < 0) return refract(I, -N, eta_i, eta_t);   // ray is inside; flip
    double eta = eta_i / eta_t;
    double k = 1 - eta * eta * (1 - cosi * cosi);
    return k < 0 ? Vec3{1, 0, 0} : I * eta + N * (eta * cosi - std::sqrt(k));
}

static bool scene_intersect(const Vec3& orig, const Vec3& dir,
                            const std::vector<Sphere>& spheres,
                            Vec3& hit, Vec3& N, Material& material) {
    double nearest = std::numeric_limits<double>::max();
    for (const Sphere& s : spheres) {
        double dist;
        if (s.ray_intersect(orig, dir, dist) && dist < nearest) {
            nearest = dist;
            hit = orig + dir * dist;
            N = (hit - s.center).normalized();
            material = s.material;
        }
    }
    // A checkerboard floor plane at y = -4.
    if (std::abs(dir.y) > 1e-3) {
        double d = -(orig.y + 4) / dir.y;
        Vec3 pt = orig + dir * d;
        if (d > 1e-3 && d < nearest && std::abs(pt.x) < 10 && pt.z < -10 && pt.z > -30) {
            nearest = d;
            hit = pt;
            N = {0, 1, 0};
            bool white = ((static_cast<int>(0.5 * hit.x + 1000) + static_cast<int>(0.5 * hit.z)) & 1) != 0;
            material = Material{};
            material.diffuse = white ? Vec3{0.3, 0.3, 0.3} : Vec3{0.3, 0.2, 0.1};
        }
    }
    return nearest < 1000;
}

static Vec3 cast_ray(const Vec3& orig, const Vec3& dir,
                     const std::vector<Sphere>& spheres,
                     const std::vector<Light>& lights, int depth = 0) {
    Vec3 hit, N;
    Material material;
    if (depth > 4 || !scene_intersect(orig, dir, spheres, hit, N, material))
        return {0.2, 0.7, 0.8};   // background sky

    // Recurse for reflection and refraction, nudging the origin off the surface.
    Vec3 reflect_dir = reflect(dir, N).normalized();
    Vec3 refract_dir = refract(dir, N, material.refractive_index).normalized();
    Vec3 reflect_orig = reflect_dir.dot(N) < 0 ? hit - N * 1e-3 : hit + N * 1e-3;
    Vec3 refract_orig = refract_dir.dot(N) < 0 ? hit - N * 1e-3 : hit + N * 1e-3;
    Vec3 reflect_color = cast_ray(reflect_orig, reflect_dir, spheres, lights, depth + 1);
    Vec3 refract_color = cast_ray(refract_orig, refract_dir, spheres, lights, depth + 1);

    double diffuse_light = 0, specular_light = 0;
    for (const Light& light : lights) {
        Vec3 light_dir = (light.position - hit).normalized();
        double light_dist = (light.position - hit).norm();

        // Shadow ray: is anything between this point and the light?
        Vec3 shadow_orig = light_dir.dot(N) < 0 ? hit - N * 1e-3 : hit + N * 1e-3;
        Vec3 s_hit, s_N;
        Material s_mat;
        if (scene_intersect(shadow_orig, light_dir, spheres, s_hit, s_N, s_mat) &&
            (s_hit - shadow_orig).norm() < light_dist)
            continue;   // in shadow from this light

        diffuse_light += light.intensity * std::max(0.0, light_dir.dot(N));
        specular_light += std::pow(std::max(0.0, reflect(-light_dir, N).dot(dir)),
                                   material.specular_exponent) * light.intensity;
    }

    return material.diffuse * (diffuse_light * material.albedo[0])
         + Vec3{1, 1, 1} * (specular_light * material.albedo[1])
         + reflect_color * material.albedo[2]
         + refract_color * material.albedo[3];
}

int main() {
    const int width = 1024, height = 768;
    const double fov = kPI / 3.0;

    Material ivory{{0.4, 0.4, 0.3}, {0.6, 0.3, 0.1, 0.0}, 50, 1.0};
    Material glass{{0.6, 0.7, 0.8}, {0.0, 0.5, 0.1, 0.8}, 125, 1.5};
    Material red_rubber{{0.3, 0.1, 0.1}, {0.9, 0.1, 0.0, 0.0}, 10, 1.0};
    Material mirror{{1.0, 1.0, 1.0}, {0.0, 10.0, 0.8, 0.0}, 1425, 1.0};

    std::vector<Sphere> spheres = {
        {{-3.0, 0.0, -16.0}, 2, ivory},
        {{-1.0, -1.5, -12.0}, 2, glass},
        {{1.5, -0.5, -18.0}, 3, red_rubber},
        {{7.0, 5.0, -18.0}, 4, mirror},
    };
    std::vector<Light> lights = {
        {{-20, 20, 20}, 1.5}, {{30, 50, -25}, 1.8}, {{30, 20, 30}, 1.7},
    };

    std::vector<Vec3> framebuffer(static_cast<std::size_t>(width) * height);
    const double aspect = width / static_cast<double>(height);
    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            double px =  (2 * (i + 0.5) / width - 1) * std::tan(fov / 2.0) * aspect;
            double py = -(2 * (j + 0.5) / height - 1) * std::tan(fov / 2.0);
            Vec3 dir = Vec3{px, py, -1}.normalized();
            framebuffer[static_cast<std::size_t>(j) * width + i] =
                cast_ray({0, 0, 0}, dir, spheres, lights);
        }
    }

    // Write a binary PPM (P6), clamping each channel to [0,1] and scaling.
    std::ofstream ofs("out.ppm", std::ios::binary);
    ofs << "P6\n" << width << " " << height << "\n255\n";
    for (const Vec3& c : framebuffer) {
        double m = std::max(c.x, std::max(c.y, c.z));   // tame over-bright highlights
        Vec3 col = m > 1 ? c * (1.0 / m) : c;
        ofs.put(static_cast<char>(255 * std::clamp(col.x, 0.0, 1.0)));
        ofs.put(static_cast<char>(255 * std::clamp(col.y, 0.0, 1.0)));
        ofs.put(static_cast<char>(255 * std::clamp(col.z, 0.0, 1.0)));
    }
    std::cout << "wrote out.ppm (" << width << "x" << height << ")\n";
    return 0;
}
