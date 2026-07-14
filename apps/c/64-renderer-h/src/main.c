#include "geometry.h"
#include "image.h"
#include "model.h"
#include "gl.h"
#include "shader.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Numerically-checkable invariants, asserted at startup (Debug build). Mirrors
// 53's selfCheck(). Every Model/Image created here is freed so the run stays
// leak-clean under the sanitizer.
static void selfCheck(void) {
    Image t = img_create(4, 4);
    img_set(&t, 1, 2, (Color){10, 20, 30});
    Color c = img_get(&t, 1, 2);
    assert(c.r == 10 && c.g == 20 && c.b == 30);
    assert(img_get(&t, -5, -5).r == 0);  // clamp returns the (black) edge pixel
    img_free(&t);

    Vec3f a = {1, 0, 0}, b = {0, 1, 0};
    assert(vec3_dot(a, b) == 0.0f);
    Vec3f cr = vec3_cross(a, b);
    assert(cr.x == 0 && cr.y == 0 && cr.z == 1);
    assert(fabsf(vec3_norm(vec3_normalize((Vec3f){3, 4, 0})) - 1.0f) < 1e-6f);
    Mat4 I = mat4_identity();
    Vec4f v = mat4_mulv(I, (Vec4f){2, 3, 4, 1});
    assert(v.x == 2 && v.y == 3 && v.z == 4 && v.w == 1);

    Image line = img_create(10, 10);
    gl_line(0, 0, 9, 0, &line, (Color){255, 255, 255});
    assert(img_get(&line, 9, 0).r == 255);
    img_free(&line);

    Vec2f A = {0, 0}, B = {4, 0}, C = {0, 4};
    Vec3f w = gl_barycentric(A, B, C, (Vec2f){1, 1});
    assert(w.x > 0 && w.y > 0 && w.z > 0);
    assert(fabsf(w.x + w.y + w.z - 1.0f) < 1e-5f);
    Vec3f wo = gl_barycentric(A, B, C, (Vec2f){5, 5});
    assert(wo.x < 0 || wo.y < 0 || wo.z < 0);

    Model cube = model_cube();
    assert(model_nfaces(&cube) == 12);
    model_free(&cube);

    Model bare = model_cube();
    Color chk = model_diffuse(&bare, (Vec2f){0.1f, 0.1f});
    assert((chk.r == 200 && chk.g == 200 && chk.b == 200) ||
           (chk.r == 60 && chk.g == 60 && chk.b == 60));
    Image white = img_create(2, 2);
    for (int i = 0; i < 4; ++i) white.px[i] = (Color){255, 255, 255};
    model_set_diffuse(&bare, &white);
    Color s = model_diffuse(&bare, (Vec2f){0.5f, 0.5f});
    assert(s.r == 255 && s.g == 255 && s.b == 255);
    img_free(&white);
    model_free(&bare);

    Mat4 p0 = gl_projection(0.0f), id = mat4_identity();
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j) assert(fabsf(p0.m[i][j] - id.m[i][j]) < 1e-6f);

    // viewport(0,0,400,400): NDC (-1,-1,-1)->(0,400,0), (1,1,1)->(400,0,255).
    // (Depth 0 = farthest, 255 = nearest — this renderer is larger-z-nearer.)
    Mat4 vpm = gl_viewport(0, 0, 400, 400);
    Vec3f lo = vec4_proj3(mat4_mulv(vpm, vec4_embed((Vec3f){-1, -1, -1}, 1.0f)));
    Vec3f hi = vec4_proj3(mat4_mulv(vpm, vec4_embed((Vec3f){1, 1, 1}, 1.0f)));
    assert(fabsf(lo.x - 0.0f) < 1e-4f && fabsf(lo.y - 400.0f) < 1e-4f && fabsf(lo.z - 0.0f) < 1e-4f);
    assert(fabsf(hi.x - 400.0f) < 1e-4f && fabsf(hi.y - 0.0f) < 1e-4f && fabsf(hi.z - 255.0f) < 1e-4f);

    Mat4 view = gl_lookat((Vec3f){1, 1, 3}, (Vec3f){0, 0, 0}, (Vec3f){0, 1, 0});
    Vec3f r0 = {view.m[0][0], view.m[0][1], view.m[0][2]};
    Vec3f r1 = {view.m[1][0], view.m[1][1], view.m[1][2]};
    Vec3f r2 = {view.m[2][0], view.m[2][1], view.m[2][2]};
    assert(fabsf(vec3_norm(r0) - 1.0f) < 1e-5f && fabsf(vec3_norm(r1) - 1.0f) < 1e-5f &&
           fabsf(vec3_norm(r2) - 1.0f) < 1e-5f);
    assert(fabsf(vec3_dot(r0, r1)) < 1e-5f && fabsf(vec3_dot(r1, r2)) < 1e-5f &&
           fabsf(vec3_dot(r0, r2)) < 1e-5f);

    Model bareNM = model_cube();
    Vec3f n = model_normalmap(&bareNM, (Vec2f){0.3f, 0.7f});
    assert(n.x == 0.0f && n.y == 0.0f && n.z == 1.0f);
    Image flat = img_create(2, 2);
    for (int i = 0; i < 4; ++i) flat.px[i] = (Color){128, 128, 255};
    model_set_normalmap(&bareNM, &flat);
    Vec3f nn = model_normalmap(&bareNM, (Vec2f){0.5f, 0.5f});
    float tol = 1.0f / 255.0f * 2.0f;
    assert(fabsf(nn.x) < tol && fabsf(nn.y) < tol && fabsf(nn.z - 1.0f) < tol);
    img_free(&flat);
    model_free(&bareNM);
}

