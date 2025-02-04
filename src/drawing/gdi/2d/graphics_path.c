#include "cern/drawing/2d/graphics_path.h"
#include "cern/core/icloneable.h"
#include "cern/drawing/internal/native_gdi_object.h"

#include <Windows.h>
#include <gdiplus.h>

struct _CernGraphicsPath {
  GObject parent_instance;

  gpointer handle;
};

static
void
cern_graphics_path_cloneable_iface(CernICLoneableInterface *iface) {
  iface->clone
    = (CernICLoneable *(*)(CernICLoneable *)) cern_graphics_path_clone;
}

gpointer
cern_graphics_path_get_native_handle(CernGraphicsPath *self) {
  return self->handle;
}

void
cern_graphics_path_set_native_handle(CernGraphicsPath *self, gpointer handle) {
  self->handle = handle;
}

static
void
cern_graphics_path_native_gdi_object_iface(CernNativeGdiObjectInterface *iface) {
  iface->get_native_handle
    = (gpointer(*)(CernNativeGdiObject *)) cern_graphics_path_get_native_handle;
  iface->set_native_handle
    = (void(*)(CernNativeGdiObject *, gpointer)) cern_graphics_path_set_native_handle;
}

G_DEFINE_FINAL_TYPE_WITH_CODE(
  CernGraphicsPath,
  cern_graphics_path,
  G_TYPE_OBJECT,
  G_IMPLEMENT_INTERFACE(CERN_TYPE_ICLONEABLE, cern_graphics_path_cloneable_iface)
  G_IMPLEMENT_INTERFACE(CERN_TYPE_NATIVE_GDI_OBJECT, cern_graphics_path_native_gdi_object_iface))

static
void
cern_graphics_path_init(CernGraphicsPath *self) { }

static
void
cern_graphics_path_class_init(CernGraphicsPathClass *klass) { }

CernGraphicsPath *
cern_graphics_path_new_from_native_handle(gpointer handle) {
  CernGraphicsPath *self;
  self = g_object_new(CERN_TYPE_GRAPHICS_PATH, NULL);
  self->handle = handle;
  return self;
}

CernGraphicsPath *
cern_graphics_path_new(void) {
  return
    cern_graphics_path_new_fill_mode(FillModeAlternate);
}

CernGraphicsPath *
cern_graphics_path_new_fill_mode(CernFillMode fill_mode) {
  CernGraphicsPath *self;
  GpPath *path;
  GpStatus status;

  status = GdipCreatePath(fill_mode, &path);

  if (status != Ok) {
    g_critical("cern_graphics_path_new_fill_mode(...): GdipCreatePath() failed: %d", status);
    return NULL;
  }

  return cern_graphics_path_new_from_native_handle(path);
}

CernGraphicsPath *
cern_fraphics_path_new_points(CernPoint *points, gsize count,
                              GByteArray *types) {
  return
    cern_graphics_path_new_points_fill_mode(points, count,
                                            types, CernFillMode_Alternate);
}

CernGraphicsPath *
cern_fraphics_path_new_points_f(CernPointF *points, gsize count,
                                GByteArray *types) {
  return
    cern_graphics_path_new_points_fill_mode_f(points, count,
                                              types, CernFillMode_Alternate);
}

CernGraphicsPath *
cern_graphics_path_new_points_fill_mode_f(CernPointF *points,
                                          gsize points_count,
                                          GByteArray *types,
                                          CernFillMode fill_mode) {
  g_return_val_if_fail(points != NULL, NULL);
  g_return_val_if_fail(points_count != 0, NULL);
  g_return_val_if_fail(types != NULL, NULL);

  CernGraphicsPath *self;
  GpPath *path;
  GpStatus status;

  GpPointF *local_points;

  local_points = g_new(GpPointF, points_count);

  for (size_t i = 0; i < points_count; i++) {
    local_points[i].X = points[i].x;
    local_points[i].Y = points[i].y;
  }
}

CernGraphicsPath *
cern_graphics_path_new_points_fill_mode(CernPoint *points,
                                        gsize points_count,
                                        GByteArray *types,
                                        CernFillMode fill_mode) {
  g_return_val_if_fail(points != NULL, NULL);
  g_return_val_if_fail(points_count != 0, NULL);
  g_return_val_if_fail(types != NULL, NULL);
}

CernGraphicsPath *
cern_graphics_path_clone(CernGraphicsPath *self) {
  GpStatus status;
  GpPath *native_path;

  status = GdipClonePath(self->handle, &native_path);

  if (status != Ok) {
    g_critical("cern_graphics_path_clone(...): GdipClonePath() failed: %d", status);
    return NULL;
  }

  return cern_graphics_path_new_from_native_handle(native_path);
}
