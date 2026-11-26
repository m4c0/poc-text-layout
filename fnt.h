#ifndef FNT_H
#define FNT_H

typedef struct fnt_pen_s {
  void (*line)(void * ptr, int x1, int y1, int x2, int y2);
  void * ptr;

  float x;
  float y;
  int a;
} fnt_pen_t;

void fnt_layout(fnt_pen_t * pen, const char * str);

#ifdef FNT_IMPLEMENTATION
#include <math.h>

#define PI 3.1415926535897
static void fnt_pen_m(fnt_pen_t * p, int d) {
  p->x += cos((p->a * PI) / 180.f) * d;
  p->y += sin((p->a * PI) / 180.f) * d;
}
static void fnt_pen_f(fnt_pen_t * p, int d) {
  float tx = p->x + cos((p->a * PI) / 180.f) * d;
  float ty = p->y + sin((p->a * PI) / 180.f) * d;
  (p->line)(p->ptr, p->x, p->y, tx, ty);
  p->x = tx;
  p->y = ty;
}
static void fnt_pen_r(fnt_pen_t * p, int a) {
  p->a = (p->a - a + 360) % 360;
}
#undef PI

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


#endif
#endif
