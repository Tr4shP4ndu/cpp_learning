#pragma once
#include "geometry.hpp"
#include "image.hpp"
#include "model.hpp"

// A programmable-shading interface in the spirit of GPU vertex/fragment
// shaders. The rasterizer (gl's triangle()) is generic: it knows nothing
// about lighting or texturing, and instead drives an IShader.
//
// The contract, per face:
//   1. triangle()'s caller calls vertex(face, nth) once for each corner
//      nth in {0,1,2}. Each call transforms that corner into CLIP space
//      (before the perspective divide and viewport) and stashes any
//      per-vertex "varyings" (UV, normal, precomputed intensity, ...) as
//      shader members, keyed by nth.
//   2. triangle() does the perspective divide + viewport itself, then for
//      every covered pixel calls fragment(bary, out): the shader interpolates
//      its stashed varyings with the barycentric weights `bary`, computes the
//      final colour into `out`, and returns true to DISCARD the fragment
//      (skip drawing it) or false to keep it.
//
// Clip-vs-screen split (important): vertex() returns clip space, i.e. the
// vertex AFTER projection*view (and any model transform) but BEFORE the
// w-divide and the viewport map. triangle() owns those last two steps. So a
// shader's transform matrix is the "MVP" WITHOUT viewport; folding viewport
// into it would double-apply the screen mapping.
struct IShader {
    virtual ~IShader() = default;
    // Transforms corner `nth` (in {0,1,2}) of face `face` to clip space and
    // stashes that corner's varyings. Returns the clip-space position.
    virtual Vec4f vertex(int face, int nth) = 0;
    // Interpolates the stashed varyings with barycentric weights `bary`,
    // writes the fragment colour to `out`, and returns true to discard the
    // fragment (do not draw) or false to keep it.
    virtual bool  fragment(Vec3f bary, Color& out) = 0;
};

// Flat shading: one constant light intensity for the whole triangle, taken
// from the (normalized) average of the face's three vertex normals. Produces
// visible facet edges on a low-poly mesh because adjacent faces get different
// constant intensities. Still interpolates UV per pixel for the base colour.
class FlatShader : public IShader {
public:
    FlatShader(const Model& model, const Matrix& mvp, Vec3f light);
    Vec4f vertex(int face, int nth) override;
    bool  fragment(Vec3f bary, Color& out) override;

private:
    const Model& model_;
    Matrix       mvp_;                 // projection * view * model (NO viewport)
    Vec3f        light_;              // world-space light direction (normalized)
    Vec2f        varyingUV_[3]{};     // per-corner UV, filled by vertex()
    Vec3f        normalSum_{0, 0, 0}; // running sum of the face's vertex normals
    float        intensity_ = 0.0f;   // constant face intensity, set at nth==2
};

// Gouraud shading: light intensity is computed per vertex in vertex() and
// linearly interpolated across the triangle in fragment(). Smoother than flat
// (gradients within a face) but highlights can be lost between vertices.
class GouraudShader : public IShader {
public:
    GouraudShader(const Model& model, const Matrix& mvp, Vec3f light);
    Vec4f vertex(int face, int nth) override;
    bool  fragment(Vec3f bary, Color& out) override;

private:
    const Model& model_;
    Matrix       mvp_;
    Vec3f        light_;
    Vec2f        varyingUV_[3]{};
    float        varyingIntensity_[3]{}; // per-corner Lambert intensity
};

// Phong shading: the surface normal itself is interpolated per pixel and the
// light intensity is computed per fragment from that interpolated normal.
// Smoothest of the three (no facet edges, highlights land correctly), at the
// cost of a normalize + dot per fragment. Base colour is model.diffuse(uv).
class PhongShader : public IShader {
public:
    PhongShader(const Model& model, const Matrix& mvp, Vec3f light);
    Vec4f vertex(int face, int nth) override;
    bool  fragment(Vec3f bary, Color& out) override;

private:
    const Model& model_;
    Matrix       mvp_;
    Vec3f        light_;
    Vec2f        varyingUV_[3]{};
    Vec3f        varyingNormal_[3]{}; // per-corner normal, interpolated in fragment
};
