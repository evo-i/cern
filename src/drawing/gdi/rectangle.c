#include "cern/drawing/rectangle.h"
#include "cern/drawing/point.h"
#include "cern/drawing/size.h"
#include <glib-object.h>
#include <glib.h>
#include <winperf.h>

G_DEFINE_BOXED_TYPE(CernRectangle, cern_rectangle, cern_rectangle_copy, cern_rectangle_free);

CernRectangle *
cern_rectangle_new(void) {
  return g_new0(CernRectangle, 1);
}

CernRectangle *
cern_rectangle_copy(CernRectangle *self) {
  return g_memdup2(self, sizeof(CernRectangle));
}

void
cern_rectangle_free(CernRectangle *self) {
  g_free(self);
}

CernRectangle
cern_rectangle_create(gint32 x, gint32 y, gint32 width, gint32 height) {
  return (CernRectangle) {
    .x = x,
    .y = y,
    .width = width,
    .height = height
  };
}

CernRectangle
cern_rectangle_create_with_ltrb(gint32 left, gint32 top, gint32 right, gint32 bottom) {
  return (CernRectangle) {
    .x = left,
    .y = top,
    .width = right - left,
    .height = bottom - top
  };
}

CernRectangle
cern_rectangle_create_with_point_and_size(CernPoint *point, CernSize *size) {
  return (CernRectangle) {
    .x = cern_point_get_x(point),
    .y = cern_point_get_y(point),
    .width = cern_size_get_width(size),
    .height = cern_size_get_height(size)
  };
}

CernRectangle
cern_rectangle_empty(void) {
  return (CernRectangle) { 0 };
}

gint32
cern_rectangle_get_x(CernRectangle *self) {
  return self->x;
}

void
cern_rectangle_set_x(CernRectangle *self, gint32 x) {
  self->x = x;
}

gint32
cern_rectangle_get_y(CernRectangle *self) {
  return self->y;
}

void
cern_rectangle_set_y(CernRectangle *self, gint32 y) {
  self->y = y;
}

gint32
cern_rectangle_get_width(CernRectangle *self) {
  return self->width;
}

void
cern_rectangle_set_width(CernRectangle *self, gint32 width) {
  self->width = width;
}

gint32
cern_rectangle_get_height(CernRectangle *self) {
  return self->height;
}

void
cern_rectangle_set_height(CernRectangle *self, gint32 height) {
  self->height = height;
}

gint32
cern_rectangle_get_left(CernRectangle *self) {
  return self->x;
}

gint32
cern_rectangle_get_top(CernRectangle *self) {
  return self->y;
}

gint32
cern_rectangle_get_right(CernRectangle *self) {
  return self->x + self->width;
}

gint32
cern_rectangle_get_bottom(CernRectangle *self) {
  return self->y + self->height;
}

CernSize
cern_rectangle_get_size(CernRectangle *self) {
  return
    cern_size_create(self->width, self->height);
}

void
cern_rectangle_set_size(CernRectangle *self, CernSize *size) {
  self->width = size->width;
  self->height = size->height;
}

CernPoint
cern_rectangle_get_location(CernRectangle *self) {
  return cern_point_create(self->x, self->y);
}

gboolean
cern_rectangle_is_empty(CernRectangle *self) {
  return
    self == NULL
      || (self->width == 0
          || self->height == 0);
}

/* offset */

void
cern_rectangle_offset(CernRectangle *self, gint32 x, gint32 y) {
  self->x += x;
  self->y += y;
}

void
cern_rectangle_offset_with_point(CernRectangle *self, CernPoint *point) {
  self->x += cern_point_get_x(point);
  self->y += cern_point_get_y(point);
}

/* contains */

gboolean
cern_rectangle_contains_point(CernRectangle *self, CernPoint *point) {
  return
    cern_point_get_x(point) >= self->x
      && cern_point_get_x(point) <= self->x + self->width
      && cern_point_get_y(point) >= self->y
      && cern_point_get_y(point) <= self->y + self->height;
}

gboolean
cern_rectangle_contains_rectangle(CernRectangle *self, CernRectangle *rectangle) {
  g_critical("Not implemented");
  return FALSE;
}

gboolean
cern_rectangle_contains_coords(CernRectangle *self, gint32 x, gint32 y) {
  return
    x >= self->x
      && x <= self->x + self->width
      && y >= self->y
      && y <= self->y + self->height;
}

/* intersect */

void
cern_rectangle_intersect(CernRectangle *self, CernRectangle *rectangle) {
  g_critical("Not implemented");
}

gboolean
cern_rectangle_intersect_with(CernRectangle *self, CernRectangle *rectangle) {
  g_critical("Not implemented");
  return FALSE;
}

/* inflate */

void
cern_rectangle_inflate(CernRectangle *self, gint32 width, gint32 height) {
  self->width += width;
  self->height += height;
}

void
cern_rectangle_inflate_with_size(CernRectangle *self, CernSize *size) {
  self->width += cern_size_get_width(size);
  self->height += cern_size_get_height(size);
}

gboolean
cern_rectangle_equals(CernRectangle *lhs, CernRectangle *rhs) {
  return 
    lhs->x == rhs->x
      && lhs->y == rhs->y
      && lhs->width == rhs->width
      && lhs->height == rhs->height;
}

