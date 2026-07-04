#include "image.hpp"
#include "geometry.hpp"
#include "gl.hpp"
#include "model.hpp"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <limits>
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
    // For x=y=0, w=h=400:
    //   NDC(-1,-1,-1) -> screen (0, 400, 0)     [down/back corner -> bottom row, near plane]
    //   NDC( 1, 1, 1) -> screen (400, 0, 255)   [up/front corner  -> top row,    far plane]
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
}

int main(int argc, char** argv) {
    selfCheck();
    const int W = 400, H = 400;
    Image img(W, H);
    std::vector<float> zbuf(static_cast<std::size_t>(W) * static_cast<std::size_t>(H),
                             -std::numeric_limits<float>::infinity());

    Model model = Model::cube();
    // Pass a PPM path (e.g. `./render some_texture.ppm`) to wrap the cube in
    // a real texture instead of the default procedural checkerboard.
    if (argc > 1) {
        model.setDiffuse(Image::readPPM(argv[1]));
    }
    // Camera: eye offset from the origin (which the cube is centered on) for
    // a 3/4 view with visible perspective foreshortening. Per this lesson's
    // convention, coeff = -1/eye.z (eye placed along +z from a center at the
    // origin).
    const Vec3f eye{1.0f, 1.0f, 3.0f};
    const Vec3f center{0.0f, 0.0f, 0.0f};
    const Vec3f up{0.0f, 1.0f, 0.0f};
    const float coeff = -1.0f / eye[2];
    // This projection has no separate focal-length term: unlike tinyrenderer's
    // usual unit-sphere-normalized models, our procedural cube's corners sit
    // at distance sqrt(3) from its center, which would otherwise project
    // outside the [-1,1] NDC box (and thus off-screen). Scaling the model
    // down before the camera transform keeps it framed without touching
    // Model::cube()'s (unit, ±1) geometry, which other stages rely on.
    const float modelScale = 0.6f;

    const Matrix view = lookAt(eye, center, up);
    const Matrix proj = projection(coeff);
    const Matrix vp = viewport(0, 0, W, H);
    const Matrix M = vp * proj * view;

    const Vec3f lightDir = normalize(Vec3f{0.3f, 0.5f, 1.0f});

    for (int f = 0; f < model.nfaces(); ++f) {
        Vec3f screen[3];
        Vec2f uv[3];
        Vec3f normalSum{0, 0, 0};
        for (int k = 0; k < 3; ++k) {
            screen[k] = proj3(M * embed(model.vert(f, k) * modelScale));
            uv[k] = model.uv(f, k);
            // Normals are directions, not points: the model itself is never
            // transformed here (only the camera moves, via `view`), so
            // model space IS world space and the raw model-space normal is
            // already correct. Pushing it through M would be wrong now that
            // M includes lookAt's translation (and the perspective divide).
            normalSum = normalSum + model.normal(f, k);
        }
        const float intensity = std::clamp(dot(normalize(normalSum), lightDir), 0.0f, 1.0f);

        triangleTextured(screen, uv, model, intensity, img, zbuf);
    }

    img.writePPM("render.ppm");
    return 0;
}
