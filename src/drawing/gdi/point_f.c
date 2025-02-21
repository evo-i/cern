#include "cern/drawing/point_f.h"
#include "cern/drawing/size_f.h"

G_DEFINE_BOXED_TYPE(CernPointF, cern_point_f, cern_point_f_copy, cern_point_f_free)

CernPointF *
cern_point_f_new(void) {
  return g_slice_new(CernPointF);
}

CernPointF *
cern_point_f_copy(CernPointF *self) {
  return self ? g_slice_dup(CernPointF, self) : NULL;
}

void
cern_point_f_free(CernPointF *self) {
  if (self) {
    g_slice_free(CernPointF, self);
  }
}

CernPointF
cern_point_f_create(gfloat x, gfloat y) {
  return (CernPointF) { .x = x, .y = y };
}

CernPointF
cern_point_f_create_with_size(CernSizeF *size) {
  return (CernPointF) {
    .x = cern_size_f_get_width(size),
    .y = cern_size_f_get_height(size)
  };
}

gfloat
cern_point_f_get_x(CernPointF *self) {
  return self->x;
}

gfloat
cern_point_f_get_y(CernPointF *self) {
  return self->y;
}

void
cern_point_f_set_x(CernPointF *self, gfloat x) {
  self->x = x;
}

void
cern_point_f_set_y(CernPointF *self, gfloat y) {
  self->y = y;
}

gboolean
cern_point_f_is_equals(CernPointF *self, CernPointF *point) {
  return (self->x == point->x) && (self->y == point->y);
}

gboolean
cern_point_f_is_empty(CernPointF *self) {
  return (self->x == 0.0f) && (self->y == 0.0f);
}

CernPointF
cern_point_f_add_point(CernPointF *self, CernPointF *point) {
  return cern_point_f_create(self->x + point->x, self->y + point->y);
}

CernPointF
cern_point_f_sub_point(CernPointF *self, CernPointF *point) {
  return cern_point_f_create(self->x - point->x, self->y - point->y);
}

CernPointF
cern_point_f_mul_point(CernPointF *self, CernPointF *point) {
  return cern_point_f_create(self->x * point->x, self->y * point->y);
}

CernPointF
cern_point_f_div_point(CernPointF *self, CernPointF *point) {
  return cern_point_f_create(self->x / point->x, self->y / point->y);
}

CernPointF
cern_point_f_add_size(CernPointF *self, CernSizeF *size) {
  return cern_point_f_create(self->x + cern_size_f_get_width(size),
                             self->y + cern_size_f_get_height(size));
}

CernPointF
cern_point_f_sub_size(CernPointF *self, CernSizeF *size) {
  return cern_point_f_create(self->x - cern_size_f_get_width(size),
                             self->y - cern_size_f_get_height(size));
}

CernPointF
cern_point_f_mul_size(CernPointF *self, CernSizeF *size) {
  return cern_point_f_create(self->x * cern_size_f_get_width(size),
                             self->y * cern_size_f_get_height(size));
}

CernPointF
cern_point_f_div_size(CernPointF *self, CernSizeF *size) {
  return cern_point_f_create(self->x / cern_size_f_get_width(size),
                             self->y / cern_size_f_get_height(size));
}

void
cern_point_f_offset(CernPointF *self, gfloat dx, gfloat dy) {
  self->x += dx;
  self->y += dy;
}

void
cern_point_f_offset_with_point(CernPointF *self, CernPointF *point) {
  self->x += point->x;
  self->y += point->y;
}
