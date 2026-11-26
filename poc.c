#define _CRT_SECURE_NO_WARNINGS

#define FNT_IMPLEMENTATION
#include "fnt.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

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
struct bmp_s {
  unsigned * data;
  int w, h;
};

static void bmp_write(struct bmp_s * bmp) {
  FILE * f = fopen("poc.bmp", "wb");

  struct bmp_hdr_s hdr = {
    .bm   = { 'B', 'M' },
    .size = sizeof(struct bmp_hdr_s) + bmp->w * bmp->h * 4,
    .res  = 0,
    .offs = sizeof(struct bmp_hdr_s),

    .hdr_sz = 12,
    .width  = bmp->w,
    .height = bmp->h,
    .planes = 1,
    .bitc   = 32,
  };
  fwrite(&hdr, 1, sizeof(hdr), f);

  fwrite(bmp->data, 1, bmp->w * bmp->h * 4, f);

  fclose(f);
}

static inline void bmp_clear(struct bmp_s * bmp) {
  for (int i = 0; i < bmp->w * bmp->h; i++) bmp->data[i] = 0xFF000000;
}

static inline void bmp_pix(struct bmp_s * bmp, int x, int y) {
  if (x < 0 || x >= bmp->w || y < 0 || y >= bmp->h) return;
  bmp->data[(bmp->h - y - 1) * bmp->w + x] = ~0;
}
static void bmp_blip(struct bmp_s * bmp, int x, int y) {
  for (int dx = -BMP_BLIP_R; dx <= BMP_BLIP_R; dx++) {
    for (int dy = -BMP_BLIP_R; dy <= BMP_BLIP_R; dy++) {
      bmp_pix(bmp, x + dx, y + dy);
    }
  }
}
static void bmp_line(void * bmp, int x1, int y1, int x2, int y2) {
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

static int count = 0;
static void dummy(void * bmp, int x1, int y1, int x2, int y2) {
  count++;
}
int main(int argc, char ** argv) {
  const char * str = argc == 2 ? argv[1] : "caô";

  fnt_pen_t p = {
    .line = dummy,
    .x = 30,
    .y = 80,
  };
  fnt_layout(&p, str);
  printf("lines: %d\n", count);

  struct bmp_s bmp = {
    .data = malloc(p.x * p.y * 4),
    .w    = p.x,
    .h    = p.y,
  };

  bmp_clear(&bmp);

  p = (fnt_pen_t) {
    .line = bmp_line,
    .ptr  = &bmp,
    .x    = 20,
    .y    = 60,
  };
  fnt_layout(&p, str);

  bmp_write(&bmp);
}
