#define _CRT_SECURE_NO_WARNINGS

#include <stdint.h>
#include <stdio.h>

#define WIDTH 128
#define HEIGHT 128

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

static void write_bmp(unsigned * bmp) {
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

int main() {
  unsigned bmp[WIDTH * HEIGHT];

  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {
      unsigned bit = 0xFFFF0000;
      bit |= x << 9;
      bit |= y << 1;
      bmp[(HEIGHT - y - 1) * WIDTH + x] = bit;
    }
  }

  write_bmp(bmp);
}
