#ifndef CERN_DRAWING_2D_MATRIX_H
#define CERN_DRAWING_2D_MATRIX_H 1

#include <glib-object.h>
#include "matrix_order.h"
#include "../point.h"
#include "../point_f.h"
#include "../rectangle.h"
#include "../rectangle_f.h"

G_BEGIN_DECLS

#define CERN_TYPE_MATRIX (cern_matrix_get_type())

G_DECLARE_FINAL_TYPE(CernMatrix, cern_matrix, CERN, MATRIX, GObject)


CernMatrix *
cern_matrix_new(void);

CernMatrix *
cern_matrix_new_from_values(gfloat m11, gfloat m12, gfloat m21, gfloat m22,
                            gfloat dx, gfloat dy);

CernMatrix *
cern_matrix_new_from_rectangle_and_points(CernRectangleF *rect,
                                          CernPointF *points, gsize points_count);

CernMatrix *
cern_matrix_new_from_rectangle_and_points_i(CernRectangle *rect,
                                            CernPoint *points, gsize points_count);

CernMatrix *
cern_matrix_clone(CernMatrix *self);

void
cern_matrix_get_elements(CernMatrix *self, gfloat *elements, gsize *length);

gfloat
cern_matrix_get_offset_x(CernMatrix *self);

gfloat
cern_matrix_get_offset_y(CernMatrix *self);

void
cern_matrix_reset(CernMatrix *self);

void
cern_matrix_multiply(CernMatrix *self, CernMatrix *other);

void
cern_matrix_mutlipty_with_order(CernMatrix *self, CernMatrix *other,
                                CernMatrixOrder order);

void
cern_matrix_translate(CernMatrix *self, gfloat offset_x, gfloat offset_y);

void
cern_matrix_translate_with_order(CernMatrix *self, gfloat offset_x,
                                 gfloat offset_y, CernMatrixOrder order);

void
cern_matrix_scale(CernMatrix *self, gfloat scale_x, gfloat scale_y);

void
cern_matrix_scale_with_order(CernMatrix *self, gfloat scale_x, gfloat scale_y,
                             CernMatrixOrder order);

void
cern_matrix_rotate(CernMatrix *self, gfloat angle);

void
cern_matrix_rotate_with_order(CernMatrix *self, gfloat angle,
                              CernMatrixOrder order);

void
cern_matrix_rotate_at(CernMatrix *self, gfloat angle, CernPointF *point);

void
cern_matrix_rotate_at_with_order(CernMatrix *self, gfloat angle,
                                 CernPointF *point, CernMatrixOrder order);

void
cern_matrix_shear(CernMatrix *self, gfloat shear_x, gfloat shear_y);

void
cern_matrix_shear_with_order(CernMatrix *self, gfloat shear_x, gfloat shear_y,
                             CernMatrixOrder order);

void
cern_matrix_invert(CernMatrix *self);

void
cern_matrix_transform_points(CernMatrix *self, CernPointF *points,
                             gsize points_count);

void
cern_matrix_transform_points_i(CernMatrix *self, CernPoint *points,
                               gsize points_count);

void
cern_matrix_transform_vectors(CernMatrix *self, CernPointF *points,
                              gsize points_count);

void
cern_matrix_transform_vectors_i(CernMatrix *self, CernPoint *points,
                                gsize points_count);

void
cern_matrix_vector_transform_points_i(CernMatrix *self, CernPoint *points,
                                      gsize points_count);

gboolean
cern_matrix_is_invertible(CernMatrix *self);

gboolean
cern_matrix_is_identity(CernMatrix *self);

gboolean
cern_matrix_is_equals(CernMatrix *self, CernMatrix *other);

G_END_DECLS

#endif /* CERN_DRAWING_2D_MATRIX_H */
