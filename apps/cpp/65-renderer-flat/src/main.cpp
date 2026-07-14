// 63-renderer-flat — the ENTIRE tinyrenderer pipeline in one translation unit.
//
// This is a mechanical flatten of the multi-file 62-renderer-hpp: identical
// renderer, single file. See apps/cpp/63-renderer-flat/README.md for the
// layout contrast.
// The only differences from 53 are organizational: #pragma once and the
// cross-file #include "..." lines are gone, and every standard-library header
// is hoisted (deduplicated) into the block below. Order: all declarations
// (geometry, image, model, shader, gl) first, then all definitions.

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <memory>
#include <numbers>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

// ===================== geometry.hpp =====================

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

// ===================== image.hpp =====================

struct Color { unsigned char r{}, g{}, b{}; };

class Image {
public:
    Image(int w, int h);
    int width()  const { return w_; }
    int height() const { return h_; }
    void  set(int x, int y, Color c);
    Color get(int x, int y) const;                 // clamps to edge (safe for sampling)
    void  writePPM(const std::string& path) const;
    static Image readPPM(const std::string& path); // P6 only; throws std::runtime_error
private:
    int w_, h_;
    std::vector<Color> pixels_;                     // row-major, origin top-left
};

// ===================== model.hpp =====================

// A triangle mesh: flat arrays of unique positions/UVs/normals, plus one
// index triple per triangle corner into each array. Storing indices instead
// of duplicating full vertex data mirrors how OBJ files themselves work
// (separate v/vt/vn pools referenced by f lines).
//
// Faces are always triangles: loadObj fan-triangulates any face line with
// more than three vertices, and the procedural generators only ever emit
// triangles.
class Model {
public:
    static Model cube();                              // 8 verts, 12 triangles, face normals, planar UVs
    static Model sphere(int lat, int lon);             // UV-sphere; normals = normalized position
    static Model loadObj(const std::string& path);     // throws std::runtime_error if file won't open

    int   nfaces() const;
    Vec3f vert(int face, int nth) const;                // nth in {0,1,2}
    Vec2f uv(int face, int nth) const;
    Vec3f normal(int face, int nth) const;

    void  setDiffuse(const Image& tex);                 // stores a copy of a diffuse texture
    // Samples the diffuse texture at uv (v=1 is texture-top); with no texture
    // set, returns a procedural 8x8 checkerboard so lessons render something
    // meaningful before a real texture file exists.
    Color diffuse(Vec2f uv) const;

    void  setNormalMap(const Image& nm);                // stores a copy of a tangent-space normal map
    // Samples the normal map at uv (same convention as diffuse) and decodes each
    // channel c -> c/255*2 - 1 into a tangent-space normal; with no map set,
    // returns {0,0,1} (flat: no perturbation). The result is NOT normalized —
    // the shader transforms it through the TBN basis and lights with it.
    Vec3f normalMap(Vec2f uv) const;

private:
    std::vector<Vec3f> verts_, norms_;
    std::vector<Vec2f> uvs_;
    // One index triple per face corner: faceV_[f][n] indexes verts_ for the
    // n-th corner of face f, and likewise faceUV_/faceN_ index uvs_/norms_.
    std::vector<std::array<int, 3>> faceV_, faceUV_, faceN_;
    std::optional<Image> diffuse_;
    std::optional<Image> normalMap_;
};

// ===================== shader.hpp (IShader must precede gl.cpp's triangle()) =====================

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

// Tangent-space normal mapping: like Phong, but the per-fragment normal is read
// from a texture (model.normalMap(uv)) that stores a perturbation in *tangent
// space*. That perturbation is transformed into world space through a per-
// triangle TBN basis (Tangent, Bitangent, interpolated Normal) built from the
// triangle's edge vectors and UV gradients, then lit. This adds fine surface
// detail (bumps, grooves) that varies the lighting independently of geometry.
// With a flat/unset map, normalMap returns (0,0,1) and the result matches Phong.
class NormalMapShader : public IShader {
public:
    NormalMapShader(const Model& model, const Matrix& mvp, Vec3f light);
    Vec4f vertex(int face, int nth) override;
    bool  fragment(Vec3f bary, Color& out) override;

private:
    const Model& model_;
    Matrix       mvp_;
    Vec3f        light_;
    Vec2f        varyingUV_[3]{};
    Vec3f        varyingNormal_[3]{};   // per-corner normal
    Vec3f        varyingPos_[3]{};      // per-corner model==world position (for the TBN edges)
};

// ===================== gl.hpp =====================

struct IShader;  // defined in shader.hpp; triangle() drives it per-pixel

