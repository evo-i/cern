#include "cern/drawing/2d/matrix.h"
#include "cern/core/icloneable.h"
#include "cern/drawing/internal/native_gdi_object.h"

#include <Windows.h>
#include <gdiplus.h>

struct _CernMatrix {
  GObject parent_instance;

  gpointer handle;
};

static
gpointer
cern_matrix_get_native(CernMatrix* self) {
  CernNativeGdiObject *obj;
  obj = CERN_NATIVE_GDI_OBJECT(self);
  return cern_native_gdi_object_get_native_handle(obj);
}

void
cern_matrix_set_mative(CernMatrix* self, gpointer handle) {
  CernNativeGdiObject *obj;
  obj = CERN_NATIVE_GDI_OBJECT(self);
  cern_native_gdi_object_set_native_handle(obj, handle);
}

static
void
cern_matrix_cloneable_iface_init(CernICLoneableInterface *iface) {
  iface->clone
    = (CernICLoneable *(*)(CernICLoneable *)) cern_matrix_clone;
}

static
void
cern_matrix_native_gdi_object_iface_init(CernNativeGdiObjectInterface *iface) {
  iface->get_native_handle
    = (gpointer (*)(CernNativeGdiObject *)) cern_matrix_get_native;

  iface->set_native_handle
    = (void (*)(CernNativeGdiObject *, gpointer)) cern_matrix_set_mative;
}

G_DEFINE_FINAL_TYPE_WITH_CODE(CernMatrix, cern_matrix, G_TYPE_OBJECT,
  G_IMPLEMENT_INTERFACE(CERN_TYPE_ICLONEABLE, cern_matrix_cloneable_iface_init)
  G_IMPLEMENT_INTERFACE(CERN_TYPE_NATIVE_GDI_OBJECT, cern_matrix_native_gdi_object_iface_init))

static
void cern_matrix_finalize(GObject *obj) {
  CernMatrix *self = CERN_MATRIX(obj);
  GdipDeleteMatrix(self->handle);
  self->handle = NULL;

  G_OBJECT_CLASS(cern_matrix_parent_class)->finalize(obj);
}

static
void
cern_matrix_init(CernMatrix *self) { }

static
void
cern_matrix_class_init(CernMatrixClass *klass) {
  G_OBJECT_CLASS(klass)->finalize = cern_matrix_finalize;
}

static
CernMatrix *
cern_matrix_from_native(gpointer handle) {
  CernMatrix *self;
  self = g_object_new(CERN_TYPE_MATRIX, NULL);
  cern_matrix_set_mative(self, handle);
  return self;
}

CernMatrix *
cern_matrix_new(void) {
  GpStatus status;
  gpointer matrix;

  status = GdipCreateMatrix(&matrix);

  if (status!= Ok) {
    g_warning("cern_matrix_new(): GdipCreateMatrix() failed: %d", status);
    return NULL;
  }

  return cern_matrix_from_native(matrix);
}

CernMatrix *
cern_matrix_new_from_values(gfloat m11, gfloat m12, gfloat m21, gfloat m22,
                            gfloat dx, gfloat dy) {
  GpStatus status;
  gpointer matrix;

  status = GdipCreateMatrix2(m11, m12, m21, m22, dx, dy, &matrix);

  if (status!= Ok) {
    g_warning("cern_matrix_new_from_values(): GdipCreateMatrix2() failed: %d", status);
    return NULL;
  }

  return cern_matrix_from_native(matrix);
}


CernMatrix *
cern_matrix_new_from_rectangle_and_points(CernRectangleF *rect,
                                          CernPointF *points, gsize points_count) {
  GpRectF gp_rect;
  GpPointF gp_point;
  GpStatus status;
  gpointer matrix;

  gp_rect.X = cern_rectangle_f_get_x(rect);
  gp_rect.Y = cern_rectangle_f_get_y(rect);
  gp_rect.Width = cern_rectangle_f_get_width(rect);
  gp_rect.Height = cern_rectangle_f_get_height(rect);

  gp_point.X = cern_point_f_get_x(points);
  gp_point.Y = cern_point_f_get_y(points);

  status = GdipCreateMatrix3(&gp_rect, &gp_point, &matrix);

  if (status!= Ok) {
    g_warning("cern_matrix_new_from_rectangle_and_points(): GdipCreateMatrix3() failed: %d", status);
    return NULL;
  }

  return cern_matrix_from_native(matrix);
}

