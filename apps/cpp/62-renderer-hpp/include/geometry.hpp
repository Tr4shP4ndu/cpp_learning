#pragma once
#include <cmath>

// Fixed-size numeric vector, e.g. Vec<3,float> for a 3D point/direction.
template<int N, typename T>
struct Vec {
    T data[N]{};
    T&       operator[](int i)       { return data[i]; }
    T        operator[](int i) const { return data[i]; }
};

using Vec2f = Vec<2, float>;
using Vec3f = Vec<3, float>;
using Vec4f = Vec<4, float>;

template<int N, typename T>
Vec<N, T> operator+(const Vec<N, T>& a, const Vec<N, T>& b) {
    Vec<N, T> r;
    for (int i = 0; i < N; ++i) r[i] = a[i] + b[i];
    return r;
}

template<int N, typename T>
Vec<N, T> operator-(const Vec<N, T>& a, const Vec<N, T>& b) {
    Vec<N, T> r;
    for (int i = 0; i < N; ++i) r[i] = a[i] - b[i];
    return r;
}

template<int N, typename T>
Vec<N, T> operator*(const Vec<N, T>& v, T s) {
    Vec<N, T> r;
    for (int i = 0; i < N; ++i) r[i] = v[i] * s;
    return r;
}

template<int N, typename T>
Vec<N, T> operator*(T s, const Vec<N, T>& v) {
    return v * s;
}

template<int N, typename T>
T dot(const Vec<N, T>& a, const Vec<N, T>& b) {
    T r{};
    for (int i = 0; i < N; ++i) r += a[i] * b[i];
    return r;
}

inline Vec3f cross(const Vec3f& a, const Vec3f& b) {
    return Vec3f{{a[1] * b[2] - a[2] * b[1],
                  a[2] * b[0] - a[0] * b[2],
                  a[0] * b[1] - a[1] * b[0]}};
}

inline float norm(const Vec3f& v) { return std::sqrt(dot(v, v)); }

inline Vec3f normalize(const Vec3f& v) { return v * (1.0f / norm(v)); }

inline Vec4f embed(const Vec3f& v, float w = 1.0f) {
    return Vec4f{{v[0], v[1], v[2], w}};
}

inline Vec3f proj3(const Vec4f& v) {
    return Vec3f{{v[0] / v[3], v[1] / v[3], v[2] / v[3]}};
}

struct Matrix {
    float m[4][4]{};

    static Matrix identity() {
        Matrix r;
        for (int i = 0; i < 4; ++i) r.m[i][i] = 1.0f;
        return r;
    }
};

inline Matrix operator*(const Matrix& a, const Matrix& b) {
    Matrix r;
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j) {
            float sum = 0.0f;
            for (int k = 0; k < 4; ++k) sum += a.m[i][k] * b.m[k][j];
            r.m[i][j] = sum;
        }
    return r;
}

inline Vec4f operator*(const Matrix& a, const Vec4f& v) {
    Vec4f r;
    for (int i = 0; i < 4; ++i) {
        float sum = 0.0f;
        for (int k = 0; k < 4; ++k) sum += a.m[i][k] * v[k];
        r[i] = sum;
    }
    return r;
}