// Maps NDC coordinates in [-1,1]^3 to screen space [x, x+w] x [y, y+h] x
// [0,255] (the last being a depth range suitable for a z-buffer). Because
// Image (see image.hpp) is row-major with the origin at the TOP-LEFT, while
// NDC/camera space is right-handed with +y "up", a naive port of the classic
// (tinyrenderer) viewport formula would render the model upside down. This
// viewport folds the needed Y-flip in here (by negating the y-scale term
// while keeping the same y-offset), so callers never need to flip anything
// themselves: NDC y=+1 (up) lands at the smaller/top screen row, NDC y=-1
// (down) lands at the larger/bottom screen row.
Matrix viewport(int x, int y, int w, int h);

// Builds a perspective-projection matrix: identity except m[3][2] = coeff.
// Multiplying a view-space point by this matrix leaves x/y/z untouched but
// sets w = coeff*z + 1; the later perspective divide (proj3) by that w is
// what produces foreshortening. coeff = 0 recovers the identity (orthographic,
// no divide-driven distortion). Per this lesson's convention, coeff = -1/eye.z
// for a camera placed at distance eye.z along +z from a center at the origin.
Matrix projection(float coeff);

// Builds a view matrix that transforms world-space points into the camera's
// coordinate frame: camera at `eye`, looking toward `center`, with `up`
// defining "up" (need not be exactly perpendicular to the view direction).
// Convention (tinyrenderer): the camera basis is z = normalize(eye-center),
// x = normalize(cross(up,z)), y = cross(z,x) (already unit since x,z are
// orthonormal); these become the ROWS of the rotation part Minv, and the
// result is Minv * Tr, where Tr translates by -center. (Applying Tr first
// moves the world so `center` sits at the origin; Minv then rotates into the
// camera's axes.)
Matrix lookAt(Vec3f eye, Vec3f center, Vec3f up);

// Draws a straight line from (x0,y0) to (x1,y1) into img using color c.
// Integer Bresenham: no floating point, handles all octants (steep slopes
// and either endpoint ordering).
void drawLine(int x0, int y0, int x1, int y1, Image& img, Color c);

// Barycentric weights of p with respect to triangle (a,b,c): w[0] belongs to
// a, w[1] to b, w[2] to c. All weights sum to 1; p is inside the triangle iff
// all three are >= 0. A degenerate (zero-area) triangle returns a weight with
// a negative component so callers treat it as "outside".
Vec3f barycentric(Vec2f a, Vec2f b, Vec2f c, Vec2f p);

// Rasterizes one triangle whose three corners are given in CLIP space (the
// output of IShader::vertex, i.e. after projection*view but before the
// perspective divide and viewport). triangle() owns those two final steps: it
// does the w-divide (proj3) on each corner, then maps NDC to screen with a
// viewport(0,0,img.width(),img.height()) matrix. It then scans the clamped
// bounding box, and for every pixel inside the triangle (all barycentric
// weights >= 0) interpolates depth (screen[i][2], larger = closer) and z-tests
// against zbuf (indexed x + y*img.width()). Only on passing the z-test does it
// invoke shader.fragment(bary, color); the pixel is drawn and the z-buffer
// updated iff the shader keeps the fragment (returns false, i.e. no discard).
void triangle(const Vec4f clip[3], IShader& shader, Image& img, std::vector<float>& zbuf);

// ===================== image.cpp =====================

Image::Image(int w, int h)
    : w_(w), h_(h), pixels_(static_cast<std::size_t>(w) * static_cast<std::size_t>(h)) {}

void Image::set(int x, int y, Color c) {
    if (x < 0 || x >= w_ || y < 0 || y >= h_) return; // out-of-range: ignore
    pixels_[static_cast<std::size_t>(y) * static_cast<std::size_t>(w_) + static_cast<std::size_t>(x)] = c;
}

Color Image::get(int x, int y) const {
    const int cx = std::clamp(x, 0, w_ - 1);
    const int cy = std::clamp(y, 0, h_ - 1);
    return pixels_[static_cast<std::size_t>(cy) * static_cast<std::size_t>(w_) + static_cast<std::size_t>(cx)];
}

void Image::writePPM(const std::string& path) const {
    std::ofstream out(path, std::ios::binary);
    if (!out) throw std::runtime_error("Image::writePPM: cannot open " + path);
    out << "P6\n" << w_ << ' ' << h_ << "\n255\n";
    for (const Color& c : pixels_) {
        out.put(static_cast<char>(c.r));
        out.put(static_cast<char>(c.g));
        out.put(static_cast<char>(c.b));
    }
}

Image Image::readPPM(const std::string& path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) throw std::runtime_error("Image::readPPM: cannot open " + path);

    std::string magic;
    int w = 0, h = 0, maxval = 0;
    in >> magic >> w >> h >> maxval;
    if (!in || magic != "P6") throw std::runtime_error("Image::readPPM: not a P6 file: " + path);
    if (w <= 0 || h <= 0) throw std::runtime_error("Image::readPPM: bad dimensions: " + path);
    if (maxval != 255) throw std::runtime_error("Image::readPPM: only 8-bit (maxval 255) P6 is supported: " + path);
    in.get(); // the single whitespace byte separating the header from pixel data

    Image img(w, h);
    const auto bytes = static_cast<std::streamsize>(img.pixels_.size() * 3);
    in.read(reinterpret_cast<char*>(img.pixels_.data()), bytes);
    if (!in) throw std::runtime_error("Image::readPPM: truncated pixel data: " + path);
    return img;
}

