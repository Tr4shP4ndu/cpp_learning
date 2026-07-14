#include "shader.h"
#include <math.h>

static float clampf(float v, float lo, float hi) {
    return v < lo ? lo : (v > hi ? hi : v);
}

static Color shade(Color base, float intensity) {
    return (Color){
        (unsigned char)((float)base.r * intensity),
        (unsigned char)((float)base.g * intensity),
        (unsigned char)((float)base.b * intensity),
    };
}

static Vec2f interp_uv(const Vec2f uv[3], Vec3f bary) {
    return (Vec2f){
        bary.x * uv[0].x + bary.y * uv[1].x + bary.z * uv[2].x,
        bary.x * uv[0].y + bary.y * uv[1].y + bary.z * uv[2].y,
    };
}

// Lambert intensity of a (not-necessarily-unit) normal against a light dir.
static float lambert(Vec3f n, Vec3f light) {
    return clampf(vec3_dot(vec3_normalize(n), light), 0.0f, 1.0f);
}

static Vec3f interp_normal(const Vec3f n[3], Vec3f bary) {
    return vec3_add(vec3_add(vec3_scale(n[0], bary.x), vec3_scale(n[1], bary.y)),
                    vec3_scale(n[2], bary.z));
}

// --- FlatShader ---

static Vec4f flat_vertex(Shader* sh, int face, int nth) {
    FlatState* s = (FlatState*)sh->state;
    if (nth == 0) s->normalSum = (Vec3f){0, 0, 0};
    s->normalSum = vec3_add(s->normalSum, model_normal(s->model, face, nth));
    s->varyingUV[nth] = model_uv(s->model, face, nth);
    if (nth == 2) s->intensity = lambert(s->normalSum, s->light);
    return mat4_mulv(s->mvp, vec4_embed(model_vert(s->model, face, nth), 1.0f));
}

static int flat_fragment(Shader* sh, Vec3f bary, Color* out) {
    FlatState* s = (FlatState*)sh->state;
    *out = shade(model_diffuse(s->model, interp_uv(s->varyingUV, bary)), s->intensity);
    return 0;
}

Shader flat_shader(FlatState* s, const Model* model, Mat4 mvp, Vec3f light) {
    s->model = model; s->mvp = mvp; s->light = light;
    s->normalSum = (Vec3f){0, 0, 0}; s->intensity = 0.0f;
    Shader sh = {s, flat_vertex, flat_fragment};
    return sh;
}

// --- GouraudShader ---

static Vec4f gouraud_vertex(Shader* sh, int face, int nth) {
    GouraudState* s = (GouraudState*)sh->state;
    s->varyingUV[nth] = model_uv(s->model, face, nth);
    s->varyingIntensity[nth] = lambert(model_normal(s->model, face, nth), s->light);
    return mat4_mulv(s->mvp, vec4_embed(model_vert(s->model, face, nth), 1.0f));
}

static int gouraud_fragment(Shader* sh, Vec3f bary, Color* out) {
    GouraudState* s = (GouraudState*)sh->state;
    float intensity = bary.x * s->varyingIntensity[0] +
                      bary.y * s->varyingIntensity[1] +
                      bary.z * s->varyingIntensity[2];
    *out = shade(model_diffuse(s->model, interp_uv(s->varyingUV, bary)), intensity);
    return 0;
}

Shader gouraud_shader(GouraudState* s, const Model* model, Mat4 mvp, Vec3f light) {
    s->model = model; s->mvp = mvp; s->light = light;
    Shader sh = {s, gouraud_vertex, gouraud_fragment};
    return sh;
}

// --- PhongShader ---

static Vec4f phong_vertex(Shader* sh, int face, int nth) {
    PhongState* s = (PhongState*)sh->state;
    s->varyingUV[nth] = model_uv(s->model, face, nth);
    s->varyingNormal[nth] = model_normal(s->model, face, nth);
    return mat4_mulv(s->mvp, vec4_embed(model_vert(s->model, face, nth), 1.0f));
}

static int phong_fragment(Shader* sh, Vec3f bary, Color* out) {
    PhongState* s = (PhongState*)sh->state;
    Vec3f n = interp_normal(s->varyingNormal, bary);
    *out = shade(model_diffuse(s->model, interp_uv(s->varyingUV, bary)), lambert(n, s->light));
    return 0;
}

Shader phong_shader(PhongState* s, const Model* model, Mat4 mvp, Vec3f light) {
    s->model = model; s->mvp = mvp; s->light = light;
    Shader sh = {s, phong_vertex, phong_fragment};
    return sh;
}

// --- NormalMapShader ---

static Vec4f normalmap_vertex(Shader* sh, int face, int nth) {
    NormalMapState* s = (NormalMapState*)sh->state;
    s->varyingUV[nth] = model_uv(s->model, face, nth);
    s->varyingNormal[nth] = model_normal(s->model, face, nth);
    s->varyingPos[nth] = model_vert(s->model, face, nth);  // model==world (1.8 invariant)
    return mat4_mulv(s->mvp, vec4_embed(model_vert(s->model, face, nth), 1.0f));
}

static int normalmap_fragment(Shader* sh, Vec3f bary, Color* out) {
    NormalMapState* s = (NormalMapState*)sh->state;
    Vec3f N = vec3_normalize(interp_normal(s->varyingNormal, bary));

    // Triangle edges (world space) + matching UV gradients; solve for the
    // tangent T and bitangent B (same 2x2 UV-matrix inverse as 53's C++ shader).
    Vec3f e1 = vec3_sub(s->varyingPos[1], s->varyingPos[0]);
    Vec3f e2 = vec3_sub(s->varyingPos[2], s->varyingPos[0]);
    float du1 = s->varyingUV[1].x - s->varyingUV[0].x;
    float dv1 = s->varyingUV[1].y - s->varyingUV[0].y;
    float du2 = s->varyingUV[2].x - s->varyingUV[0].x;
    float dv2 = s->varyingUV[2].y - s->varyingUV[0].y;
    float det = du1 * dv2 - du2 * dv1;

    Vec3f n_t = model_normalmap(s->model, interp_uv(s->varyingUV, bary));
    Vec3f n_world;
    if (fabsf(det) < 1e-8f) {
        n_world = N;  // degenerate UVs: no tangent frame, fall back to smooth N
    } else {
        float inv = 1.0f / det;
        Vec3f T = vec3_scale(vec3_sub(vec3_scale(e1, dv2), vec3_scale(e2, dv1)), inv);
        // Gram-Schmidt orthonormalize T against N, then B = N x T.
        T = vec3_normalize(vec3_sub(T, vec3_scale(N, vec3_dot(T, N))));
        Vec3f B = vec3_cross(N, T);
        n_world = vec3_normalize(vec3_add(vec3_add(vec3_scale(T, n_t.x), vec3_scale(B, n_t.y)),
                                          vec3_scale(N, n_t.z)));
    }

    float intensity = clampf(vec3_dot(n_world, s->light), 0.0f, 1.0f);
    *out = shade(model_diffuse(s->model, interp_uv(s->varyingUV, bary)), intensity);
    return 0;
}

Shader normalmap_shader(NormalMapState* s, const Model* model, Mat4 mvp, Vec3f light) {
    s->model = model; s->mvp = mvp; s->light = light;
    Shader sh = {s, normalmap_vertex, normalmap_fragment};
    return sh;
}