CernMatrix *
cern_matrix_new_from_rectangle_and_points_i(CernRectangle *rect,
                                            CernPoint *points, gsize points_count) {
  GpStatus status;
  GpRect gp_rect;
  GpPoint *gp_points;
  gpointer matrix;

  gp_rect.X = cern_rectangle_get_x(rect);
  gp_rect.Y = cern_rectangle_get_y(rect);
  gp_rect.Width = cern_rectangle_get_width(rect);
  gp_rect.Height = cern_rectangle_get_height(rect);

  gp_points = g_new(GpPoint, points_count);

  for (gsize i = 0; i < points_count; i++) {
    gp_points[i].X = cern_point_get_x(points + i);
    gp_points[i].Y = cern_point_get_y(points + i);
  }

  status = GdipCreateMatrix3I(&gp_rect, gp_points, &matrix);

  g_free(gp_points);

  if (status!= Ok) {
    g_warning("cern_matrix_new_from_rectangle_and_points_i(): GdipCreateMatrix3I() failed: %d", status);
    return NULL;
  }

  return cern_matrix_from_native(matrix);
}

CernMatrix *
cern_matrix_clone(CernMatrix *self) {
  GpMatrix *matrix;
  GpStatus status;

  status = GdipCloneMatrix(cern_matrix_get_native(self), &matrix);

  if (status!= Ok) {
    g_warning("cern_matrix_clone(): GdipCloneMatrix() failed: %d", status);
    return NULL;
  }

  return cern_matrix_from_native(matrix);
}

void
cern_matrix_get_elements(CernMatrix *self, gfloat *elements, gsize *length) {
  GpMatrix *matrix;
  GpStatus status;

  if (elements == NULL) {
    g_warning("cern_matrix_get_elements(): elements array is NULL");
    return;
  }

  if (length == NULL) {
    g_warning("cern_matrix_get_elements(): length is NULL");
    return;
  }

  if (*length < 6) {
    g_warning("cern_matrix_get_elements(): elements array must have at least 6 elements");
    return;
  }

  status = GdipGetMatrixElements(cern_matrix_get_native(self), elements);

  if (status!= Ok) {
    g_warning("cern_matrix_get_elements(): GdipGetMatrixElements() failed: %d", status);
    return;
  }

  *length = 6;
}

gfloat
cern_matrix_get_offset_x(CernMatrix *self) {
  GpStatus status;
  gfloat elements[6] = { 0 };

  status = GdipGetMatrixElements(cern_matrix_get_native(self), elements);

  if (status!= Ok) {
    g_warning("cern_matrix_get_offset_x(): GdipGetMatrixElements() failed: %d", status);
    return 0;
  }

  return elements[4];
}

gfloat
cern_matrix_get_offset_y(CernMatrix *self) {
  GpStatus status;
  gfloat elements[6] = { 0 };

  status = GdipGetMatrixElements(cern_matrix_get_native(self), elements);

  if (status!= Ok) {
    g_warning("cern_matrix_get_offset_y(): GdipGetMatrixElements() failed: %d", status);
    return 0;
  }

  return elements[5];
}

void
cern_matrix_reset(CernMatrix *self) {
  GpStatus status;

  status
    = GdipSetMatrixElements(cern_matrix_get_native(self), 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);

  if (status!= Ok) {
    g_warning("cern_matrix_reset(): GdipResetMatrix() failed: %d", status);
    return;
  }
}

void
cern_matrix_multiply(CernMatrix *self, CernMatrix *other) {
  cern_matrix_mutlipty_with_order(self, other, CernMatrixOrder_Prepend);
}

void
cern_matrix_mutlipty_with_order(CernMatrix *self, CernMatrix *other,
                                CernMatrixOrder order) {
  GpStatus status;
  if (other == NULL) {
    g_warning("cern_matrix_mutlipty_with_order(): other matrix is NULL");
    return;
  }

  status
    = GdipMultiplyMatrix(cern_matrix_get_native(self),
                         cern_matrix_get_native(other), (GpMatrixOrder) order);

  if (status!= Ok) {
    g_warning("cern_matrix_mutlipty_with_order(): GdipMultiplyMatrix() failed: %d", status);
    return;
  }
}

void
cern_matrix_translate(CernMatrix *self, gfloat offset_x, gfloat offset_y) {
  cern_matrix_translate_with_order(self, offset_x, offset_y,
                                   CernMatrixOrder_Prepend);
}