// ===================== model.cpp =====================

namespace {

// Parses one OBJ face token ("v", "v/vt", "v/vt/vn", or "v//vn") into
// 0-based (vertex, uv, normal) indices. Missing vt/vn tokens default to
// index 0, as OBJ files that omit them still expect *some* attribute to be
// sampled; callers that never populate uvs_/norms_ simply never read it.
std::array<int, 3> parseFaceVertex(const std::string& tok) {
    int vi = 0, ui = 0, ni = 0;
    const std::size_t p1 = tok.find('/');
    if (p1 == std::string::npos) {
        vi = std::stoi(tok) - 1;
        return {vi, ui, ni};
    }
    vi = std::stoi(tok.substr(0, p1)) - 1;

    const std::size_t p2 = tok.find('/', p1 + 1);
    if (p2 == std::string::npos) {
        const std::string uPart = tok.substr(p1 + 1);
        if (!uPart.empty()) ui = std::stoi(uPart) - 1;
        return {vi, ui, ni};
    }

    const std::string uPart = tok.substr(p1 + 1, p2 - p1 - 1);
    if (!uPart.empty()) ui = std::stoi(uPart) - 1;
    const std::string nPart = tok.substr(p2 + 1);
    if (!nPart.empty()) ni = std::stoi(nPart) - 1;
    return {vi, ui, ni};
}

}  // namespace

Model Model::cube() {
    Model m;
    m.verts_ = {
        Vec3f{-1, -1, -1}, Vec3f{1, -1, -1}, Vec3f{1, 1, -1}, Vec3f{-1, 1, -1},
        Vec3f{-1, -1, 1},  Vec3f{1, -1, 1},  Vec3f{1, 1, 1},  Vec3f{-1, 1, 1},
    };
    // One outward-facing normal per cube face, in the order the faces are
    // added below: -Z, +Z, +X, -X, +Y, -Y.
    m.norms_ = {
        Vec3f{0, 0, -1}, Vec3f{0, 0, 1}, Vec3f{1, 0, 0},
        Vec3f{-1, 0, 0}, Vec3f{0, 1, 0}, Vec3f{0, -1, 0},
    };
    // Shared planar UV rectangle corners, reused by every face.
    m.uvs_ = {Vec2f{0, 0}, Vec2f{1, 0}, Vec2f{1, 1}, Vec2f{0, 1}};

    // Splits quad (a,b,c,d) — corner indices into verts_, wound CCW as seen
    // from outside the cube — into two triangles sharing normal index n.
    auto addQuad = [&m](int a, int b, int c, int d, int n) {
        m.faceV_.push_back({a, b, c});
        m.faceUV_.push_back({0, 1, 2});
        m.faceN_.push_back({n, n, n});
        m.faceV_.push_back({a, c, d});
        m.faceUV_.push_back({0, 2, 3});
        m.faceN_.push_back({n, n, n});
    };

    addQuad(0, 3, 2, 1, 0);  // -Z
    addQuad(4, 5, 6, 7, 1);  // +Z
    addQuad(1, 2, 6, 5, 2);  // +X
    addQuad(0, 4, 7, 3, 3);  // -X
    addQuad(2, 3, 7, 6, 4);  // +Y
    addQuad(1, 5, 4, 0, 5);  // -Y

    return m;
}

Model Model::sphere(int lat, int lon) {
    Model m;
    constexpr float pi = std::numbers::pi_v<float>;

    // A (lat+1) x (lon+1) vertex grid, walking from pole (i=0) to pole
    // (i=lat) and around the equator (j=0..lon). The grid duplicates the
    // seam column (j=0 and j=lon share position but not UV) so texture
    // coordinates wrap cleanly.
    for (int i = 0; i <= lat; ++i) {
        const float v = static_cast<float>(i) / static_cast<float>(lat);
        const float theta = v * pi;
        const float y = std::cos(theta);
        const float r = std::sin(theta);
        for (int j = 0; j <= lon; ++j) {
            const float u = static_cast<float>(j) / static_cast<float>(lon);
            const float phi = u * 2.0f * pi;
            const Vec3f pos{r * std::cos(phi), y, r * std::sin(phi)};
            m.verts_.push_back(pos);
            m.norms_.push_back(normalize(pos));  // unit sphere: position *is* the outward normal
            m.uvs_.push_back(Vec2f{u, v});
        }
    }

    const int stride = lon + 1;
    for (int i = 0; i < lat; ++i) {
        for (int j = 0; j < lon; ++j) {
            const int a = i * stride + j;
            const int b = i * stride + (j + 1);
            const int c = (i + 1) * stride + (j + 1);
            const int d = (i + 1) * stride + j;
            m.faceV_.push_back({a, b, c});
            m.faceUV_.push_back({a, b, c});
            m.faceN_.push_back({a, b, c});
            m.faceV_.push_back({a, c, d});
            m.faceUV_.push_back({a, c, d});
            m.faceN_.push_back({a, c, d});
        }
    }
    return m;
}

