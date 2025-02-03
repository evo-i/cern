#ifndef CERN_POINT_F_H
#define CERN_POINT_F_H 1

#include <glib-object.h>

G_BEGIN_DECLS

typedef struct _CernPointF {
  gfloat x;
  gfloat y;
} CernPointF;

#define CERN_TYPE_POINT_F (cern_point_f_get_type())

GType
cern_point_f_get_type(void);

CernPointF *
cern_point_f_new(void);

CernPointF *
cern_point_f_copy(CernPointF *self);

void
cern_point_f_free(CernPointF *self);

CernPointF
cern_point_f_create(void);

CernPointF
cern_point_f_create_with_coords(gfloat x, gfloat y);

gfloat
cern_point_f_get_x(CernPointF *self);

void
cern_point_f_set_x(CernPointF *self, gfloat x);

gfloat
cern_point_f_get_y(CernPointF *self);

void
cern_point_f_set_y(CernPointF *self, gfloat y);

G_END_DECLS

#endif /* CERN_POINT_F_H */
