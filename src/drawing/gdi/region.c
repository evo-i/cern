#include "cern/drawing/region.h"
#include "cern/drawing/graphics.h"
#include "cern/drawing/graphics_context.h"
#include "cern/drawing/internal/native_gdi_object.h"

#include <Windows.h>
#include <gdiplus.h>

struct _CernRegion {
  GObject parent_instance;

  gpointer handle;
};

static
void
cern_region_set_native(CernRegion *self, GpRegion *region);

static
gpointer
cern_region_get_native(CernRegion *self);

gpointer
cern_region_graphics_get_native(CernGraphics *self) {
  CernNativeGdiObject *native_graphics;
  native_graphics = CERN_NATIVE_GDI_OBJECT(self);
  return cern_native_gdi_object_get_native_handle(native_graphics);
}

static
void
cern_region_native_gdi_object_iface(CernNativeGdiObjectInterface *iface) {
  iface->get_native_handle
    = (gpointer (*)(CernNativeGdiObject *)) cern_region_get_native;
  iface->set_native_handle
    = (void (*)(CernNativeGdiObject *, gpointer)) cern_region_set_native;
}

G_DEFINE_FINAL_TYPE_WITH_CODE(
  CernRegion,
  cern_region,
  G_TYPE_OBJECT,
  G_IMPLEMENT_INTERFACE(CERN_TYPE_NATIVE_GDI_OBJECT, cern_region_native_gdi_object_iface))

static
void
cern_region_class_init(CernRegionClass *klass) { }

static
void
cern_region_init(CernRegion *self) { }

void
cern_region_set_native(CernRegion *self, gpointer region) {
  g_return_if_fail(CERN_IS_REGION(self));
  self->handle = region;
}

gpointer
cern_region_get_native(CernRegion *self) {
  g_return_val_if_fail(CERN_IS_REGION(self), NULL);
  return self->handle;
}

static
CernRegion *
cern_region_new_from_native(gpointer region) {
  CernRegion *self;
  self = g_object_new(CERN_TYPE_REGION, NULL);
  cern_region_set_native(self, region);
  return self;
}

CernRegion *
cern_region_new(void) {
  CernRegion *self;
  GpStatus status;
  GpRegion *region;

  status = GdipCreateRegion(region);

  if (status != Ok) {
    g_critical("cern_region_new(): GdipCreateRegion() failed");
    return NULL;
  }

  return cern_region_new_from_native(region);
}

CernRegion *
cern_region_new_rect_f(CernRectangleF *rect) {
  GpRectF gp_rect;
  GpStatus status;
  GpRegion *region;

  gp_rect.X = cern_rectangle_f_get_x(rect);
  gp_rect.Y = cern_rectangle_f_get_y(rect);
  gp_rect.Width = cern_rectangle_f_get_width(rect);
  gp_rect.Height = cern_rectangle_f_get_height(rect);

  status = GdipCreateRegionRect(&gp_rect, &region);

  if (status!= Ok) {
    g_critical("cern_region_new_rect_f(): GdipCreateRegionRect() failed");
    return NULL;
  }

  return cern_region_new_from_native(region);
}

CernRegion *
cern_region_new_rect(CernRectangle *rect) {
  GpRect gp_rect;
  GpStatus status;
  GpRegion *region;

  gp_rect.X = cern_rectangle_get_x(rect);
  gp_rect.Y = cern_rectangle_get_y(rect);
  gp_rect.Width = cern_rectangle_get_width(rect);
  gp_rect.Height = cern_rectangle_get_height(rect);

  status = GdipCreateRegionRectI(&gp_rect, &region);

  if (status!= Ok) {
    g_critical("cern_region_new_rect(): GdipCreateRegionRectI() failed");
    return NULL;
  }

  return cern_region_new_from_native(region);
}

CernRegion *
cern_region_new_with_graphics_path(CernGraphicsPath *path) {
  GpStatus status;
  GpRegion *region;
  GpPath *gp_path;

  return NULL;
}

void
cern_region_make_infinite(CernRegion *self) {
  GpStatus status;

  status = GdipSetInfinite(cern_region_get_native(self));

  if (status!= Ok) {
    g_critical("cern_region_make_infinite(): GdipMakeRegionInfinite() failed");
  }
}