Model Model::loadObj(const std::string& path) {
    std::ifstream in(path);
    if (!in) {
        throw std::runtime_error("Model::loadObj: cannot open file: " + path);
    }

    Model m;
    std::string line;
    while (std::getline(in, line)) {
        std::istringstream iss(line);
        std::string tag;
        iss >> tag;
        if (tag == "v") {
            Vec3f v{};
            iss >> v[0] >> v[1] >> v[2];
            m.verts_.push_back(v);
        } else if (tag == "vt") {
            Vec2f vt{};
            iss >> vt[0] >> vt[1];
            m.uvs_.push_back(vt);
        } else if (tag == "vn") {
            Vec3f vn{};
            iss >> vn[0] >> vn[1] >> vn[2];
            m.norms_.push_back(vn);
        } else if (tag == "f") {
            std::vector<std::array<int, 3>> corners;
            std::string tok;
            while (iss >> tok) {
                corners.push_back(parseFaceVertex(tok));
            }
            // Fan-triangulate: for an already-triangular face this emits
            // exactly the one triangle (corners[0], corners[1], corners[2]).
            for (std::size_t k = 1; k + 1 < corners.size(); ++k) {
                m.faceV_.push_back({corners[0][0], corners[k][0], corners[k + 1][0]});
                m.faceUV_.push_back({corners[0][1], corners[k][1], corners[k + 1][1]});
                m.faceN_.push_back({corners[0][2], corners[k][2], corners[k + 1][2]});
            }
        }
    }
    return m;
}

int Model::nfaces() const { return static_cast<int>(faceV_.size()); }

Vec3f Model::vert(int face, int nth) const {
    const auto& idx = faceV_[static_cast<std::size_t>(face)];
    return verts_[static_cast<std::size_t>(idx[static_cast<std::size_t>(nth)])];
}

Vec2f Model::uv(int face, int nth) const {
    // A minimal OBJ (only v + f lines) leaves uvs_ empty while parseFaceVertex
    // still defaults the uv index to 0; indexing an empty pool would read out
    // of bounds, so return a neutral UV instead.
    if (uvs_.empty()) return Vec2f{0, 0};
    const auto& idx = faceUV_[static_cast<std::size_t>(face)];
    return uvs_[static_cast<std::size_t>(idx[static_cast<std::size_t>(nth)])];
}

Vec3f Model::normal(int face, int nth) const {
    // Likewise for a model with no vn lines: fall back to a forward-facing
    // normal rather than indexing an empty norms_.
    if (norms_.empty()) return Vec3f{0, 0, 1};
    const auto& idx = faceN_[static_cast<std::size_t>(face)];
    return norms_[static_cast<std::size_t>(idx[static_cast<std::size_t>(nth)])];
}

void Model::setDiffuse(const Image& tex) { diffuse_ = tex; }

Color Model::diffuse(Vec2f uv) const {
    if (diffuse_) {
        const float x = uv[0] * static_cast<float>(diffuse_->width() - 1);
        const float y = (1.0f - uv[1]) * static_cast<float>(diffuse_->height() - 1);
        return diffuse_->get(static_cast<int>(x), static_cast<int>(y));
    }
    // No texture: an 8x8 procedural checkerboard, so UV mapping is visible
    // even without an external texture asset.
    const int c = (static_cast<int>(uv[0] * 8) + static_cast<int>(uv[1] * 8)) & 1;
    return c ? Color{200, 200, 200} : Color{60, 60, 60};
}

void Model::setNormalMap(const Image& nm) { normalMap_ = nm; }

Vec3f Model::normalMap(Vec2f uv) const {
    // No map set: a flat tangent-space normal (0,0,1) means "no perturbation",
    // so a NormalMapShader falls back to the smooth interpolated normal.
    if (!normalMap_) return Vec3f{0, 0, 1};
    const float x = uv[0] * static_cast<float>(normalMap_->width() - 1);
    const float y = (1.0f - uv[1]) * static_cast<float>(normalMap_->height() - 1);
    const Color c = normalMap_->get(static_cast<int>(x), static_cast<int>(y));
    // Decode each 0..255 channel back to the [-1,1] range it was packed from.
    return Vec3f{
        static_cast<float>(c.r) / 255.0f * 2.0f - 1.0f,
        static_cast<float>(c.g) / 255.0f * 2.0f - 1.0f,
        static_cast<float>(c.b) / 255.0f * 2.0f - 1.0f,
    };
}

