#include "cern/drawing/rectangle_f.h"
#include "cern/drawing/point_f.h"

CernRectangleF
cern_rectangle_f_create(void) {
  return (CernRectangleF) { .0f, .0f, .0f, .0f };
}

CernRectangleF
cern_rectangle_f_create_with_rect(gfloat x, gfloat y, gfloat width, gfloat height) {
  return (CernRectangleF) {
    .x = x,
    .y = y,
    .width = width,
    .height = height,
  };
}

gfloat
cern_rectangle_f_get_x(CernRectangleF *self) {
  return self->x;
}

void
cern_rectangle_f_set_x(CernRectangleF *self, gfloat x) {
  self->x = x;
}

gfloat
cern_rectangle_f_get_y(CernRectangleF *self) {
  return self->y;
}

void
cern_rectangle_f_set_y(CernRectangleF *self, gfloat y) {
  self->y = y;
}

gfloat
cern_rectangle_f_get_width(CernRectangleF *self) {
  return self->width;
}

void
cern_rectangle_f_set_width(CernRectangleF *self, gfloat width) {
  self->width = width;
}

gfloat
cern_rectangle_f_get_height(CernRectangleF *self) {
  return self->height;
}

void
cern_rectangle_f_set_height(CernRectangleF *self, gfloat height) {
  self->height = height;
}

void
cern_rectangle_f_set_position(CernRectangleF *self, gfloat x, gfloat y) {
  self->x = x;
  self->y = y;
}

void
cern_rectangle_f_set_size(CernRectangleF *self, gfloat width, gfloat height) {
  self->width = width;
  self->height = height;
}

CernRectangleF
cern_rectangle_f_intersect(CernRectangleF *self, CernRectangleF *other) {
  gfloat x1 = MAX(self->x, other->x);
  gfloat y1 = MAX(self->y, other->y);
  gfloat x2 = MIN(self->x + self->width, other->x + other->width);
  gfloat y2 = MIN(self->y + self->height, other->y + other->height);

  if (x2 >= x1 && y2 >= y1) {
    return
      cern_rectangle_f_create_with_rect(x1, y1, x2 - x1, y2 - y1);
  }

  return (CernRectangleF) { 0 };
}

gboolean
cern_rectangle_f_equals(CernRectangleF *self, CernRectangleF *other) {
  return
    (self->x == other->x &&
      self->y == other->y &&
      self->width == other->width &&
      self->height == other->height);
}

gboolean
cern_rectangle_f_is_empty(CernRectangleF *self) {
  return (self->width <= 0 || self->height <= 0);
}

gboolean
cern_rectangle_f_is_infinite(CernRectangleF *self) {
  return (self->width == G_MAXFLOAT && self->height == G_MAXFLOAT);
}

gfloat
cern_rectangle_f_get_left(CernRectangleF *self) {
  return self->x;
}

gfloat
cern_rectangle_f_get_top(CernRectangleF *self) {
  return self->y;
}

gfloat
cern_rectangle_f_get_right(CernRectangleF *self) {
  return self->x + self->width;
}

gfloat
cern_rectangle_f_get_bottom(CernRectangleF *self) {
  return self->y + self->height;
}

gboolean
cern_rectangle_f_contains_point(CernRectangleF *self, CernPointF *point) {
  return
    cern_point_f_get_x(point) >= self->x
      && cern_point_f_get_x(point) <= self->x + self->width
      && cern_point_f_get_y(point) >= self->y
      && cern_point_f_get_y(point) <= self->y + self->height;
}

gboolean
cern_rectangle_f_contains_rectangle(CernRectangleF *self, CernRectangleF *other) {
  return
    self->x <= other->x
      && self->y <= other->y
      && self->x + self->width >= other->x + other->width
      && self->y + self->height >= other->y + other->height;
}

gboolean
cern_rectangle_f_contains_coords(CernRectangleF *self, gfloat x, gfloat y) {
  return
    x >= self->x
      && x <= self->x + self->width
      && y >= self->y
      && y <= self->y + self->height;
}

CernPointF
cern_rectangle_f_get_location(CernRectangleF *self) {
  return cern_point_f_create(self->x, self->y);
}

void
cern_rectangle_f_set_location(CernRectangleF *self, CernPointF *location) {
  self->x = cern_point_f_get_x(location);
  self->y = cern_point_f_get_y(location);
}

CernSizeF
cern_rectangle_f_get_size(CernRectangleF *self) {
  return cern_size_f_create(self->width, self->height);
}
