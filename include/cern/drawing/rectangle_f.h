#ifndef CERN_RECTANGLE_F_H
#define CERN_RECTANGLE_F_H 1

#include <glib-object.h>

#include "point_f.h"
#include "size_f.h"

G_BEGIN_DECLS

typedef struct _CernRectangleF {
  gfloat x;
  gfloat y;
  gfloat width;
  gfloat height;
} CernRectangleF;

CernRectangleF
cern_rectangle_f_create(void);

CernRectangleF
cern_rectangle_f_create_with_rect(gfloat x, gfloat y, gfloat width, gfloat height);

gboolean
cern_rectangle_f_is_empty(CernRectangleF *self);

gboolean
cern_rectangle_f_is_infinite(CernRectangleF *self);

gfloat
cern_rectangle_f_get_left(CernRectangleF *self);

gfloat
cern_rectangle_f_get_top(CernRectangleF *self);

gfloat
cern_rectangle_f_get_right(CernRectangleF *self);

gfloat
cern_rectangle_f_get_bottom(CernRectangleF *self);

gfloat
cern_rectangle_f_get_x(CernRectangleF *self);

void
cern_rectangle_f_set_x(CernRectangleF *self, gfloat x);

gfloat
cern_rectangle_f_get_y(CernRectangleF *self);

void
cern_rectangle_f_set_y(CernRectangleF *self, gfloat y);

gfloat
cern_rectangle_f_get_width(CernRectangleF *self);

void
cern_rectangle_f_set_width(CernRectangleF *self, gfloat width);

gfloat
cern_rectangle_f_get_height(CernRectangleF *self);

void
cern_rectangle_f_set_height(CernRectangleF *self, gfloat height);

gboolean
cern_rectangle_f_contains_point(CernRectangleF *self, CernPointF *point);

gboolean
cern_rectangle_f_contains_rectangle(CernRectangleF *self, CernRectangleF *other);

gboolean
cern_rectangle_f_contains_coords(CernRectangleF *self, gfloat x, gfloat y);

CernPointF
cern_rectangle_f_get_location(CernRectangleF *self);

CernSizeF
cern_rectangle_f_get_size(CernRectangleF *self);

void
cern_rectangle_f_set_location(CernRectangleF *self, CernPointF *location);

gboolean
cern_rectangle_f_equals(CernRectangleF *self, CernRectangleF *other);

CernRectangleF
cern_rectangle_f_intersect(CernRectangleF *self, CernRectangleF *other);

G_END_DECLS

#endif /* CERN_RECTANGLE_F_H */
