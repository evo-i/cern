#include "cern/drawing/point.h"
#include "cern/drawing/size.h"

G_DEFINE_BOXED_TYPE(CernPoint, cern_point, cern_point_copy, cern_point_free)

CernPoint *
cern_point_new(void) {
  return g_new0(CernPoint, 1);
}

CernPoint *
cern_point_copy(CernPoint *self) {
  return g_memdup2(self, sizeof(CernPoint));
}

void
cern_point_free(CernPoint *self) {
  g_free(self);
}

CernPoint
cern_point_create(gint32 x, gint32 y) {
  return (CernPoint) { .x = x, .y = y };
}

CernPoint
cern_point_create_with_size(CernSize *size) {
return
  (CernPoint) {
    .x = cern_size_get_width(size),
    .y = cern_size_get_height(size)
  };
}

gint32
cern_point_get_x(CernPoint *self) {
  return self->x;
}

gint32
cern_point_get_y(CernPoint *self) {
  return self->y;
}

void
cern_point_set_x(CernPoint *self, gint32 x) {
  self->x = x;
}

void
cern_point_set_y(CernPoint *self, gint32 y) {
  self->y = y;
}

gboolean
cern_point_is_equals(CernPoint *self, CernPoint *point) {
  return
    self->x == cern_point_get_x(point)
      && self->y == cern_point_get_y(point);
}

gboolean
cern_point_is_empty(CernPoint *self) {
  return self->x == 0 && self->y == 0;
}

CernPoint
cern_point_add_point(CernPoint *self, CernPoint *point) {
  gint32 x = cern_point_get_x(point);
  gint32 y = cern_point_get_y(point);
  return
    cern_point_create(self->x + x, self->y + y);
}

CernPoint
cern_point_sub_point(CernPoint *self, CernPoint *point) {
  gint32 x = cern_point_get_x(point);
  gint32 y = cern_point_get_y(point);
  return
    cern_point_create(self->x - x, self->y - y);
}

CernPoint
cern_point_mul_point(CernPoint *self, CernPoint *point) {
  gint32 x = cern_point_get_x(point);
  gint32 y = cern_point_get_y(point);
  return
    cern_point_create(self->x * x, self->y * y);
}

CernPoint
cern_point_div_point(CernPoint *self, CernPoint *point) {
  gint32 x = cern_point_get_x(point);
  gint32 y = cern_point_get_y(point);
  return
    cern_point_create(self->x / x, self->y / y);
}

CernPoint
cern_point_add_size(CernPoint *self, CernSize *size) {
  gint32 width = cern_size_get_width(size);
  gint32 height = cern_size_get_height(size);
  return
    cern_point_create(self->x + width, self->y + height);
}

CernPoint
cern_point_sub_size(CernPoint *self, CernSize *size) {
  gint32 width = cern_size_get_width(size);
  gint32 height = cern_size_get_height(size);
  return
    cern_point_create(self->x - width, self->y - height);
}

CernPoint
cern_point_mul_size(CernPoint *self, CernSize *size) {
  gint32 width = cern_size_get_width(size);
  gint32 height = cern_size_get_height(size);
  return
    cern_point_create(self->x * width, self->y * height);
}

CernPoint
cern_point_div_size(CernPoint *self, CernSize *size) {
  gint32 width = cern_size_get_width(size);
  gint32 height = cern_size_get_height(size);
  return
    cern_point_create(self->x / width, self->y / height);
}

void
cern_point_offset(CernPoint *self, gint32 dx, gint32 dy) {
  self->x += dx;
  self->y += dy;
}

void
cern_point_offset_with_point(CernPoint *self, CernPoint *point) {
  gint32 x = cern_point_get_x(point);
  gint32 y = cern_point_get_y(point);
  self->x += x;
  self->y += y;
}
