#ifndef CERN_DRAWING_POINT_F_H
#define CERN_DRAWING_POINT_F_H 1

#include <glib-object.h>

G_BEGIN_DECLS

typedef struct _CernSizeF CernSizeF;

typedef struct _CernPointF {
  gfloat x;
  gfloat y;
} CernPointF;

#define CERN_TYPE_POINT_F (cern_point_f_get_type())

GType
cern_point_f_get_type(void);

CernPointF *
cern_point_f_copy(CernPointF *self);

void
cern_point_f_free(CernPointF *self);

CernPointF
cern_point_f_create(gfloat x, gfloat y);

CernPointF
cern_point_f_create_with_size(CernSizeF *size);

gfloat
cern_point_f_get_x(CernPointF *point);

gfloat
cern_point_f_get_y(CernPointF *point);

void
cern_point_f_set_x(CernPointF *point, gfloat x);

void
cern_point_f_set_y(CernPointF *point, gfloat y);

gboolean
cern_point_f_is_equals(CernPointF *self, CernPointF *point);

gboolean
cern_point_f_is_empty(CernPointF *self);

CernPointF
cern_point_f_add_point(CernPointF *self, CernPointF *point);

CernPointF
cern_point_f_sub_point(CernPointF *self, CernPointF *point);

CernPointF
cern_point_f_mul_point(CernPointF *self, CernPointF *point);

CernPointF
cern_point_f_div_point(CernPointF *self, CernPointF *point);

CernPointF
cern_point_f_add_size(CernPointF *self, CernSizeF *size);

CernPointF
cern_point_f_sub_size(CernPointF *self, CernSizeF *size);

CernPointF
cern_point_f_mul_size(CernPointF *self, CernSizeF *size);

CernPointF
cern_point_f_div_size(CernPointF *self, CernSizeF *size);

void
cern_point_f_offset(CernPointF *self, gfloat dx, gfloat dy);

void
cern_point_f_offset_with_point(CernPointF *self, CernPointF *point);

G_END_DECLS

#endif /* CERN_DRAWING_POINT_F_H */
