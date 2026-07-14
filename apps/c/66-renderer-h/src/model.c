#include "model.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// A dynamic-array push: double the capacity when full, then append. This is the
// C stand-in for std::vector::push_back. ponytail: on realloc failure the old
// block leaks and the next write crashes — fine for the bounded inputs here
// (procedural meshes + well-formed .obj); add a checked grow if that changes.
#define PUSH(arr, n, cap, val)                                          \
    do {                                                               \
        if ((n) >= (cap)) {                                            \
            int nc_ = (cap) ? (cap) * 2 : 8;                          \
            (arr) = realloc((arr), (size_t)nc_ * sizeof(*(arr)));      \
            (cap) = nc_;                                               \
        }                                                             \
        (arr)[(n)++] = (val);                                         \
    } while (0)

static Model model_new(void) {
    Model m;
    memset(&m, 0, sizeof m);
    return m;
}

// Append one triangle: its vertex/uv/normal index triples grow in lockstep.
static void push_face(Model* m, Face fv, Face fuv, Face fn) {
    if (m->nfaces >= m->capfaces) {
        int nc = m->capfaces ? m->capfaces * 2 : 8;
        m->faceV  = realloc(m->faceV,  (size_t)nc * sizeof(Face));
        m->faceUV = realloc(m->faceUV, (size_t)nc * sizeof(Face));
        m->faceN  = realloc(m->faceN,  (size_t)nc * sizeof(Face));
        m->capfaces = nc;
    }
    m->faceV[m->nfaces]  = fv;
    m->faceUV[m->nfaces] = fuv;
    m->faceN[m->nfaces]  = fn;
    m->nfaces++;
}

static Image img_copy(const Image* src) {
    Image d = img_create(src->w, src->h);
    if (d.px && src->px)
        memcpy(d.px, src->px, (size_t)src->w * (size_t)src->h * sizeof(Color));
    return d;
}

// Split quad (a,b,c,d) — vertex indices, CCW from outside — into two triangles
// sharing normal index n and the shared planar-UV rectangle corners.
static void push_quad(Model* m, int a, int b, int c, int d, int n) {
    Face fn = {{n, n, n}};
    push_face(m, (Face){{a, b, c}}, (Face){{0, 1, 2}}, fn);
    push_face(m, (Face){{a, c, d}}, (Face){{0, 2, 3}}, fn);
}

Model model_cube(void) {
    Model m = model_new();
    Vec3f v[8] = {
        {-1, -1, -1}, {1, -1, -1}, {1, 1, -1}, {-1, 1, -1},
        {-1, -1, 1},  {1, -1, 1},  {1, 1, 1},  {-1, 1, 1},
    };
    for (int i = 0; i < 8; ++i) PUSH(m.verts, m.nverts, m.capverts, v[i]);
    // Outward normals in face order: -Z, +Z, +X, -X, +Y, -Y.
    Vec3f nn[6] = {
        {0, 0, -1}, {0, 0, 1}, {1, 0, 0}, {-1, 0, 0}, {0, 1, 0}, {0, -1, 0},
    };
    for (int i = 0; i < 6; ++i) PUSH(m.norms, m.nnorms, m.capnorms, nn[i]);
    Vec2f uv[4] = {{0, 0}, {1, 0}, {1, 1}, {0, 1}};
    for (int i = 0; i < 4; ++i) PUSH(m.uvs, m.nuvs, m.capuvs, uv[i]);

    push_quad(&m, 0, 3, 2, 1, 0);  // -Z
    push_quad(&m, 4, 5, 6, 7, 1);  // +Z
    push_quad(&m, 1, 2, 6, 5, 2);  // +X
    push_quad(&m, 0, 4, 7, 3, 3);  // -X
    push_quad(&m, 2, 3, 7, 6, 4);  // +Y
    push_quad(&m, 1, 5, 4, 0, 5);  // -Y
    return m;
}

Model model_sphere(int lat, int lon) {
    Model m = model_new();
    const float pi = 3.14159265358979323846f;

    // A (lat+1) x (lon+1) vertex grid, pole to pole and around the equator; the
    // seam column is duplicated (same position, different UV) so texture coords
    // wrap cleanly.
    for (int i = 0; i <= lat; ++i) {
        float v = (float)i / (float)lat;
        float theta = v * pi;
        float y = cosf(theta);
        float r = sinf(theta);
        for (int j = 0; j <= lon; ++j) {
            float u = (float)j / (float)lon;
            float phi = u * 2.0f * pi;
            Vec3f pos = {r * cosf(phi), y, r * sinf(phi)};
            PUSH(m.verts, m.nverts, m.capverts, pos);
            PUSH(m.norms, m.nnorms, m.capnorms, vec3_normalize(pos));
            PUSH(m.uvs, m.nuvs, m.capuvs, ((Vec2f){u, v}));
        }
    }

    int stride = lon + 1;
    for (int i = 0; i < lat; ++i) {
        for (int j = 0; j < lon; ++j) {
            int a = i * stride + j;
            int b = i * stride + (j + 1);
            int c = (i + 1) * stride + (j + 1);
            int d = (i + 1) * stride + j;
            push_face(&m, (Face){{a, b, c}}, (Face){{a, b, c}}, (Face){{a, b, c}});
            push_face(&m, (Face){{a, c, d}}, (Face){{a, c, d}}, (Face){{a, c, d}});
        }
    }
    return m;
}