void
cern_region_make_empty(CernRegion *self) {
  GpStatus status;

  status = GdipSetEmpty(cern_region_get_native(self));

  if (status!= Ok) {
    g_critical("cern_region_make_empty(): GdipMakeRegionEmpty() failed");
  }
}

/* intersect */
void
cern_region_intersect(CernRegion *self, CernRegion *region) {
  GpStatus status;

  gpointer self_handle;
  gpointer region_handle;

  CombineMode mode = (CombineMode) CernCombineMode_Intersect;

  self_handle = cern_region_get_native(self);
  region_handle = cern_region_get_native(region);

  status = GdipCombineRegionRegion(self_handle, region_handle, mode);

  if (status!= Ok) {
    g_critical("cern_region_intersect(): GdipCombineRegionRegion() failed");
  }
}

void
cern_region_intersect_with_graphics_path(CernRegion *region,
                                         CernGraphicsPath *path) {
  GpStatus status;
  CernNativeGdiObject *native_path;
  CombineMode mode = (CombineMode) CernCombineMode_Intersect;

  native_path = CERN_NATIVE_GDI_OBJECT(path);

  status
    = GdipCombineRegionPath(cern_region_get_native(region),
                            cern_native_gdi_object_get_native_handle(native_path),
                            mode);

  if (status != Ok) {
    g_critical("cern_region_intersect_with_graphics_path(): GdipCombineRegionPath() failed");
  }
}

void
cern_region_intersect_with_rectangle(CernRegion *region, CernRectangle *rect) {
  GpRect gp_rect;
  GpStatus status;
  CombineMode mode = (CombineMode) CernCombineMode_Intersect;

  gp_rect.X = cern_rectangle_get_x(rect);
  gp_rect.Y = cern_rectangle_get_y(rect);
  gp_rect.Width = cern_rectangle_get_width(rect);
  gp_rect.Height = cern_rectangle_get_height(rect);

  status
    = GdipCombineRegionRectI(cern_region_get_native(region),
                             &gp_rect, mode);

  if (status != Ok) {
    g_critical("cern_region_intersect_with_rectangle(): GdipCombineRegionRect() failed");
  }
}

void
cern_region_intersect_with_rectangle_f(CernRegion *region, CernRectangleF *rect) {
  GpRectF gp_rect;
  GpStatus status;
  CombineMode mode = (CombineMode) CernCombineMode_Intersect;

  gp_rect.X = cern_rectangle_f_get_x(rect);
  gp_rect.Y = cern_rectangle_f_get_y(rect);
  gp_rect.Width = cern_rectangle_f_get_width(rect);
  gp_rect.Height = cern_rectangle_f_get_height(rect);

  status
    = GdipCombineRegionRect(&gp_rect, cern_region_get_native(region),
                            mode);

  if (status != Ok) {
    g_critical("cern_region_intersect_with_rectangle_f(): GdipCombineRegionRectF() failed");
  }
}

/* complement */
void
cern_region_complement(CernRegion *self, CernRegion *region) {
  GpStatus status;
  CombineMode mode = (CombineMode) CernCombineMode_Complement;

  if (region == NULL) {
    g_critical("cern_region_complement(): passed NULL region");
    return;
  }

  status
    = GdipCombineRegionRegion(cern_region_get_native(self),
                              cern_region_get_native(region),
                              mode);

  if (status != Ok) {
    g_critical("cern_region_complement(): GdipCombineRegionRegion() failed");
  }
}

void
cern_region_complement_with_graphics_path(CernRegion *region,
                                          CernGraphicsPath *path) {
  GpStatus status;
  CernNativeGdiObject *native_path;
  CernNativeGdiObject *native_region;

  gpointer native_path_handle;
  gpointer native_region_handle;

  CombineMode mode = (CombineMode) CernCombineMode_Complement;

  native_path = CERN_NATIVE_GDI_OBJECT(path);
  native_region = CERN_NATIVE_GDI_OBJECT(region);

  native_path_handle = cern_native_gdi_object_get_native_handle(native_path);
  native_region_handle = cern_native_gdi_object_get_native_handle(native_region);

  status
    = GdipCombineRegionPath(native_region_handle, native_path_handle, mode);

  if (status != Ok) {
    g_critical("cern_region_complement_with_graphics_path(): GdipCombineRegionPath() failed");
  }
}