// ===================== gl.cpp =====================

Matrix viewport(int x, int y, int w, int h) {
    Matrix r = Matrix::identity();
    r.m[0][0] = static_cast<float>(w) / 2.0f;
    r.m[0][3] = static_cast<float>(x) + static_cast<float>(w) / 2.0f;
    // Negated (vs. the textbook +h/2) to fold in the Y-flip: see the
    // rationale in gl.hpp's viewport() comment.
    r.m[1][1] = -static_cast<float>(h) / 2.0f;
    r.m[1][3] = static_cast<float>(y) + static_cast<float>(h) / 2.0f;
    r.m[2][2] = 255.0f / 2.0f;
    r.m[2][3] = 255.0f / 2.0f;
    return r;
}

Matrix projection(float coeff) {
    Matrix r = Matrix::identity();
    r.m[3][2] = coeff;
    return r;
}

Matrix lookAt(Vec3f eye, Vec3f center, Vec3f up) {
    const Vec3f z = normalize(eye - center);
    const Vec3f x = normalize(cross(up, z));
    const Vec3f y = cross(z, x);  // unit: cross of two orthonormal unit vectors

    Matrix minv = Matrix::identity();
    Matrix tr = Matrix::identity();
    for (int i = 0; i < 3; ++i) {
        minv.m[0][i] = x[i];
        minv.m[1][i] = y[i];
        minv.m[2][i] = z[i];
        tr.m[i][3] = -center[i];
    }
    return minv * tr;
}

void drawLine(int x0, int y0, int x1, int y1, Image& img, Color c) {
    // Steep lines (|dy| > |dx|) are transposed so we always step along the
    // major axis; this keeps the error accumulator integer-only and gap-free.
    const bool steep = std::abs(y1 - y0) > std::abs(x1 - x0);
    if (steep) {
        std::swap(x0, y0);
        std::swap(x1, y1);
    }
    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    const int dx = x1 - x0;
    const int dy = std::abs(y1 - y0);
    const int yStep = (y0 < y1) ? 1 : -1;

    int error = dx / 2;
    int y = y0;
    for (int x = x0; x <= x1; ++x) {
        if (steep) {
            img.set(y, x, c);
        } else {
            img.set(x, y, c);
        }
        error -= dy;
        if (error < 0) {
            y += yStep;
            error += dx;
        }
    }
}

Vec3f barycentric(Vec2f a, Vec2f b, Vec2f c, Vec2f p) {
    // Cross the edge vectors' x- and y-components; u.z is (twice) the signed
    // triangle area. |u.z| < 1 means the triangle is degenerate on the pixel
    // grid, so we report "outside" via a negative weight rather than divide
    // by (near) zero.
    Vec3f u = cross(Vec3f{c[0] - a[0], b[0] - a[0], a[0] - p[0]},
                     Vec3f{c[1] - a[1], b[1] - a[1], a[1] - p[1]});
    if (std::abs(u[2]) < 1) return Vec3f{-1, 1, 1};
    return Vec3f{1.f - (u[0] + u[1]) / u[2], u[1] / u[2], u[0] / u[2]};
}

void triangle(const Vec4f clip[3], IShader& shader, Image& img, std::vector<float>& zbuf) {
    // Finish the pipeline the shader deliberately stopped short of: perspective
    // divide (clip -> NDC) then viewport (NDC -> screen, with depth in [0,255]).
    const Matrix vp = viewport(0, 0, img.width(), img.height());
    Vec3f screen[3];
    for (int i = 0; i < 3; ++i) screen[i] = proj3(vp * embed(proj3(clip[i])));

    const Vec2f a{screen[0][0], screen[0][1]};
    const Vec2f b{screen[1][0], screen[1][1]};
    const Vec2f cc{screen[2][0], screen[2][1]};

    const float minXf = std::min({a[0], b[0], cc[0]});
    const float maxXf = std::max({a[0], b[0], cc[0]});
    const float minYf = std::min({a[1], b[1], cc[1]});
    const float maxYf = std::max({a[1], b[1], cc[1]});

    const int x0 = std::clamp(static_cast<int>(std::floor(minXf)), 0, img.width() - 1);
    const int x1 = std::clamp(static_cast<int>(std::ceil(maxXf)), 0, img.width() - 1);
    const int y0 = std::clamp(static_cast<int>(std::floor(minYf)), 0, img.height() - 1);
    const int y1 = std::clamp(static_cast<int>(std::ceil(maxYf)), 0, img.height() - 1);

    for (int y = y0; y <= y1; ++y) {
        for (int x = x0; x <= x1; ++x) {
            const Vec2f p{static_cast<float>(x), static_cast<float>(y)};
            const Vec3f w = barycentric(a, b, cc, p);
            if (w[0] >= 0 && w[1] >= 0 && w[2] >= 0) {
                const float z = w[0] * screen[0][2] + w[1] * screen[1][2] + w[2] * screen[2][2];
                const std::size_t idx =
                    static_cast<std::size_t>(x) + static_cast<std::size_t>(y) * static_cast<std::size_t>(img.width());
                if (z > zbuf[idx]) {
                    Color color;
                    // Let the shader colour (or discard) the fragment; only a
                    // kept fragment updates the framebuffer and z-buffer.
                    if (!shader.fragment(w, color)) {
                        zbuf[idx] = z;
                        img.set(x, y, color);
                    }
                }
            }
        }
    }
}

