#include "cern/drawing/point_f.h"

G_DEFINE_BOXED_TYPE(CernPointF, cern_point_f, cern_point_f_copy, cern_point_f_free)

CernPointF *
cern_point_f_new(void) {
  return g_new0(CernPointF, 1);
}

CernPointF *
cern_point_f_copy(CernPointF *self) {
  return g_memdup2(self, sizeof(CernPointF));
}

void
cern_point_f_free(CernPointF *self) {
  g_free(self);
}

CernPointF
cern_point_f_create(void) {
  return (CernPointF) { .0f };
}

CernPointF
cern_point_f_create_with_coords(gfloat x, gfloat y) {
  return (CernPointF) {
    .x = x,
    .y = y
  };
}

gfloat
cern_point_f_get_x(CernPointF *self) {
  return self->x;
}

void
cern_point_f_set_x(CernPointF *self, gfloat x) {
  self->x = x;
}

gfloat
cern_point_f_get_y(CernPointF *self) {
  return self->y;
}

void
cern_point_f_set_y(CernPointF *self, gfloat y) {
  self->y = y;
}