void
cern_region_complement_with_rectangle(CernRegion *region, CernRectangle *rect) {
  GpRect gp_rect;
  GpStatus status;

  CernNativeGdiObject *gdi_object;
  gpointer region_handle;

  CombineMode mode = (CombineMode) CernCombineMode_Complement;

  gdi_object = CERN_NATIVE_GDI_OBJECT(region);
  region_handle = cern_native_gdi_object_get_native_handle(gdi_object);

  gp_rect.X = cern_rectangle_get_x(rect);
  gp_rect.Y = cern_rectangle_get_y(rect);
  gp_rect.Width = cern_rectangle_get_width(rect);
  gp_rect.Height = cern_rectangle_get_height(rect);

  status = GdipCombineRegionRectI(region_handle, &gp_rect, mode);

  if (status != Ok) {
    g_critical("cern_region_complement_with_rectangle(): GdipCombineRegionRect() failed");
  }
}

void
cern_region_complement_with_rectangle_f(CernRegion *region, CernRectangleF *rect) {
  GpRectF gp_rect;
  GpStatus status;
  CombineMode mode = (CombineMode) CernCombineMode_Complement;

  gp_rect.X = cern_rectangle_f_get_x(rect);
  gp_rect.Y = cern_rectangle_f_get_y(rect);
  gp_rect.Width = cern_rectangle_f_get_width(rect);
  gp_rect.Height = cern_rectangle_f_get_height(rect);

  status
    = GdipCombineRegionRect(cern_region_get_native(region), &gp_rect, mode);

  if (status != Ok) {
    g_critical("cern_region_complement_with_rectangle_f(): GdipCombineRegionRectF() failed");
  }
}

void
cern_region_translate(CernRegion *self, gint32 dx, gint32 dy) {
  GpStatus status;

  status = GdipTranslateRegionI(self->handle, dx, dy);

  if (status != Ok) {
    g_warning("%s(...) failed: GdipTranslateRegionI(): %d", __func__, status);
  }
}

void
cern_region_translate_f(CernRegion *self, gfloat dx, gfloat dy) {
  GpStatus status;

  status = GdipTranslateRegion(self->handle, dx, dy);

  if (status != Ok) {
    g_warning("%s(...) failed: GdipTranslateRegionI(): %d", __func__, status);
  }
}

void
cern_region_transform(CernRegion *self, CernMatrix *matrix) {
  GpStatus status;
  gpointer matrix_handle;
  CernNativeGdiObject *matrix_object;

  matrix_object = CERN_NATIVE_GDI_OBJECT(matrix);
  matrix_handle = cern_native_gdi_object_get_native_handle(matrix_object);

  status = GdipTransformRegion(self->handle, matrix_handle);

  if (status != Ok) {
    g_warning("%s(...) failed: GdipTransformRegion(): %d", __func__, status);
  }
}

/* exclude */
void
cern_region_exclude(CernRegion *self, CernRegion *region) {
  GpStatus status;

  gpointer self_handle;
  gpointer region_handle;

  CombineMode mode = (CombineMode) CernCombineMode_Exclude;

  if (region == NULL) {
    g_critical("cern_region_exclude(): passed NULL region");
    return;
  }

  self_handle = cern_region_get_native(self);
  region_handle = cern_region_get_native(region);

  status = GdipCombineRegionRegion(self_handle, region_handle, mode);

  if (status != Ok) {
    g_critical("cern_region_exclude(): GdipCombineRegionRegion() failed");
  }
}

void
cern_region_exclude_with_graphics_path(CernRegion *region,
                                       CernGraphicsPath *path) {
  GpStatus status;
  CernNativeGdiObject *gdi_object;
  gpointer self_handle;
  gpointer path_handle;

  CombineMode mode = (CombineMode) CernCombineMode_Exclude;

  gdi_object = CERN_NATIVE_GDI_OBJECT(path);

  self_handle = cern_region_get_native(region);
  path_handle = cern_native_gdi_object_get_native_handle(gdi_object);

  status = GdipCombineRegionPath(self_handle, path_handle, mode);


  if (status != Ok) {
    g_critical("cern_region_exclude_with_graphics_path(): GdipCombineRegionPath() failed");
  }
}

