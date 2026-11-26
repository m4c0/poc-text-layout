#ifndef FNT_H
#define FNT_H

typedef struct fnt_pen_s {
  void (*line)(void * ptr, int x1, int y1, int x2, int y2);
  void * ptr;

  float x;
  float y;
  int angle;
} fnt_pen_t;

int  fnt_x_advance(const char * str);
void fnt_layout(fnt_pen_t * pen, const char * str);

#ifdef FNT_IMPLEMENTATION
#include <math.h>

static inline float fnt_d2r(float a) {
  return (a * 3.141592658979323) / 180.f;
}
static inline void fnt_pen_m(fnt_pen_t * p, int d) {
  p->x += cos(fnt_d2r(p->angle)) * d;
  p->y += sin(fnt_d2r(p->angle)) * d;
}
static inline void fnt_pen_f(fnt_pen_t * p, int d) {
  float tx = p->x + cos(fnt_d2r(p->angle)) * d;
  float ty = p->y + sin(fnt_d2r(p->angle)) * d;
  (p->line)(p->ptr, p->x, p->y, tx, ty);
  p->x = tx;
  p->y = ty;
}
static inline void fnt_pen_r(fnt_pen_t * p, int a) {
  p->angle = (p->angle - a + 360) % 360;
}

static void fnt_space(fnt_pen_t * p) {
  fnt_pen_m(p, 20);
}

static void fnt_0(fnt_pen_t * p) {
  fnt_pen_f(p, 20);
  fnt_pen_r(p, 90); fnt_pen_f(p, 40);
  fnt_pen_r(p, 90); fnt_pen_f(p, 20);
  fnt_pen_r(p, 90); fnt_pen_f(p, 40);
  fnt_pen_r(p, 90); fnt_pen_m(p, 10);
  fnt_pen_r(p, 90); fnt_pen_m(p, 20);
  fnt_pen_f(p, 0);

  fnt_pen_m(p, -20);
  fnt_pen_r(p, -90); fnt_pen_m(p, 20);
}
static void fnt_1(fnt_pen_t * p) {
  fnt_pen_m(p, 10); fnt_pen_r(p, 90);
  fnt_pen_f(p, 40); fnt_pen_r(p, 90);
  fnt_pen_f(p, 10);

  fnt_pen_r(p, 90); fnt_pen_m(p, 40);
  fnt_pen_r(p, 90); fnt_pen_m(p, 20);
}
static void fnt_a(fnt_pen_t * p) {
  fnt_pen_r(p,  90); fnt_pen_m(p, 30);
  fnt_pen_r(p, -90); fnt_pen_f(p, 20);
  fnt_pen_r(p, -90); fnt_pen_f(p, 30);
  fnt_pen_r(p, -90); fnt_pen_f(p, 20);
  fnt_pen_r(p, -90); fnt_pen_f(p, 20);
  fnt_pen_r(p, -90); fnt_pen_f(p, 20);
  fnt_pen_r(p, -90); fnt_pen_m(p, 20);
  fnt_pen_r(p,  90); fnt_pen_m(p, 10);
}
static void fnt_e(fnt_pen_t * p) {
  fnt_pen_r(p,  90); fnt_pen_m(p, 10);
  fnt_pen_r(p, -90); fnt_pen_f(p, 20);
  fnt_pen_r(p,  90); fnt_pen_f(p, 20);
  fnt_pen_r(p,  90); fnt_pen_f(p, 20);
  fnt_pen_r(p,  90); fnt_pen_f(p, 30);
  fnt_pen_r(p,  90); fnt_pen_f(p, 20);
  fnt_pen_m(p,  10);
}
static void fnt_t(fnt_pen_t * p) {
  fnt_pen_f(p,  15);
  fnt_pen_m(p, -15);
  fnt_pen_r(p,  90); fnt_pen_f(p, 40);
  fnt_pen_m(p, -10);
  fnt_pen_r(p, -90); fnt_pen_f(p, 10);
  fnt_pen_m(p,  15);
  fnt_pen_r(p, -90); fnt_pen_m(p, 30);
  fnt_pen_r(p, 90);
}

static void fnt_circ(fnt_pen_t * p) {
  fnt_pen_m(p,   5);
  fnt_pen_r(p,  90); fnt_pen_f(p,  5);
  fnt_pen_r(p, -90); fnt_pen_f(p, 10);
  fnt_pen_r(p, -90); fnt_pen_f(p,  5);
  fnt_pen_r(p,  90); fnt_pen_m(p, 15);
}

static void fnt_comp_circ(fnt_pen_t * p) {
  fnt_pen_m(p, -30);
  fnt_pen_r(p,  90); fnt_pen_m(p, 40);
  fnt_pen_r(p, -90);

  fnt_circ(p);

  fnt_pen_r(p, -90); fnt_pen_m(p, 40);
  fnt_pen_r(p,  90);
}
static void fnt_a_circ(fnt_pen_t * p) {
  fnt_a(p);
  fnt_comp_circ(p);
}
static void fnt_e_circ(fnt_pen_t * p) {
  fnt_e(p);
  fnt_comp_circ(p);
}

void fnt_layout(fnt_pen_t * p, const char * str) {
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

static void fnt_dummy(void * bmp, int x1, int y1, int x2, int y2) {}
int fnt_x_advance(const char * str) {
  fnt_pen_t p = { .line = fnt_dummy };
  fnt_layout(&p, str);
  return p.x;
}

#endif
#endif
