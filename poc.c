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

int main() {
  unsigned bmp[WIDTH * HEIGHT];

  bmp_clear(bmp);



  bmp_write(bmp);
}