void
cern_region_exclude_with_rectangle(CernRegion *region, CernRectangle *rect) {
  GpRect gp_rect;
  GpStatus status;
  gpointer self_handle;
  CombineMode mode = (CombineMode) CernCombineMode_Exclude;

  gp_rect.X = cern_rectangle_get_x(rect);
  gp_rect.Y = cern_rectangle_get_y(rect);
  gp_rect.Width = cern_rectangle_get_width(rect);
  gp_rect.Height = cern_rectangle_get_height(rect);

  self_handle = cern_region_get_native(region);

  status = GdipCombineRegionRectI(self_handle, &gp_rect, mode);

  if (status != Ok) {
    g_critical("cern_region_exclude_with_rectangle(): GdipCombineRegionRect() failed");
  }
}

void
cern_region_exclude_with_rectangle_f(CernRegion *region, CernRectangleF *rect) {
  GpRectF gp_rect;
  GpStatus status;

  gpointer self_handle;

  CombineMode mode = (CombineMode) CernCombineMode_Exclude;

  self_handle = cern_region_get_native(region);

  gp_rect.X = cern_rectangle_f_get_x(rect);
  gp_rect.Y = cern_rectangle_f_get_y(rect);
  gp_rect.Width = cern_rectangle_f_get_width(rect);
  gp_rect.Height = cern_rectangle_f_get_height(rect);

  status = GdipCombineRegionRect(self_handle, &gp_rect, mode);

  if (status != Ok) {
    g_critical("cern_region_exclude_with_rectangle_f(): GdipCombineRegionRectF() failed");
  }
}

/* bounds */
CernRectangleF
cern_region_get_bounds_f(CernRegion *self, CernGraphics *graphics) {
  GpRectF gp_rect;
  GpStatus status;

  if (graphics == NULL) {
    g_critical("cern_region_get_bounds_f(): passed NULL graphics");
    return (CernRectangleF) { 0 };
  }

  status
    = GdipGetRegionBounds(cern_region_get_native(self),
                          cern_region_graphics_get_native(graphics),
                          &gp_rect);

  if (status != Ok) {
    g_critical("cern_region_get_bounds_f(): GdipGetRegionBounds() failed");
    return (CernRectangleF) { 0 };
  }

  return
    cern_rectangle_f_create_with_rect(gp_rect.X, gp_rect.Y,
                                      gp_rect.Width, gp_rect.Height);
}

gpointer
cern_region_get_h_region(CernRegion *self, CernGraphics *graphics) {
  GpStatus status;
  HRGN h_region;
  gpointer graphics_handle;

  CernNativeGdiObject *native_graphics;

  native_graphics = CERN_NATIVE_GDI_OBJECT(graphics);
  graphics_handle = cern_native_gdi_object_get_native_handle(native_graphics);

  status = GdipGetRegionHRgn(self, graphics_handle, &h_region);

  if (status != Ok) {
    g_critical("cern_region_get_h_region(): GdipGetRegionHRgn() failed");
    return NULL;
  }

  return (gpointer) h_region;
}

void
cern_region_release_h_region(CernRegion *self, gpointer h_region) {
  if (h_region == NULL) {
    g_critical("cern_region_release_h_region(): passed NULL handle");
    return;
  }

  DeleteObject(h_region);
}

CernRegionData *
cern_region_get_region_data(CernRegion *self) {
  GpStatus status;
  CernRegionData *region_data;
  GByteArray *byte_array;
  guint32 size;

  status
    = GdipGetRegionDataSize(cern_region_get_native(self), &size);

  if (status != Ok) {
    g_critical("cern_region_get_region_data(): GdipGetRegionDataSize() failed");
    return NULL;
  }

  byte_array = g_byte_array_new();
  byte_array = g_byte_array_set_size(byte_array, size);
  status
    = GdipGetRegionData(cern_region_get_native(self),
                        byte_array->data, size, &size);


  if (status != Ok) {
    g_critical("cern_region_get_region_data(): GdipGetRegionData() failed");
    g_byte_array_unref(byte_array);
    return NULL;
  }

  region_data = cern_region_data_new(byte_array);

  g_byte_array_unref(byte_array);

  return region_data;
}

GList *
cern_region_get_region_scans(CernRegion *self, CernMatrix *matrix);

