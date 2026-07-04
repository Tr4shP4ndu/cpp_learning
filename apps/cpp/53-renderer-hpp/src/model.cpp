#include "model.hpp"
#include <cmath>
#include <cstddef>
#include <fstream>
#include <numbers>
#include <sstream>
#include <stdexcept>

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
