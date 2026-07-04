#include "shader.hpp"
#include <algorithm>
#include <cmath>

namespace {

// Scales a colour's channels by a [0,1] light intensity (Lambert dimming).
Color shade(Color base, float intensity) {
    return Color{
        static_cast<unsigned char>(static_cast<float>(base.r) * intensity),
        static_cast<unsigned char>(static_cast<float>(base.g) * intensity),
        static_cast<unsigned char>(static_cast<float>(base.b) * intensity),
    };
}

// Barycentric interpolation of the three per-corner UVs.
Vec2f interpUV(const Vec2f uv[3], Vec3f bary) {
    return bary[0] * uv[0] + bary[1] * uv[1] + bary[2] * uv[2];
}

// Lambert intensity of a (not-necessarily-unit) normal against a light dir:
// clamp(dot(normalize(n), light), 0, 1). Back-facing surfaces clamp to 0.
float lambert(Vec3f normal, Vec3f light) {
    return std::clamp(dot(normalize(normal), light), 0.0f, 1.0f);
}

}  // namespace

// --- FlatShader ---

FlatShader::FlatShader(const Model& model, const Matrix& mvp, Vec3f light)
    : model_(model), mvp_(mvp), light_(light) {}

Vec4f FlatShader::vertex(int face, int nth) {
    if (nth == 0) normalSum_ = Vec3f{0, 0, 0};
    normalSum_ = normalSum_ + model_.normal(face, nth);
    varyingUV_[nth] = model_.uv(face, nth);
    // Once all three corners are in, the face's constant intensity is fixed
    // from the averaged (summed, then normalized in lambert) vertex normals.
    if (nth == 2) intensity_ = lambert(normalSum_, light_);
    return mvp_ * embed(model_.vert(face, nth));
}

bool FlatShader::fragment(Vec3f bary, Color& out) {
    out = shade(model_.diffuse(interpUV(varyingUV_, bary)), intensity_);
    return false;  // flat shading never discards
}

// --- GouraudShader ---

GouraudShader::GouraudShader(const Model& model, const Matrix& mvp, Vec3f light)
    : model_(model), mvp_(mvp), light_(light) {}

Vec4f GouraudShader::vertex(int face, int nth) {
    varyingUV_[nth] = model_.uv(face, nth);
    varyingIntensity_[nth] = lambert(model_.normal(face, nth), light_);
    return mvp_ * embed(model_.vert(face, nth));
}

bool GouraudShader::fragment(Vec3f bary, Color& out) {
    const float intensity = bary[0] * varyingIntensity_[0] +
                            bary[1] * varyingIntensity_[1] +
                            bary[2] * varyingIntensity_[2];
    out = shade(model_.diffuse(interpUV(varyingUV_, bary)), intensity);
    return false;
}

// --- PhongShader ---

PhongShader::PhongShader(const Model& model, const Matrix& mvp, Vec3f light)
    : model_(model), mvp_(mvp), light_(light) {}

Vec4f PhongShader::vertex(int face, int nth) {
    varyingUV_[nth] = model_.uv(face, nth);
    varyingNormal_[nth] = model_.normal(face, nth);
    return mvp_ * embed(model_.vert(face, nth));
}

bool PhongShader::fragment(Vec3f bary, Color& out) {
    // Interpolate the normal across the face, then light it per fragment.
    const Vec3f n = bary[0] * varyingNormal_[0] +
                    bary[1] * varyingNormal_[1] +
                    bary[2] * varyingNormal_[2];
    const float intensity = lambert(n, light_);
    out = shade(model_.diffuse(interpUV(varyingUV_, bary)), intensity);
    return false;
}

// --- NormalMapShader ---

NormalMapShader::NormalMapShader(const Model& model, const Matrix& mvp, Vec3f light)
    : model_(model), mvp_(mvp), light_(light) {}

Vec4f NormalMapShader::vertex(int face, int nth) {
    varyingUV_[nth] = model_.uv(face, nth);
    varyingNormal_[nth] = model_.normal(face, nth);
    varyingPos_[nth] = model_.vert(face, nth);  // model space == world space (1.8 invariant)
    return mvp_ * embed(model_.vert(face, nth));
}

bool NormalMapShader::fragment(Vec3f bary, Color& out) {
    // Interpolated smooth normal, used both as the TBN's N axis and as the
    // fallback when the triangle's UVs are degenerate.
    const Vec3f N = normalize(bary[0] * varyingNormal_[0] +
                              bary[1] * varyingNormal_[1] +
                              bary[2] * varyingNormal_[2]);

    // Triangle edges (world space) and matching UV gradients. Solving
    //   [e1] = [du1 dv1][T]      gives      T = ( dv2*e1 - dv1*e2) / det
    //   [e2]   [du2 dv2][B]                 B = (-du2*e1 + du1*e2) / det
    // with det = du1*dv2 - du2*dv1 (the 2x2 UV-matrix determinant).
    const Vec3f e1 = varyingPos_[1] - varyingPos_[0];
    const Vec3f e2 = varyingPos_[2] - varyingPos_[0];
    const float du1 = varyingUV_[1][0] - varyingUV_[0][0];
    const float dv1 = varyingUV_[1][1] - varyingUV_[0][1];
    const float du2 = varyingUV_[2][0] - varyingUV_[0][0];
    const float dv2 = varyingUV_[2][1] - varyingUV_[0][1];
    const float det = du1 * dv2 - du2 * dv1;

    const Vec3f n_t = model_.normalMap(interpUV(varyingUV_, bary));
    Vec3f n_world;
    if (std::abs(det) < 1e-8f) {
        // Degenerate UVs (no usable tangent frame): fall back to the smooth
        // normal, ignoring the tangent-space perturbation.
        n_world = N;
    } else {
        const float inv = 1.0f / det;
        Vec3f T = (dv2 * e1 - dv1 * e2) * inv;
        // Gram-Schmidt: orthonormalize T against the per-fragment N, then take
        // B perpendicular to both so [T B N] is a proper orthonormal basis.
        T = normalize(T - dot(T, N) * N);
        const Vec3f B = cross(N, T);
        // Transform the tangent-space normal into world space via [T B N].
        n_world = normalize(T * n_t[0] + B * n_t[1] + N * n_t[2]);
    }

    const float intensity = std::clamp(dot(n_world, light_), 0.0f, 1.0f);
    out = shade(model_.diffuse(interpUV(varyingUV_, bary)), intensity);
    return false;
}