gboolean
cern_region_is_empty(CernRegion *self, CernGraphics *graphics) {
  GpStatus status;
  BOOL result;

  if (graphics == NULL) {
    g_critical("cern_region_is_empty(): passed NULL graphics");
    return FALSE;
  }

  status
    = GdipIsEmptyRegion(cern_region_get_native(self),
                        cern_region_graphics_get_native(graphics), &result);

  if (status != Ok) {
    g_critical("cern_region_is_empty(): GdipIsRegionEmpty() failed");
    return FALSE;
  }

  return result == TRUE;
}

gboolean
cern_region_is_equals(CernRegion *self, CernRegion *region,
                      CernGraphics *graphics) {
  GpStatus status;
  BOOL result;

  if (region == NULL) {
    g_critical("cern_region_is_equals(): passed NULL region");
    return FALSE;
  }

  if (graphics == NULL) {
    g_critical("cern_region_is_equals(): passed NULL graphics");
    return FALSE;
  }

  status
    = GdipIsEqualRegion(cern_region_get_native(self),
                        cern_region_get_native(region),
                        cern_region_graphics_get_native(graphics), &result);

  if (status != Ok) {
    g_critical("cern_region_is_equals(): GdipIsRegionEqual() failed");
    return FALSE;
  }

  return result == TRUE;
}

gboolean
cern_region_is_infinite(CernRegion *self, CernGraphics *graphics) {
  GpStatus status;
  BOOL result;

  if (graphics == NULL) {
    g_critical("cern_region_is_infinite(): passed NULL graphics");
    return FALSE;
  }

  status
    = GdipIsInfiniteRegion(cern_region_get_native(self),
                           cern_region_graphics_get_native(graphics), &result);

  if (status != Ok) {
    g_critical("cern_region_is_infinite(): GdipIsRegionVisible() failed");
    return FALSE;
  }

  return result == TRUE;
}

gboolean
cern_region_is_visible_point_f(CernRegion *self, CernPointF *point) {
  GpStatus status;
  BOOL result;

  if (point == NULL) {
    g_critical("cern_region_is_visible_point_f(): passed NULL point");
    return FALSE;
  }

  status
    = GdipIsVisibleRegionPoint(cern_region_get_native(self),
                               cern_point_f_get_x(point),
                               cern_point_f_get_y(point),
                               NULL, &result);

  if (status != Ok) {
    g_critical("cern_region_is_visible_point_f(): GdipIsVisibleRegionPoint() failed");
    return FALSE;
  }

  return result == TRUE;
}

gboolean
cern_region_is_visible_point(CernRegion *self, CernPoint *point) {
  GpStatus status;
  BOOL result;

  if (point == NULL) {
    g_critical("cern_region_is_visible_point(): passed NULL point");
    return FALSE;
  }

  status
    = GdipIsVisibleRegionPointI(cern_region_get_native(self),
                                cern_point_get_x(point),
                                cern_point_get_y(point),
                                NULL, &result);

  if (status != Ok) {
    g_critical("cern_region_is_visible_point(): GdipIsVisibleRegionPointI() failed");
    return FALSE;
  }

  return result == TRUE;
}

gboolean
cern_region_is_visible_rect_f(CernRegion *self, CernRectangleF *rect) {
  return cern_region_is_visible_rect_f_graphics(self, rect, NULL);
}

gboolean
cern_region_is_visible_rect_f_graphics(CernRegion *self, CernRectangleF *rect,
                                       CernGraphics *graphics) {
  GpStatus status;
  BOOL is_visible;
  GpGraphics *gfx;
  CernNativeGdiObject *gfx_native;

  gfx_native = CERN_NATIVE_GDI_OBJECT(graphics);
  gfx = cern_native_gdi_object_get_native_handle(gfx_native);

  status
    = GdipIsVisibleRegionRect(self->handle,
                               cern_rectangle_f_get_x(rect),
                               cern_rectangle_f_get_y(rect),
                               cern_rectangle_f_get_width(rect),
                               cern_rectangle_f_get_height(rect),
                               gfx, &is_visible);

  if (status != Ok) {
    g_warning("GdipIsVisibleRegionRect() failed: %d", status);
    return FALSE;
  }

  return is_visible != 0;
}
