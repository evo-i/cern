#include "cern/drawing/region.h"
#include "cern/drawing/graphics.h"

#include "cern/core/icloneable.h"

#include "cern/drawing/internal/windows_region.h"
#include "cern/drawing/internal/native_gdi_object.h"

#include <windows.h>

struct _CernWindowsRegion {
  GObject parent_instance;

  gpointer handle;
  gboolean is_handle_owner;
};

static
gpointer
cern_windows_region_get_handle(CernWindowsRegion *self) {
  return self->handle;
}

static
void
cern_windows_region_set_handle(CernWindowsRegion *self, gpointer handle) {
  self->handle = handle;
}

CernWindowsRegion *
cern_windows_region_new_from_handle(gpointer handle) {
  CernWindowsRegion *self;

  self = g_object_new(CERN_TYPE_WINDOWS_REGION, NULL);
  cern_windows_region_set_handle(self, handle);
  return self;
}

static
void
cern_windows_region_native_gdi_object_interface_init(CernNativeGdiObjectInterface *iface) {
  iface->get_native_handle
    = (gpointer(*)(CernNativeGdiObject*))       cern_windows_region_get_handle;
  iface->set_native_handle
    = (void(*)(CernNativeGdiObject*,gpointer))  cern_windows_region_set_handle;
}

CernWindowsRegion *
cern_windows_region_clone(CernWindowsRegion *self) {
  CernRectangle rect;

  if (cern_windows_region_is_infinite(self)) {
    return cern_windows_region_new_from_handle(NULL);
  }

  rect = cern_windows_region_to_rectangle(self);

  return cern_windows_region_new(&rect);
}

static
void
cern_windows_region_cloneable_interface_init(CernICloneableInterface *iface) {
  iface->clone = (CernICloneable* (*)(CernICloneable*)) cern_windows_region_clone;
}

G_DEFINE_FINAL_TYPE_WITH_CODE(CernWindowsRegion,
  cern_windows_region,
  G_TYPE_OBJECT,
  G_IMPLEMENT_INTERFACE(CERN_TYPE_NATIVE_GDI_OBJECT, cern_windows_region_native_gdi_object_interface_init)
  G_IMPLEMENT_INTERFACE(CERN_TYPE_ICLONEABLE, cern_windows_region_cloneable_interface_init));

static
void
cern_windows_region_finalize(GObject *object) {
  CernWindowsRegion *self = CERN_WINDOWS_REGION(object);

  G_OBJECT_CLASS(cern_windows_region_parent_class)->finalize(object);
}

static
void
cern_windows_region_class_init(CernWindowsRegionClass *klass) {
  GObjectClass *object_class = G_OBJECT_CLASS(klass);

  object_class->finalize = cern_windows_region_finalize;
}

static
void
cern_windows_region_init(CernWindowsRegion *self) {
  self->is_handle_owner = FALSE;
}

CernWindowsRegion *
cern_windows_region_new(CernRectangle *rect) {
  return
    cern_windows_region_new_with_rectangle(rect->x, rect->y,
                                          rect->width, rect->height);
}

CernWindowsRegion *
cern_windows_region_new_with_rectangle(gint32 x, gint32 y,
                                       gint32 width, gint32 height) {
  CernWindowsRegion *self;
  gpointer handle;

  handle = CreateRectRgn(x, y, width, height);

  self = cern_windows_region_new_from_handle(handle);
  self->is_handle_owner = TRUE;

  return self;
}

CernWindowsRegion *
cern_windows_region_new_from_h_region(gpointer h_region,
                                      gboolean take_ownership) {
  CernWindowsRegion *self;

  self = cern_windows_region_new_from_handle(NULL);

  if (h_region != NULL) {
    self->handle = h_region;
    self->is_handle_owner = take_ownership;
  }

  return self;
}

CernWindowsRegion *
cern_windows_region_new_from_region(CernRegion *region, CernGraphics *graphics) {
  gpointer region_handle;

  region_handle = NULL;

  if (cern_region_is_infinite(region, graphics)) {
    return cern_windows_region_new_from_h_region(region_handle, FALSE);
  }

  region_handle = cern_region_get_h_region(region, graphics);

  return
    cern_windows_region_new_from_h_region(region_handle, TRUE);
}

gboolean
cern_windows_region_is_infinite(CernWindowsRegion *self) {
  return cern_windows_region_get_handle(self) == NULL;
}

CernRectangle
cern_windows_region_to_rectangle(CernWindowsRegion *self) {
  RECT rect = { 0 };
  GetRgnBox(cern_windows_region_get_handle(self), &rect);
  return cern_rectangle_create(rect.left, rect.top, rect.right, rect.bottom);
}

CernRegionFlags
cern_windows_region_combine(CernWindowsRegion *self,
                            CernWindowsRegion *region1,
                            CernWindowsRegion *region2,
                            CernRegionCombineMode mode) {
  return
    (CernRegionFlags)
      CombineRgn(self->handle, region1->handle, region2->handle, (int) mode);
}
