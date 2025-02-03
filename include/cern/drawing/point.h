#ifndef CERN_DRAWING_POINT_H
#define CERN_DRAWING_POINT_H 1

#include <glib-object.h>

G_BEGIN_DECLS

typedef struct _CernSize CernSize;

typedef struct _CernPoint {
  gint32 x;
  gint32 y;
} CernPoint;

#define CERN_TYPE_POINT (cern_point_get_type())

GType
cern_point_get_type (void);

CernPoint *
cern_point_copy (CernPoint *self);

void
cern_point_free (CernPoint *self);

CernPoint
cern_point_create(gint32 x, gint32 y);

CernPoint
cern_point_create_with_size(CernSize *size);

gint32
cern_point_get_x(CernPoint *point);

gint32
cern_point_get_y(CernPoint *point);

void
cern_point_set_x(CernPoint *point, gint32 x);

void
cern_point_set_y(CernPoint *point, gint32 y);

gboolean
cern_point_is_equals(CernPoint *self, CernPoint *point);

gboolean
cern_point_is_empty(CernPoint *self);

CernPoint
cern_point_add_point(CernPoint *self, CernPoint *point);

CernPoint
cern_point_sub_point(CernPoint *self, CernPoint *point);

CernPoint
cern_point_mul_point(CernPoint *self, CernPoint *point);

CernPoint
cern_point_div_point(CernPoint *self, CernPoint *point);

CernPoint
cern_point_add_size(CernPoint *self, CernSize *size);

CernPoint
cern_point_sub_size(CernPoint *self, CernSize *size);

CernPoint
cern_point_mul_size(CernPoint *self, CernSize *size);

CernPoint
cern_point_div_size(CernPoint *self, CernSize *size);

void
cern_point_offset(CernPoint *self, gint32 dx, gint32 dy);

void
cern_point_offset_with_point(CernPoint *self, CernPoint *point);

G_END_DECLS

#endif /* CERN_DRAWING_POINT_H */
