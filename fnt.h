#ifndef FNT_H
#define FNT_H

typedef struct fnt_metrics_s {
  int min_x, min_y;
  int max_x, max_y;
} fnt_metrics_t;

typedef struct fnt_pen_s {
  void (*line)(void * ptr, int x1, int y1, int x2, int y2);
  void * ptr;

  double x;
  double y;
  int angle;
} fnt_pen_t;

void fnt_layout(fnt_pen_t * pen, const char * str);
void fnt_metrics(fnt_metrics_t * m, const char * str);

#ifdef FNT_IMPLEMENTATION
#include <math.h>

static inline double fnt_d2r(double a) {
  return (a * 3.141592658979323) / 180.;
}
static inline void fnt_pen_m(fnt_pen_t * p, int d) {
  p->x += cos(fnt_d2r(p->angle)) * d;
  p->y += sin(fnt_d2r(p->angle)) * d;
}
static inline void fnt_pen_f(fnt_pen_t * p, int d) {
  double tx = p->x + cos(fnt_d2r(p->angle)) * d;
  double ty = p->y + sin(fnt_d2r(p->angle)) * d;
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

static int fnt_decode_utf8(const char ** str) {
  unsigned char a = *(*str)++;
  if ((a & 0x80) == 0) return a;

  int n = 0;
  if ((a & 0xE0) == 0xC0) {
    n = 1;
    a &= ~0xE0;
  } else if ((a & 0xF0) == 0xE0) {
    n = 2;
    a &= ~0xF0;
  } else if ((a & 0xF8) == 0xF0) {
    n = 3;
    a &= ~0xF8;
  } else {
    return 0;
  }

  int res = a;
  for (int i = 0; i < n; i++) {
    unsigned char a = *(*str)++;
    if ((a & 0xC0) != 0x80) return 0;
    res = (res << 6) | (a & ~0xC0);
  }
  return res;
}
void fnt_layout(fnt_pen_t * p, const char * str) {
  for (const char * c = str; *c;) {
    int uni = fnt_decode_utf8(&c);
    switch (uni) {
      case ' ': fnt_space(p); break;

      case 'a': fnt_a(p); break;
      case 'e': fnt_e(p); break;
      case 't': fnt_t(p); break;

      case '0': fnt_0(p); break;
      case '1': fnt_1(p); break;

      case 0xe2: fnt_a_circ(p); break;
      case 0xea: fnt_e_circ(p); break;
    }
  }
}

static void fnt_metrics_fn(void * ptr, int x1, int y1, int x2, int y2) {
  fnt_metrics_t * m = ptr;

  if (x1 < m->min_x) m->min_x = x1;
  if (x2 < m->min_x) m->min_x = x2;
  if (y1 < m->min_y) m->min_y = y1;
  if (y2 < m->min_y) m->min_y = y2;

  if (x1 > m->max_x) m->max_x = x1;
  if (x2 > m->max_x) m->max_x = x2;
  if (y1 > m->max_y) m->max_y = y1;
  if (y2 > m->max_y) m->max_y = y2;
}
void fnt_metrics(fnt_metrics_t * m, const char * str) {
  *m = (fnt_metrics_t){0};

  fnt_pen_t p = {
    .line = fnt_metrics_fn,
    .ptr  = m,
  };
  fnt_layout(&p, str);
}

#endif
#endif
