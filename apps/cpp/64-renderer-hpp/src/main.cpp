#include "image.hpp"
#include "geometry.hpp"
#include "gl.hpp"
#include "model.hpp"
#include "shader.hpp"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include <vector>

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
        std::cerr << "62-renderer-hpp: " << e.what() << '\n';
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
