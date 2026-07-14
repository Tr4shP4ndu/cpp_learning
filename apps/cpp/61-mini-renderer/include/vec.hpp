// vec.hpp — tiny vector math. The data the graphics pipeline moves around.
#pragma once
#include <cmath>

// A 3D float vector (also used as 2D by ignoring z).
struct Vec3f {
    float x = 0, y = 0, z = 0;
};

inline Vec3f operator+(Vec3f a, Vec3f b) { return {a.x + b.x, a.y + b.y, a.z + b.z}; }
inline Vec3f operator-(Vec3f a, Vec3f b) { return {a.x - b.x, a.y - b.y, a.z - b.z}; }
inline Vec3f operator*(Vec3f v, float s) { return {v.x * s, v.y * s, v.z * s}; }

inline float dot(Vec3f a, Vec3f b) { return a.x * b.x + a.y * b.y + a.z * b.z; }

inline Vec3f cross(Vec3f a, Vec3f b) {
    return {a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x};
}

inline Vec3f normalize(Vec3f v) {
    float len = std::sqrt(dot(v, v));
    return (len > 0) ? v * (1.0f / len) : v;
}