// Parse one OBJ face token ("v", "v/vt", "v/vt/vn", or "v//vn") into 0-based
// indices; missing vt/vn default to 0 (mirrors 53's parseFaceVertex).
static void parse_face_vertex(const char* tok, int* vi, int* ui, int* ni) {
    int a = 0, b = 0, c = 0;
    *vi = 0; *ui = 0; *ni = 0;
    if (sscanf(tok, "%d/%d/%d", &a, &b, &c) == 3)      { *vi = a - 1; *ui = b - 1; *ni = c - 1; }
    else if (sscanf(tok, "%d//%d", &a, &c) == 2)       { *vi = a - 1; *ni = c - 1; }
    else if (sscanf(tok, "%d/%d", &a, &b) == 2)        { *vi = a - 1; *ui = b - 1; }
    else if (sscanf(tok, "%d", &a) == 1)               { *vi = a - 1; }
}

int model_load_obj(Model* out, const char* path) {
    FILE* f = fopen(path, "r");
    if (!f) return -1;

    Model m = model_new();
    char line[1024];
    while (fgets(line, sizeof line, f)) {
        if (line[0] == 'v' && line[1] == ' ') {
            Vec3f v;
            if (sscanf(line + 2, "%f %f %f", &v.x, &v.y, &v.z) == 3)
                PUSH(m.verts, m.nverts, m.capverts, v);
        } else if (line[0] == 'v' && line[1] == 't') {
            Vec2f t;
            if (sscanf(line + 3, "%f %f", &t.x, &t.y) == 2)
                PUSH(m.uvs, m.nuvs, m.capuvs, t);
        } else if (line[0] == 'v' && line[1] == 'n') {
            Vec3f n;
            if (sscanf(line + 3, "%f %f %f", &n.x, &n.y, &n.z) == 3)
                PUSH(m.norms, m.nnorms, m.capnorms, n);
        } else if (line[0] == 'f' && line[1] == ' ') {
            int vs[64], us[64], ns[64], cnt = 0;
            char* tok = strtok(line + 2, " \t\r\n");
            while (tok && cnt < 64) {
                parse_face_vertex(tok, &vs[cnt], &us[cnt], &ns[cnt]);
                cnt++;
                tok = strtok(NULL, " \t\r\n");
            }
            // Fan-triangulate (a triangular face emits exactly one triangle).
            for (int k = 1; k + 1 < cnt; ++k) {
                push_face(&m,
                          (Face){{vs[0], vs[k], vs[k + 1]}},
                          (Face){{us[0], us[k], us[k + 1]}},
                          (Face){{ns[0], ns[k], ns[k + 1]}});
            }
        }
    }
    fclose(f);
    *out = m;
    return 0;
}

void model_free(Model* m) {
    if (!m) return;
    free(m->verts);
    free(m->norms);
    free(m->uvs);
    free(m->faceV);
    free(m->faceUV);
    free(m->faceN);
    if (m->has_diffuse)   img_free(&m->diffuse);
    if (m->has_normalmap) img_free(&m->normalmap);
    memset(m, 0, sizeof *m);
}

int   model_nfaces(const Model* m) { return m->nfaces; }

Vec3f model_vert(const Model* m, int face, int nth) {
    return m->verts[m->faceV[face].idx[nth]];
}

Vec2f model_uv(const Model* m, int face, int nth) {
    if (m->nuvs == 0) return (Vec2f){0, 0};  // minimal .obj (no vt lines)
    return m->uvs[m->faceUV[face].idx[nth]];
}

Vec3f model_normal(const Model* m, int face, int nth) {
    if (m->nnorms == 0) return (Vec3f){0, 0, 1};  // no vn lines
    return m->norms[m->faceN[face].idx[nth]];
}

void model_set_diffuse(Model* m, const Image* tex) {
    if (m->has_diffuse) img_free(&m->diffuse);
    m->diffuse = img_copy(tex);
    m->has_diffuse = 1;
}

Color model_diffuse(const Model* m, Vec2f uv) {
    if (m->has_diffuse) {
        int x = (int)(uv.x * (float)(m->diffuse.w - 1));
        int y = (int)((1.0f - uv.y) * (float)(m->diffuse.h - 1));
        return img_get(&m->diffuse, x, y);
    }
    int c = ((int)(uv.x * 8) + (int)(uv.y * 8)) & 1;
    return c ? (Color){200, 200, 200} : (Color){60, 60, 60};
}

void model_set_normalmap(Model* m, const Image* nm) {
    if (m->has_normalmap) img_free(&m->normalmap);
    m->normalmap = img_copy(nm);
    m->has_normalmap = 1;
}

Vec3f model_normalmap(const Model* m, Vec2f uv) {
    if (!m->has_normalmap) return (Vec3f){0, 0, 1};
    int x = (int)(uv.x * (float)(m->normalmap.w - 1));
    int y = (int)((1.0f - uv.y) * (float)(m->normalmap.h - 1));
    Color c = img_get(&m->normalmap, x, y);
    return (Vec3f){
        (float)c.r / 255.0f * 2.0f - 1.0f,
        (float)c.g / 255.0f * 2.0f - 1.0f,
        (float)c.b / 255.0f * 2.0f - 1.0f,
    };
}
