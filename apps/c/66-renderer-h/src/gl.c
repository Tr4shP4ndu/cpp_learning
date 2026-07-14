#include "gl.h"
#include "shader.h"
#include <math.h>
#include <stdlib.h>

Mat4 gl_viewport(int x, int y, int w, int h) {
    Mat4 r = mat4_identity();
    r.m[0][0] = (float)w / 2.0f;
    r.m[0][3] = (float)x + (float)w / 2.0f;
    // Negated (vs. textbook +h/2) to fold in the Y-flip (see 53's gl.hpp note).
    r.m[1][1] = -(float)h / 2.0f;
    r.m[1][3] = (float)y + (float)h / 2.0f;
    r.m[2][2] = 255.0f / 2.0f;
    r.m[2][3] = 255.0f / 2.0f;
    return r;
}

Mat4 gl_projection(float coeff) {
    Mat4 r = mat4_identity();
    r.m[3][2] = coeff;
    return r;
}

Mat4 gl_lookat(Vec3f eye, Vec3f center, Vec3f up) {
    Vec3f z = vec3_normalize(vec3_sub(eye, center));
    Vec3f x = vec3_normalize(vec3_cross(up, z));
    Vec3f y = vec3_cross(z, x);

    Mat4 minv = mat4_identity();
    Mat4 tr = mat4_identity();
    minv.m[0][0] = x.x; minv.m[0][1] = x.y; minv.m[0][2] = x.z;
    minv.m[1][0] = y.x; minv.m[1][1] = y.y; minv.m[1][2] = y.z;
    minv.m[2][0] = z.x; minv.m[2][1] = z.y; minv.m[2][2] = z.z;
    tr.m[0][3] = -center.x; tr.m[1][3] = -center.y; tr.m[2][3] = -center.z;
    return mat4_mul(minv, tr);
}

void gl_line(int x0, int y0, int x1, int y1, Image* img, Color c) {
    int steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep) { int t = x0; x0 = y0; y0 = t; t = x1; x1 = y1; y1 = t; }
    if (x0 > x1) { int t = x0; x0 = x1; x1 = t; t = y0; y0 = y1; y1 = t; }

    int dx = x1 - x0;
    int dy = abs(y1 - y0);
    int ystep = (y0 < y1) ? 1 : -1;
    int error = dx / 2;
    int y = y0;
    for (int x = x0; x <= x1; ++x) {
        if (steep) img_set(img, y, x, c);
        else       img_set(img, x, y, c);
        error -= dy;
        if (error < 0) { y += ystep; error += dx; }
    }
}

Vec3f gl_barycentric(Vec2f a, Vec2f b, Vec2f c, Vec2f p) {
    Vec3f u = vec3_cross((Vec3f){c.x - a.x, b.x - a.x, a.x - p.x},
                         (Vec3f){c.y - a.y, b.y - a.y, a.y - p.y});
    if (fabsf(u.z) < 1.0f) return (Vec3f){-1, 1, 1};
    return (Vec3f){1.0f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z};
}

static int clampi(int v, int lo, int hi) {
    return v < lo ? lo : (v > hi ? hi : v);
}

void gl_triangle(const Vec4f clip[3], Shader* sh, Image* img, float* zbuf) {
    // Perspective divide (clip -> NDC) then viewport (NDC -> screen, depth 0..255).
    Mat4 vp = gl_viewport(0, 0, img->w, img->h);
    Vec3f screen[3];
    for (int i = 0; i < 3; ++i)
        screen[i] = vec4_proj3(mat4_mulv(vp, vec4_embed(vec4_proj3(clip[i]), 1.0f)));

    Vec2f a = {screen[0].x, screen[0].y};
    Vec2f b = {screen[1].x, screen[1].y};
    Vec2f cc = {screen[2].x, screen[2].y};

    float minXf = fminf(a.x, fminf(b.x, cc.x));
    float maxXf = fmaxf(a.x, fmaxf(b.x, cc.x));
    float minYf = fminf(a.y, fminf(b.y, cc.y));
    float maxYf = fmaxf(a.y, fmaxf(b.y, cc.y));

    int x0 = clampi((int)floorf(minXf), 0, img->w - 1);
    int x1 = clampi((int)ceilf(maxXf), 0, img->w - 1);
    int y0 = clampi((int)floorf(minYf), 0, img->h - 1);
    int y1 = clampi((int)ceilf(maxYf), 0, img->h - 1);

    for (int y = y0; y <= y1; ++y) {
        for (int x = x0; x <= x1; ++x) {
            Vec3f w = gl_barycentric(a, b, cc, (Vec2f){(float)x, (float)y});
            if (w.x >= 0 && w.y >= 0 && w.z >= 0) {
                float z = w.x * screen[0].z + w.y * screen[1].z + w.z * screen[2].z;
                size_t idx = (size_t)x + (size_t)y * (size_t)img->w;
                if (z > zbuf[idx]) {
                    Color color;
                    if (!sh->fragment(sh, w, &color)) {
                        zbuf[idx] = z;
                        img_set(img, x, y, color);
                    }
                }
            }
        }
    }
}
