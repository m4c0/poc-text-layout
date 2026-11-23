#define _CRT_SECURE_NO_WARNINGS

#include <math.h>
#include <stdint.h>
#include <stdio.h>

#define WIDTH 128
#define HEIGHT 128

#define PI 3.1415926535897
#define SWAP(a, b) do { a ^= b; b ^= a; a ^= b; } while (0)

#define BMP_BLIP_R 2

struct __attribute__((packed)) bmp_hdr_s {
  char     bm[2];
  unsigned size;
  unsigned res;
  unsigned offs;

  unsigned hdr_sz;
  uint16_t width;
  uint16_t height;
  uint16_t planes;
  uint16_t bitc;
};

static void bmp_write(unsigned * bmp) {
  FILE * f = fopen("poc.bmp", "wb");

  struct bmp_hdr_s hdr = {
    .bm   = { 'B', 'M' },
    .size = sizeof(struct bmp_hdr_s) + WIDTH * HEIGHT * 4,
    .res  = 0,
    .offs = sizeof(struct bmp_hdr_s),

    .hdr_sz = 12,
    .width  = WIDTH,
    .height = HEIGHT,
    .planes = 1,
    .bitc   = 32,
  };
  fwrite(&hdr, 1, sizeof(hdr), f);

  fwrite(bmp, 1, WIDTH * HEIGHT * 4, f);

  fclose(f);
}

static inline void bmp_clear(unsigned * bmp) {
  for (int i = 0; i < WIDTH * HEIGHT; i++, bmp++) *bmp = 0xFF000000;
}

static inline void bmp_pix(unsigned * bmp, int x, int y) {
  if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return;
  bmp[(HEIGHT - y - 1) * WIDTH + x] = ~0;
}
static void bmp_blip(unsigned * bmp, int x, int y) {
  for (int dx = -BMP_BLIP_R; dx <= BMP_BLIP_R; dx++) {
    for (int dy = -BMP_BLIP_R; dy <= BMP_BLIP_R; dy++) {
      bmp_pix(bmp, x + dx, y + dy);
    }
  }
}
static void bmp_line(unsigned * bmp, int x1, int y1, int x2, int y2) {
  if (x1 > x2) SWAP(x1, x2);
  if (y1 > y2) SWAP(y1, y2);

  int dx = x2 - x1;
  int dy = y2 - y1;

  if (dx == 0 && dy == 0) {
    bmp_blip(bmp, x1, y1);
    return;
  }

  if (dx > dy) {
    for (int x = x1; x <= x2; x++) {
      int y = y1 + (x - x1) * dy / dx;
      bmp_blip(bmp, x, y);
    }
  } else {
    for (int y = y1; y <= y2; y++) {
      int x = x1 + (y - y1) * dx / dy;
      bmp_blip(bmp, x, y);
    }
  }
}

struct pen_s {
  unsigned * bmp;
  float x;
  float y;
  int a;
};

static void pen_f(struct pen_s * p, int d) {
  float tx = p->x + cos((p->a * PI) / 180.f) * d;
  float ty = p->y + sin((p->a * PI) / 180.f) * d;
  if (d >= 0) bmp_line(p->bmp, p->x, p->y, tx, ty);
  p->x = tx;
  p->y = ty;
}
static void pen_r(struct pen_s * p, int a) {
  p->a = (p->a - a + 360) % 360;
}

int main() {
  unsigned bmp[WIDTH * HEIGHT];

  bmp_clear(bmp);

  struct pen_s p = {
    .bmp = bmp,
    .x   = 20,
    .y   = 60,
  };
  pen_f(&p, 20);
  pen_f(&p, -10);
  pen_r(&p, 90);
  pen_f(&p, 40);
  pen_r(&p, 90);
  pen_f(&p, 10);

  p = (struct pen_s) {
    .bmp = bmp,
    .x   = 20 + 30,
    .y   = 60 +  0,
  };
  pen_f(&p, 20);
  pen_r(&p, 90);
  pen_f(&p, 40);
  pen_r(&p, 90);
  pen_f(&p, 20);
  pen_r(&p, 90);
  pen_f(&p, 40);
  pen_r(&p, -90);
  pen_f(&p, -10);
  pen_r(&p, 90);
  pen_f(&p, -20);
  pen_r(&p, 180);
  pen_f(&p, 0);

#if 0
  bmp_line(bmp, 20, 60, 40, 60);
  bmp_line(bmp, 30, 60, 30, 20);
  bmp_line(bmp, 30, 20, 20, 20);

  bmp_line(bmp, 50, 60, 70, 60);
  bmp_line(bmp, 70, 60, 70, 20);
  bmp_line(bmp, 70, 20, 50, 20);
  bmp_line(bmp, 50, 20, 50, 60);
  bmp_line(bmp, 60, 40, 60, 40);
#endif

  bmp_write(bmp);
}