void
cern_matrix_translate_with_order(CernMatrix *self, gfloat offset_x,
                                 gfloat offset_y, CernMatrixOrder order) {
  GpStatus status;

  status = GdipTranslateMatrix(self, offset_x, offset_y, (GpMatrixOrder) order);

  if (status!= Ok) {
    g_warning("cern_matrix_translate_with_order(): GdipTranslateMatrix() failed: %d", status);
    return;
  }
}

void
cern_matrix_scale(CernMatrix *self, gfloat scale_x, gfloat scale_y) {
  cern_matrix_scale_with_order(self, scale_x, scale_y, CernMatrixOrder_Prepend);
}

void
cern_matrix_scale_with_order(CernMatrix *self, gfloat scale_x, gfloat scale_y,
                             CernMatrixOrder order) {
  GpStatus status;

  status
    = GdipScaleMatrix(cern_matrix_get_native(self), scale_x, scale_y,
                      (GpMatrixOrder) order);

  if (status!= Ok) {
    g_warning("cern_matrix_scale_with_order(): GdipScaleMatrix() failed: %d", status);
    return;
  }
}

void
cern_matrix_rotate(CernMatrix *self, gfloat angle) {
  cern_matrix_rotate_with_order(self, angle, CernMatrixOrder_Prepend);
}

void
cern_matrix_rotate_with_order(CernMatrix *self, gfloat angle,
                              CernMatrixOrder order) {
  GpStatus status;

  status
    = GdipRotateMatrix(cern_matrix_get_native(self), angle,
                       (GpMatrixOrder) order);

  if (status!= Ok) {
    g_warning("cern_matrix_rotate_with_order(): GdipRotateMatrix() failed: %d", status);
    return;
  }
}

void
cern_matrix_rotate_at(CernMatrix *self, gfloat angle, CernPointF *point) {
  cern_matrix_rotate_at_with_order(self, angle, point, CernMatrixOrder_Prepend);
}

void
cern_matrix_rotate_at_with_order(CernMatrix *self, gfloat angle,
                                 CernPointF *point, CernMatrixOrder order) {
  GpStatus status;
  GpPointF gp_point = {
    .X = point->x,
    .Y = point->y
  };

  if (order == CernMatrixOrder_Prepend) {
    status = GdipTranslateMatrix(cern_matrix_get_native(self), gp_point.X, gp_point.Y, (GpMatrixOrder) order);
    status |= GdipRotateMatrix(cern_matrix_get_native(self), angle, (GpMatrixOrder) order);
    status |= GdipTranslateMatrix(cern_matrix_get_native(self), -(gp_point.X), -(gp_point.Y), (GpMatrixOrder) order);
  } else {
    status = GdipTranslateMatrix(cern_matrix_get_native(self), -(gp_point.X), -(gp_point.Y), (GpMatrixOrder) order);
    status |= GdipRotateMatrix(cern_matrix_get_native(self), angle, (GpMatrixOrder) order);
    status |= GdipTranslateMatrix(cern_matrix_get_native(self), gp_point.X, gp_point.Y, (GpMatrixOrder) order);
  }

  if (status!= Ok) {
    g_warning("cern_matrix_rotate_at_with_order(): GdipTransformMatrixPoints() failed: %d", status);
    return;
  }
}

void
cern_matrix_shear(CernMatrix *self, gfloat shear_x, gfloat shear_y) {
  cern_matrix_shear_with_order(self, shear_x, shear_y, CernMatrixOrder_Prepend);
}

void
cern_matrix_shear_with_order(CernMatrix *self, gfloat shear_x, gfloat shear_y,
                             CernMatrixOrder order) {
  GpStatus status;

  status
    = GdipShearMatrix(cern_matrix_get_native(self), shear_x, shear_y,
                      (GpMatrixOrder) order);

  if (status!= Ok) {
    g_warning("cern_matrix_shear_with_order(): GdipShearMatrix() failed: %d", status);
    return;
  }
}

void
cern_matrix_invert(CernMatrix *self) {
  GpStatus status;

  status = GdipInvertMatrix(cern_matrix_get_native(self));

  if (status!= Ok) {
    g_warning("cern_matrix_invert(): GdipInvertMatrix() failed: %d", status);
    return;
  }
}

