#include "geometry.h"
#include <math.h>

Vec3f vec3_add(Vec3f a, Vec3f b)   { return (Vec3f){a.x + b.x, a.y + b.y, a.z + b.z}; }
Vec3f vec3_sub(Vec3f a, Vec3f b)   { return (Vec3f){a.x - b.x, a.y - b.y, a.z - b.z}; }
Vec3f vec3_scale(Vec3f v, float s) { return (Vec3f){v.x * s, v.y * s, v.z * s}; }
float vec3_dot(Vec3f a, Vec3f b)   { return a.x * b.x + a.y * b.y + a.z * b.z; }

Vec3f vec3_cross(Vec3f a, Vec3f b) {
    return (Vec3f){a.y * b.z - a.z * b.y,
                   a.z * b.x - a.x * b.z,
                   a.x * b.y - a.y * b.x};
}

float vec3_norm(Vec3f v)      { return sqrtf(vec3_dot(v, v)); }
Vec3f vec3_normalize(Vec3f v) { return vec3_scale(v, 1.0f / vec3_norm(v)); }

Vec4f vec4_embed(Vec3f v, float w) { return (Vec4f){v.x, v.y, v.z, w}; }
Vec3f vec4_proj3(Vec4f v)          { return (Vec3f){v.x / v.w, v.y / v.w, v.z / v.w}; }

Mat4 mat4_identity(void) {
    Mat4 r = {{{0}}};
    for (int i = 0; i < 4; ++i) r.m[i][i] = 1.0f;
    return r;
}

Mat4 mat4_mul(Mat4 a, Mat4 b) {
    Mat4 r = {{{0}}};
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j) {
            float sum = 0.0f;
            for (int k = 0; k < 4; ++k) sum += a.m[i][k] * b.m[k][j];
            r.m[i][j] = sum;
        }
    return r;
}

Vec4f mat4_mulv(Mat4 a, Vec4f v) {
    float in[4]  = {v.x, v.y, v.z, v.w};
    float out[4] = {0, 0, 0, 0};
    for (int i = 0; i < 4; ++i) {
        float sum = 0.0f;
        for (int k = 0; k < 4; ++k) sum += a.m[i][k] * in[k];
        out[i] = sum;
    }
    return (Vec4f){out[0], out[1], out[2], out[3]};
}
