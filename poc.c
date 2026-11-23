#define _CRT_SECURE_NO_WARNINGS

#include <stdint.h>
#include <stdio.h>

#define WIDTH 128
#define HEIGHT 128

#define SWAP(a, b) do { a ^= b; b ^= a; a ^= b; } while (0)

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
  if (x < 0 || x >= WIDTH / 2 || y < 0 || y >= HEIGHT / 2) return;
  bmp[(HEIGHT - y - 1) * WIDTH + x] = ~0;
}
static void bmp_line(unsigned * bmp, int x1, int y1, int x2, int y2) {
  if (x1 > x2) SWAP(x1, x2);
  if (y1 > y2) SWAP(y1, y2);

  int dx = x2 - x1;
  int dy = y2 - y1;

  if (dx > dy) {
    for (int x = x1; x <= x2; x++) {
      int y = y1 + (x - x1) * dy / dx;
      bmp_pix(bmp, x, y);
    }
  } else {
    for (int y = y1; y <= y2; y++) {
      int x = x1 + (y - y1) * dx / dy;
      bmp_pix(bmp, x, y);
    }
  }
}

int main() {
  unsigned bmp[WIDTH * HEIGHT];

  bmp_clear(bmp);

  bmp_line(bmp, 20, 30, 100, 50);

  bmp_write(bmp);
}
