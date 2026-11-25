#define _CRT_SECURE_NO_WARNINGS

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

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

struct pen_s {
  void (*line)(void * ptr, int x1, int y1, int x2, int y2);
  void * ptr;

  float x;
  float y;
  int a;
};

static void pen_m(struct pen_s * p, int d) {
  p->x += cos((p->a * PI) / 180.f) * d;
  p->y += sin((p->a * PI) / 180.f) * d;
}
static void pen_f(struct pen_s * p, int d) {
  float tx = p->x + cos((p->a * PI) / 180.f) * d;
  float ty = p->y + sin((p->a * PI) / 180.f) * d;
  (p->line)(p->ptr, p->x, p->y, tx, ty);
  p->x = tx;
  p->y = ty;
}
static void pen_r(struct pen_s * p, int a) {
  p->a = (p->a - a + 360) % 360;
}

static void fnt_space(struct pen_s * p) {
  pen_m(p, 20);
}

static void fnt_0(struct pen_s * p) {
  pen_f(p, 20);
  pen_r(p, 90); pen_f(p, 40);
  pen_r(p, 90); pen_f(p, 20);
  pen_r(p, 90); pen_f(p, 40);
  pen_r(p, 90); pen_m(p, 10);
  pen_r(p, 90); pen_m(p, 20);
  pen_f(p, 0);

  pen_m(p, -20);
  pen_r(p, -90); pen_m(p, 20);
}
static void fnt_1(struct pen_s * p) {
  pen_m(p, 10); pen_r(p, 90);
  pen_f(p, 40); pen_r(p, 90);
  pen_f(p, 10);

  pen_r(p, 90); pen_m(p, 40);
  pen_r(p, 90); pen_m(p, 20);
}
static void fnt_a(struct pen_s * p) {
  pen_r(p,  90); pen_m(p, 30);
  pen_r(p, -90); pen_f(p, 20);
  pen_r(p, -90); pen_f(p, 30);
  pen_r(p, -90); pen_f(p, 20);
  pen_r(p, -90); pen_f(p, 20);
  pen_r(p, -90); pen_f(p, 20);
  pen_r(p, -90); pen_m(p, 20);
  pen_r(p,  90); pen_m(p, 10);
}
static void fnt_e(struct pen_s * p) {
  pen_r(p,  90); pen_m(p, 10);
  pen_r(p, -90); pen_f(p, 20);
  pen_r(p,  90); pen_f(p, 20);
  pen_r(p,  90); pen_f(p, 20);
  pen_r(p,  90); pen_f(p, 30);
  pen_r(p,  90); pen_f(p, 20);
  pen_m(p,  10);
}
static void fnt_t(struct pen_s * p) {
  pen_f(p,  15);
  pen_m(p, -15);
  pen_r(p,  90); pen_f(p, 40);
  pen_m(p, -10);
  pen_r(p, -90); pen_f(p, 10);
  pen_m(p,  15);
  pen_r(p, -90); pen_m(p, 30);
  pen_r(p, 90);
}

static void fnt_circ(struct pen_s * p) {
  pen_m(p,   5);
  pen_r(p,  90); pen_f(p,  5);
  pen_r(p, -90); pen_f(p, 10);
  pen_r(p, -90); pen_f(p,  5);
  pen_r(p,  90); pen_m(p, 15);
}

static void fnt_comp_circ(struct pen_s * p) {
  pen_m(p, -30);
  pen_r(p,  90); pen_m(p, 40);
  pen_r(p, -90);

  fnt_circ(p);

  pen_r(p, -90); pen_m(p, 40);
  pen_r(p,  90);
}
static void fnt_a_circ(struct pen_s * p) {
  fnt_a(p);
  fnt_comp_circ(p);
}
static void fnt_e_circ(struct pen_s * p) {
  fnt_e(p);
  fnt_comp_circ(p);
}

static void layout(struct pen_s * p, const char * str) {
  for (const char * c = str; *c; c++) {
    switch ((unsigned char)*c) {
      case ' ': fnt_space(p); break;

      case 'a': fnt_a(p); break;
      case 'e': fnt_e(p); break;
      case 't': fnt_t(p); break;

      case '0': fnt_0(p); break;
      case '1': fnt_1(p); break;

      case 0xc3: {
        c++;
        switch ((unsigned char)*c) {
          case 0xa2: fnt_a_circ(p); break;
          case 0xaa: fnt_e_circ(p); break;
        }
        break;
      }
    }
  }
}

static int count = 0;
static void dummy(void * bmp, int x1, int y1, int x2, int y2) {
  count++;
}
int main(int argc, char ** argv) {
  const char * str = argc == 2 ? argv[1] : "caô";

  struct pen_s p = {
    .line = dummy,
    .x = 30,
    .y = 80,
  };
  layout(&p, str);
  printf("lines: %d\n", count);

  struct bmp_s bmp = {
    .data = malloc(p.x * p.y * 4),
    .w    = p.x,
    .h    = p.y,
  };

  bmp_clear(&bmp);

  p = (struct pen_s) {
    .line = bmp_line,
    .ptr  = &bmp,
    .x    = 20,
    .y    = 60,
  };
  layout(&p, str);

  bmp_write(&bmp);
}
