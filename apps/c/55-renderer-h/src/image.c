#include "image.h"
#include <stdio.h>
#include <stdlib.h>

Image img_create(int w, int h) {
    Image img;
    img.w = w;
    img.h = h;
    // calloc zero-fills -> a black framebuffer. On failure px stays NULL and
    // callers (img_set/img_get) tolerate it; the owner still calls img_free.
    img.px = (w > 0 && h > 0) ? (Color*)calloc((size_t)w * (size_t)h, sizeof(Color)) : NULL;
    return img;
}

void img_free(Image* img) {
    if (!img) return;
    free(img->px);
    img->px = NULL;
}

void img_set(Image* img, int x, int y, Color c) {
    if (!img->px || x < 0 || x >= img->w || y < 0 || y >= img->h) return;
    img->px[(size_t)y * (size_t)img->w + (size_t)x] = c;
}

Color img_get(const Image* img, int x, int y) {
    if (!img->px) return (Color){0, 0, 0};
    int cx = x < 0 ? 0 : (x >= img->w ? img->w - 1 : x);
    int cy = y < 0 ? 0 : (y >= img->h ? img->h - 1 : y);
    return img->px[(size_t)cy * (size_t)img->w + (size_t)cx];
}

int img_write_ppm(const Image* img, const char* path) {
    FILE* f = fopen(path, "wb");
    if (!f) return -1;
    fprintf(f, "P6\n%d %d\n255\n", img->w, img->h);
    size_t n = (size_t)img->w * (size_t)img->h;
    for (size_t i = 0; i < n; ++i) {
        Color c = img->px[i];
        unsigned char rgb[3] = {c.r, c.g, c.b};
        if (fwrite(rgb, 1, 3, f) != 3) { fclose(f); return -1; }
    }
    fclose(f);
    return 0;
}

int img_read_ppm(Image* out, const char* path) {
    FILE* f = fopen(path, "rb");
    if (!f) return -1;

    char magic[3] = {0};
    int w = 0, h = 0, maxval = 0;
    if (fscanf(f, "%2s %d %d %d", magic, &w, &h, &maxval) != 4 ||
        magic[0] != 'P' || magic[1] != '6' || w <= 0 || h <= 0 || maxval != 255) {
        fclose(f);
        return -1;
    }
    fgetc(f);  // the single whitespace byte between the header and pixel data

    Image img = img_create(w, h);
    if (!img.px) { fclose(f); return -1; }
    size_t n = (size_t)w * (size_t)h;
    if (fread(img.px, 3, n, f) != n) {  // Color is 3 tightly-packed bytes
        img_free(&img);
        fclose(f);
        return -1;
    }
    fclose(f);
    *out = img;
    return 0;
}
