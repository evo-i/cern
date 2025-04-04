#include <glib.h>
#ifndef CERN_RECTANGLE_H
#define CERN_RECTANGLE_H 1

#include <glib-object.h>

G_BEGIN_DECLS

typedef struct _CernPoint CernPoint;
typedef struct _CernSize CernSize;

typedef struct _CernRectangle {
  gint32 x;
  gint32 y;
  gint32 width;
  gint32 height;
} CernRectangle;

#define CERN_TYPE_RECTANGLE cern_rectangle_get_type ()

CernRectangle *
cern_rectangle_new(void);

CernRectangle *
cern_rectangle_copy(CernRectangle *self);

void
cern_rectangle_free(CernRectangle *self);

CernRectangle
cern_rectangle_empty(void);

CernRectangle
cern_rectangle_create(gint32 x, gint32 y, gint32 width, gint32 height);

CernRectangle
cern_rectangle_create_with_ltrb(gint32 left, gint32 top, gint32 right, gint32 bottom);

CernRectangle
cern_rectangle_create_with_point_and_size(CernPoint *point, CernSize *size);

gint32
cern_rectangle_get_x(CernRectangle *self);

void
cern_rectangle_set_x(CernRectangle *self, gint32 x);

gint32
cern_rectangle_get_y(CernRectangle *self);

void
cern_rectangle_set_y(CernRectangle *self, gint32 y);

gint32
cern_rectangle_get_width(CernRectangle *self);

void
cern_rectangle_set_width(CernRectangle *self, gint32 width);

gint32
cern_rectangle_get_height(CernRectangle *self);

void
cern_rectangle_set_height(CernRectangle *self, gint32 height);

gint32
cern_rectangle_get_left(CernRectangle *self);

gint32
cern_rectangle_get_top(CernRectangle *self);

gint32
cern_rectangle_get_right(CernRectangle *self);

gint32
cern_rectangle_get_bottom(CernRectangle *self);

CernSize
cern_rectangle_get_size(CernRectangle *self);

void
cern_rectangle_set_size(CernRectangle *self, CernSize *size);

CernPoint
cern_rectangle_get_location(CernRectangle *self);

void
cern_rectangle_set_location(CernRectangle *self, CernPoint *location);

gboolean
cern_rectangle_is_empty(CernRectangle *self);

/* offset */

void
cern_rectangle_offset(CernRectangle *self, gint32 x, gint32 y);

void
cern_rectangle_offset_with_point(CernRectangle *self, CernPoint *point);

/* contains */

gboolean
cern_rectangle_contains_point(CernRectangle *self, CernPoint *point);

gboolean
cern_rectangle_contains_rectangle(CernRectangle *self, CernRectangle *rectangle);

gboolean
cern_rectangle_contains_coords(CernRectangle *self, gint32 x, gint32 y);

/* intersect */

void
cern_rectangle_intersect(CernRectangle *self, CernRectangle *rectangle);

gboolean
cern_rectangle_intersect_with(CernRectangle *self, CernRectangle *rectangle);

/* inflate */

void
cern_rectangle_inflate(CernRectangle *self, gint32 width, gint32 height);

void
cern_rectangle_inflate_with_size(CernRectangle *self, CernSize *size);

gboolean
cern_rectangle_equals(CernRectangle *lhs, CernRectangle *rhs);

G_END_DECLS

#endif /* CERN_RECTANGLE_H */