// ===================== shader.cpp =====================

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

// ===================== main.cpp =====================

static void selfCheck() {
    Image t(4, 4);
    t.set(1, 2, Color{10, 20, 30});
    Color c = t.get(1, 2);
    assert(c.r == 10 && c.g == 20 && c.b == 30);
    assert(t.get(-5, -5).r == 0);                  // clamp returns edge pixel (black here)

    Vec3f a{1,0,0}, b{0,1,0};
    assert(dot(a, b) == 0.0f);
    Vec3f v3 = cross(a, b);                        // z = +1
    assert(v3[0]==0 && v3[1]==0 && v3[2]==1);
    assert(std::abs(norm(normalize(Vec3f{3,4,0})) - 1.0f) < 1e-6f);
    Matrix I = Matrix::identity();
    Vec4f v = I * Vec4f{2,3,4,1};
    assert(v[0]==2 && v[1]==3 && v[2]==4 && v[3]==1);

    Image line(10, 10);
    drawLine(0, 0, 9, 0, line, Color{255, 255, 255});
    assert(line.get(9, 0).r == 255);               // last endpoint got drawn

    Vec2f A{0,0}, B{4,0}, C{0,4};
    Vec3f w = barycentric(A, B, C, Vec2f{1,1});     // inside
    assert(w[0]>0 && w[1]>0 && w[2]>0);
    assert(std::abs(w[0]+w[1]+w[2] - 1.0f) < 1e-5f);
    Vec3f wo = barycentric(A, B, C, Vec2f{5,5});    // outside -> a negative weight
    assert(wo[0]<0 || wo[1]<0 || wo[2]<0);

    assert(Model::cube().nfaces() == 12);           // 6 faces * 2 triangles
    const Model cube = Model::cube();
    assert(std::isfinite(cube.uv(0, 0)[0]) && std::isfinite(cube.uv(0, 0)[1]));
    assert(std::isfinite(cube.normal(0, 0)[0]) && std::isfinite(cube.normal(0, 0)[2]));

    // No texture set: diffuse() must fall back to one of the two checkerboard
    // colors (never something else, e.g. a stale/garbage value).
    Model bare = Model::cube();
    const Color checker = bare.diffuse(Vec2f{0.1f, 0.1f});
    const Color light{200, 200, 200}, dark{60, 60, 60};
    auto sameColor = [](Color a, Color b) { return a.r == b.r && a.g == b.g && a.b == b.b; };
    assert(sameColor(checker, light) || sameColor(checker, dark));

    // An all-white texture sampled anywhere must read back as white.
    Image white(2, 2);
    white.set(0, 0, Color{255, 255, 255});
    white.set(1, 0, Color{255, 255, 255});
    white.set(0, 1, Color{255, 255, 255});
    white.set(1, 1, Color{255, 255, 255});
    bare.setDiffuse(white);
    const Color sampled = bare.diffuse(Vec2f{0.5f, 0.5f});
    assert(sampled.r == 255 && sampled.g == 255 && sampled.b == 255);

    // --- Task 1.8: projection / viewport / lookAt ---

    // projection(0) must be exactly the identity: no perspective divide
    // distortion when coeff is 0.
    {
        const Matrix p0 = projection(0.0f);
        const Matrix id = Matrix::identity();
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j) assert(std::abs(p0.m[i][j] - id.m[i][j]) < 1e-6f);
    }

    // viewport(0,0,400,400) maps the NDC cube's two opposite corners to exact
    // screen/depth values, by hand from the formula in gl.cpp:
    //   screen_x = w/2 * ndc_x + (x + w/2)
    //   screen_y = -h/2 * ndc_y + (y + h/2)   (negated to fold in the Y-flip)
    //   depth    = 255/2 * ndc_z + 255/2
    // For x=y=0, w=h=400 (recall this renderer is larger-depth-is-nearer, so
    // depth 0 = farthest from the camera, depth 255 = nearest):
    //   NDC(-1,-1,-1) -> screen (0, 400, 0)     [down corner -> bottom row, depth 0   (farthest)]
    //   NDC( 1, 1, 1) -> screen (400, 0, 255)   [up corner   -> top row,    depth 255 (nearest)]
    {
        const Matrix vp = viewport(0, 0, 400, 400);
        const Vec3f lo = proj3(vp * embed(Vec3f{-1, -1, -1}));
        const Vec3f hi = proj3(vp * embed(Vec3f{1, 1, 1}));
        assert(std::abs(lo[0] - 0.0f) < 1e-4f);
        assert(std::abs(lo[1] - 400.0f) < 1e-4f);
        assert(std::abs(lo[2] - 0.0f) < 1e-4f);
        assert(std::abs(hi[0] - 400.0f) < 1e-4f);
        assert(std::abs(hi[1] - 0.0f) < 1e-4f);
        assert(std::abs(hi[2] - 255.0f) < 1e-4f);
    }

    // lookAt's rotation part (rows 0..2, cols 0..2) must be orthonormal: each
    // row unit length, and every pair of rows mutually perpendicular.
    {
        const Matrix view = lookAt(Vec3f{1, 1, 3}, Vec3f{0, 0, 0}, Vec3f{0, 1, 0});
        Vec3f rows[3];
        for (int i = 0; i < 3; ++i) rows[i] = Vec3f{view.m[i][0], view.m[i][1], view.m[i][2]};
        for (int i = 0; i < 3; ++i) assert(std::abs(norm(rows[i]) - 1.0f) < 1e-5f);
        assert(std::abs(dot(rows[0], rows[1])) < 1e-5f);
        assert(std::abs(dot(rows[1], rows[2])) < 1e-5f);
        assert(std::abs(dot(rows[0], rows[2])) < 1e-5f);
    }

    // --- Task 1.9: shader abstraction ---

    // vertex() must apply the shader's MVP to the clip-space position, and
    // fragment() must (a) never discard for these three shaders and (b) at a
    // corner where the light exactly matches that corner's normal, produce the
    // *undimmed* diffuse sample (intensity == 1).
    {
        const Model cubeS = Model::cube();
        // A pure scale-by-2 MVP so the vertex()-applies-MVP check has teeth:
        // if vertex() forgot the multiply, clip would equal embed(vert), not 2x.
        Matrix scale2 = Matrix::identity();
        scale2.m[0][0] = scale2.m[1][1] = scale2.m[2][2] = 2.0f;
        // Aim the light straight down corner 0's own normal.
        const Vec3f n0 = normalize(cubeS.normal(0, 0));
        PhongShader ph(cubeS, scale2, n0);

        const Vec4f clip0 = ph.vertex(0, 0);
        const Vec3f v0 = cubeS.vert(0, 0);
        assert(std::abs(clip0[0] - 2.0f * v0[0]) < 1e-5f);
        assert(std::abs(clip0[1] - 2.0f * v0[1]) < 1e-5f);
        assert(std::abs(clip0[2] - 2.0f * v0[2]) < 1e-5f);
        assert(std::abs(clip0[3] - 1.0f) < 1e-5f);

        // Fill the remaining two corners' varyings before shading.
        ph.vertex(0, 1);
        ph.vertex(0, 2);
        Color out{};
        const bool discard = ph.fragment(Vec3f{1, 0, 0}, out);  // exactly at corner 0
        assert(!discard);
        // Interpolated normal at bary (1,0,0) is corner 0's normal, so
        // intensity = dot(n0, n0) = 1 and the colour is the raw diffuse sample.
        const Color base0 = cubeS.diffuse(cubeS.uv(0, 0));
        assert(out.r == base0.r && out.g == base0.g && out.b == base0.b);
    }

    // --- Task 1.10: tangent-space normal mapping ---

    // With no normal map set, normalMap() must return exactly the flat
    // tangent-space normal (0,0,1) — the "no perturbation" fallback that makes
    // a NormalMapShader reduce to Phong.
    {
        const Model bareNM = Model::cube();
        const Vec3f n = bareNM.normalMap(Vec2f{0.3f, 0.7f});
        assert(n[0] == 0.0f && n[1] == 0.0f && n[2] == 1.0f);
    }

    // A flat map filled with (128,128,255) encodes tangent-space (0,0,1): each
    // channel decodes as c/255*2-1, so 128 -> ~0.004 and 255 -> +1. The result
    // must be approximately (0,0,1) within one quantization step (~1/255*2).
    {
        Model flatNM = Model::cube();
        Image flat(2, 2);
        for (int y = 0; y < 2; ++y)
            for (int x = 0; x < 2; ++x) flat.set(x, y, Color{128, 128, 255});
        flatNM.setNormalMap(flat);
        const Vec3f n = flatNM.normalMap(Vec2f{0.5f, 0.5f});
        const float tol = 1.0f / 255.0f * 2.0f;  // ~0.0078
        assert(std::abs(n[0]) < tol && std::abs(n[1]) < tol && std::abs(n[2] - 1.0f) < tol);
    }
}

