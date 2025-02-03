#ifndef CERN_DRAWING_MATRIX_H
#define CERN_DRAWING_MATRIX_H 1

#include <glib-object.h>

#include "point_f.h"
#include "rectangle_f.h"

G_BEGIN_DECLS

#define CERN_TYPE_MATRIX (cern_matrix_get_type())

G_DECLARE_FINAL_TYPE(CernMatrix, cern_matrix, CERN, MATRIX, GObject)

CernMatrix *
cern_matrix_new(void);

CernMatrix *
cern_matrix_new_from_values(gfloat m11, gfloat m12, gfloat m21,
                            gfloat m22, gfloat dx, gfloat dy);

CernMatrix *
cern_matrix_new_from_rectangle_and_point(CernRectangleF *rect, GList *points);

G_END_DECLS

#endif /* CERN_DRAWING_MATRIX_H */
