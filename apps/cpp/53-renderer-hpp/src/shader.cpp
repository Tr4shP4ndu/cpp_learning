#include "shader.hpp"
#include <algorithm>

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