void
cern_matrix_transform_points(CernMatrix *self, CernPointF *points,
                             gsize points_count) {
  GpStatus status;
  GpPointF *gp_points;

  gp_points = g_new(GpPointF, points_count);

  for (gsize i = 0; i < points_count; i++) {
    gp_points[i].X = points[i].x;
    gp_points[i].Y = points[i].y;
  }

  status
    = GdipTransformMatrixPoints(cern_matrix_get_native(self), gp_points, points_count);

  if (status!= Ok) {
    g_warning("cern_matrix_transform_points(): GdipTransformMatrixPoints() failed: %d", status);
  } else {
    for (gsize i = 0; i < points_count; i++) {
      points[i].x = gp_points[i].X;
      points[i].y = gp_points[i].Y;
    }
  }

  g_free(gp_points);
}

void
cern_matrix_transform_points_i(CernMatrix *self, CernPoint *points,
                               gsize points_count) {
  GpStatus status;
  GpPoint *gp_points;

  gp_points = g_new(GpPoint, points_count);

  for (gsize i = 0; i < points_count; i++) {
    gp_points[i].X = points[i].x;
    gp_points[i].Y = points[i].y;
  }

  status
    = GdipTransformMatrixPointsI(cern_matrix_get_native(self), gp_points, points_count);

  if (status!= Ok) {
    g_warning("cern_matrix_transform_points_i(): GdipTransformMatrixPointsI() failed: %d", status);
  } else {
    for (gsize i = 0; i < points_count; i++) {
      points[i].x = gp_points[i].X;
      points[i].y = gp_points[i].Y;
    }
  }

  g_free(gp_points);
}

void
cern_matrix_transform_vectors(CernMatrix *self, CernPointF *points,
                              gsize points_count) {
  GpStatus status;
  GpPointF *gp_points;

  gp_points = g_new(GpPointF, points_count);

  for (gsize i = 0; i < points_count; i++) {
    gp_points[i].X = points[i].x;
    gp_points[i].Y = points[i].y;
  }

  status
    = GdipVectorTransformMatrixPoints(cern_matrix_get_native(self), gp_points, points_count);

  if (status!= Ok) {
    g_warning("cern_matrix_transform_vectors(): GdipVectorTransformMatrixPoints() failed: %d", status);
  } else {
    for (gsize i = 0; i < points_count; i++) {
      points[i].x = gp_points[i].X;
      points[i].y = gp_points[i].Y;
    }
  }

  g_free(gp_points);
}

void
cern_matrix_transform_vectors_i(CernMatrix *self, CernPoint *points,
                                gsize points_count) {
  GpStatus status;

  GpPoint *gp_points;

  gp_points = g_new(GpPoint, points_count);
  for (gsize i = 0; i < points_count; i++) {
    gp_points[i].X = points[i].x;
    gp_points[i].Y = points[i].y;
  }

  status
    = GdipVectorTransformMatrixPointsI(cern_matrix_get_native(self), gp_points, points_count);

  if (status!= Ok) {
    g_warning("cern_matrix_transform_vectors_i(): GdipVectorTransformMatrixPointsI() failed: %d", status);
  } else {
    for (gsize i = 0; i < points_count; i++) {
      points[i].x = gp_points[i].X;
      points[i].y = gp_points[i].Y;
    }
  }

  g_free(gp_points);
}

void
cern_matrix_vector_transform_points_i(CernMatrix *self, CernPoint *points,
                                      gsize points_count) {
  cern_matrix_transform_vectors_i(self, points, points_count);
}

gboolean
cern_matrix_is_invertible(CernMatrix *self) {
  GpStatus status;
  BOOL result;

  status = GdipIsMatrixInvertible(cern_matrix_get_native(self), &result);

  if (status!= Ok) {
    g_warning("cern_matrix_is_invertible(): GdipIsMatrixInvertible() failed: %d", status);
    return FALSE;
  }

  return result != FALSE;
}

gboolean
cern_matrix_is_identity(CernMatrix *self) {
  GpStatus status;
  BOOL result;

  status = GdipIsMatrixIdentity(cern_matrix_get_native(self), &result);

  if (status!= Ok) {
    g_warning("cern_matrix_is_identity(): GdipIsMatrixIdentity() failed: %d", status);
    return FALSE;
  }

  return result != FALSE;
}

gboolean
cern_matrix_is_equals(CernMatrix *self, CernMatrix *other) {
  GpStatus status;
  BOOL result;

  status = GdipIsMatrixEqual(cern_matrix_get_native(self), cern_matrix_get_native(other), &result);

  if (status!= Ok) {
    g_warning("cern_matrix_is_equals(): GdipIsMatrixEqual() failed: %d", status);
    return FALSE;
  }

  return result != FALSE;
}