int main(int argc, char** argv) {
    selfCheck();
    const int W = 400, H = 400;
    Image img(W, H);
    std::vector<float> zbuf(static_cast<std::size_t>(W) * static_cast<std::size_t>(H),
                             -std::numeric_limits<float>::infinity());

    // --- Model + optional textures from the command line ---
    // (full arg list in assets/tinyrenderer/README.md)
    //   argv[1]  model:  "cube" (default) | "sphere" | path to a .obj
    //   argv[2]  shader: "flat" | "gouraud" | "phong" | "normal" (default)
    //   argv[3]  P6 PPM diffuse texture   ("-" keeps the procedural checkerboard)
    //   argv[4]  P6 PPM tangent-space normal map (else a flat, no-op map)
    // A bad path or malformed file throws; catch it so the user gets a clean
    // message instead of an uncaught-exception std::terminate.
    Model model = Model::cube();
    try {
        const std::string modelArg = (argc > 1) ? argv[1] : "cube";
        if (modelArg == "cube") {
            model = Model::cube();
        } else if (modelArg == "sphere") {
            model = Model::sphere(24, 24);
        } else {
            model = Model::loadObj(modelArg);
        }
        if (argc > 3 && std::string(argv[3]) != "-") {
            model.setDiffuse(Image::readPPM(argv[3]));
        }
        if (argc > 4 && std::string(argv[4]) != "-") {
            model.setNormalMap(Image::readPPM(argv[4]));
        }
    } catch (const std::exception& e) {
        std::cerr << "63-renderer-flat: " << e.what() << '\n';
        return 1;
    }
    // argv[2]: shader — "flat" (facet edges), "gouraud", "phong" (smooth), or
    // "normal" (default; adds normal-map detail, == phong with a flat/unset map).
    const std::string shaderName = (argc > 2) ? argv[2] : "normal";

    // Camera: eye offset from the origin (models are centered there) for a 3/4
    // view with visible perspective foreshortening. coeff = -1/eye.z (eye placed
    // along +z from a center at the origin).
    const Vec3f eye{1.0f, 1.0f, 3.0f};
    const Vec3f center{0.0f, 0.0f, 0.0f};
    const Vec3f up{0.0f, 1.0f, 0.0f};
    const float coeff = -1.0f / eye[2];

    // This minimal projection has no focal-length/FOV term, so a model fits the
    // [-1,1] NDC box only if its own extent is small enough. Rather than hardcode
    // a factor for one mesh, fit ANY model: find its largest |coordinate| and
    // scale so that maps to ~0.6 (a unit ±1 cube -> 0.6, exactly as before; a
    // bigger/smaller .obj scales to match). Uniform scale, so normals are safe.
    float maxExtent = 1e-6f;  // seed away from 0 to avoid /0 on an empty model
    for (int f = 0; f < model.nfaces(); ++f)
        for (int k = 0; k < 3; ++k) {
            const Vec3f v = model.vert(f, k);
            maxExtent = std::max({maxExtent, std::abs(v[0]), std::abs(v[1]), std::abs(v[2])});
        }
    const float modelScale = 0.6f / maxExtent;

    const Matrix view = lookAt(eye, center, up);
    const Matrix proj = projection(coeff);
    // The shader's "MVP" is projection * view * (uniform model scale) — and
    // deliberately EXCLUDES viewport: vertex() returns clip space, and
    // triangle() applies the perspective divide + viewport itself. Folding the
    // framing scale in as the model matrix keeps the shader oblivious to it
    // (and, being a uniform scale, it never touches normals).
    Matrix scale = Matrix::identity();
    scale.m[0][0] = scale.m[1][1] = scale.m[2][2] = modelScale;
    const Matrix mvp = proj * view * scale;

    const Vec3f lightDir = normalize(Vec3f{0.3f, 0.5f, 1.0f});

    // Normals are directions, not points: the model itself is never transformed
    // here (only the camera moves, via `view`), so model space IS world space
    // and the raw model-space normals the shaders read are already correct.
    // Pushing them through the MVP would be wrong (it carries lookAt's
    // translation and the perspective term).
    std::unique_ptr<IShader> shader;
    if (shaderName == "flat") {
        shader = std::make_unique<FlatShader>(model, mvp, lightDir);
    } else if (shaderName == "gouraud") {
        shader = std::make_unique<GouraudShader>(model, mvp, lightDir);
    } else if (shaderName == "phong") {
        shader = std::make_unique<PhongShader>(model, mvp, lightDir);
    } else {
        shader = std::make_unique<NormalMapShader>(model, mvp, lightDir);  // default
    }

    for (int f = 0; f < model.nfaces(); ++f) {
        Vec4f clip[3];
        for (int k = 0; k < 3; ++k) clip[k] = shader->vertex(f, k);
        triangle(clip, *shader, img, zbuf);
    }

    img.writePPM("render.ppm");
    return 0;
}