int main(int argc, char** argv) {
    selfCheck();
    const int W = 400, H = 400;
    Image img = img_create(W, H);
    float* zbuf = (float*)malloc((size_t)W * (size_t)H * sizeof(float));
    for (size_t i = 0; i < (size_t)W * (size_t)H; ++i) zbuf[i] = -INFINITY;

    // --- Model + optional textures from the command line ---
    // (full arg list in assets/tinyrenderer/README.md)
    //   argv[1]  model:  "cube" (default) | "sphere" | path to a .obj
    //   argv[2]  shader: "flat" | "gouraud" | "phong" | "normal" (default)
    //   argv[3]  P6 PPM diffuse texture   ("-" keeps the procedural checkerboard)
    //   argv[4]  P6 PPM tangent-space normal map (else a flat, no-op map)
    const char* modelArg = (argc > 1) ? argv[1] : "cube";
    Model model;
    if (strcmp(modelArg, "cube") == 0) {
        model = model_cube();
    } else if (strcmp(modelArg, "sphere") == 0) {
        model = model_sphere(24, 24);
    } else if (model_load_obj(&model, modelArg) != 0) {
        fprintf(stderr, "64-renderer-h: cannot load model: %s\n", modelArg);
        img_free(&img);
        free(zbuf);
        return 1;
    }

    if (argc > 3 && strcmp(argv[3], "-") != 0) {
        Image tex;
        if (img_read_ppm(&tex, argv[3]) != 0) {
            fprintf(stderr, "64-renderer-h: cannot load diffuse: %s\n", argv[3]);
            model_free(&model); img_free(&img); free(zbuf);
            return 1;
        }
        model_set_diffuse(&model, &tex);
        img_free(&tex);
    }
    if (argc > 4 && strcmp(argv[4], "-") != 0) {
        Image nm;
        if (img_read_ppm(&nm, argv[4]) != 0) {
            fprintf(stderr, "64-renderer-h: cannot load normal map: %s\n", argv[4]);
            model_free(&model); img_free(&img); free(zbuf);
            return 1;
        }
        model_set_normalmap(&model, &nm);
        img_free(&nm);
    }
    const char* shaderName = (argc > 2) ? argv[2] : "normal";

    // Camera: 3/4 view with perspective foreshortening. coeff = -1/eye.z.
    Vec3f eye = {1.0f, 1.0f, 3.0f};
    Vec3f center = {0.0f, 0.0f, 0.0f};
    Vec3f up = {0.0f, 1.0f, 0.0f};
    float coeff = -1.0f / eye.z;

    // Autofit any model: scale its largest |coordinate| to ~0.6 so it lands
    // inside the [-1,1] NDC box (a unit ±1 cube -> 0.6, as in 53). Uniform scale,
    // so normals are untouched.
    float maxExtent = 1e-6f;
    for (int f = 0; f < model_nfaces(&model); ++f)
        for (int k = 0; k < 3; ++k) {
            Vec3f vv = model_vert(&model, f, k);
            maxExtent = fmaxf(maxExtent, fmaxf(fabsf(vv.x), fmaxf(fabsf(vv.y), fabsf(vv.z))));
        }
    float modelScale = 0.6f / maxExtent;

    Mat4 view = gl_lookat(eye, center, up);
    Mat4 proj = gl_projection(coeff);
    Mat4 scale = mat4_identity();
    scale.m[0][0] = scale.m[1][1] = scale.m[2][2] = modelScale;
    Mat4 mvp = mat4_mul(mat4_mul(proj, view), scale);

    Vec3f light = vec3_normalize((Vec3f){0.3f, 0.5f, 1.0f});

    // One shader's state is live per run; a union avoids declaring the unused
    // three (which would trip -Wunused-variable).
    union {
        FlatState f;
        GouraudState g;
        PhongState p;
        NormalMapState n;
    } st;
    Shader sh;
    if (strcmp(shaderName, "flat") == 0) {
        sh = flat_shader(&st.f, &model, mvp, light);
    } else if (strcmp(shaderName, "gouraud") == 0) {
        sh = gouraud_shader(&st.g, &model, mvp, light);
    } else if (strcmp(shaderName, "phong") == 0) {
        sh = phong_shader(&st.p, &model, mvp, light);
    } else {
        sh = normalmap_shader(&st.n, &model, mvp, light);  // default
    }

    for (int f = 0; f < model_nfaces(&model); ++f) {
        Vec4f clip[3];
        for (int k = 0; k < 3; ++k) clip[k] = sh.vertex(&sh, f, k);
        gl_triangle(clip, &sh, &img, zbuf);
    }

    img_write_ppm(&img, "render.ppm");

    model_free(&model);
    img_free(&img);
    free(zbuf);
    return 0;
}
