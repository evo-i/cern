#include "cern/drawing/2d/hatch_brush.h"
#include "cern/core/icloneable.h"
#include "cern/drawing/internal/native_gdi_object.h"

#include <Windows.h>
#include <gdiplus.h>

struct _CernHatchBrush {
  CernBrush parent_instance;
};

static
gpointer
cern_hatch_brush_get_native(CernHatchBrush *self) {
  CernNativeGdiObject *obj;
  obj = CERN_NATIVE_GDI_OBJECT(self);
  return cern_native_gdi_object_get_native_handle(obj);
}

static
void
cern_hatch_brush_set_native(CernHatchBrush *self, gpointer handle) {
  CernNativeGdiObject *obj;
  obj = CERN_NATIVE_GDI_OBJECT(self);
  cern_native_gdi_object_set_native_handle(obj, handle);
}

static
CernICloneable *
cern_hatch_clone(CernICloneable *self);

static
void
cern_hatch_brush_clone_interface_init(CernICloneableInterface *iface) {
  iface->clone = (CernICloneable *(*)(CernICloneable *)) cern_hatch_brush_clone;
}

G_DEFINE_TYPE_WITH_CODE(CernHatchBrush, cern_hatch_brush, CERN_TYPE_BRUSH,
  G_IMPLEMENT_INTERFACE(CERN_TYPE_ICLONEABLE,
    cern_hatch_brush_clone_interface_init))

static
void
cern_hatch_brush_init(CernHatchBrush *self) { }

static
void
cern_hatch_brush_class_init(CernHatchBrushClass *klass) { }

CernHatchBrush *
cern_hatch_brush_new_with_native(gpointer handle) {
  CernHatchBrush *brush;
  brush = g_object_new(CERN_TYPE_HATCH_BRUSH, NULL);
  cern_hatch_brush_set_native(brush, handle);
  return brush;
}

CernBrush *
cern_hatch_brush_new(CernHatchStyle style, CernColor *foreground_color) {
  return cern_hatch_brush_new_with_color(style, foreground_color, NULL);
}

CernBrush *
cern_hatch_brush_new_with_color(CernHatchStyle style,
                                CernColor *foreground_color,
                                CernColor *background_color) {
 GpStatus status;
 GpHatchStyle gp_style;
 gpointer brush;

  gp_style = (GpHatchStyle) style;

  status
    = GdipCreateHatchBrush(gp_style,
                           cern_color_to_argb(foreground_color),
                           background_color
                            ? cern_color_to_argb(background_color)
                            : 0xff000000,
                           &brush);

  if (status != Ok) {
    g_critical("Failed to create hatch brush: GdipCreateHatchBrush failed");
    return NULL;
  }

  return CERN_BRUSH(cern_hatch_brush_new_with_native(brush));
}

CernBrush *
cern_hatch_brush_clone(CernHatchBrush *self) {
  CernICloneable *cloneable;
  CernICloneableInterface *iface;
  cloneable = CERN_ICLONEABLE(self);
  iface = CERN_ICLONEABLE_GET_IFACE(cloneable);
  return CERN_BRUSH(iface->clone(cloneable));
}

CernHatchStyle
cern_hatch_brush_get_style(CernHatchBrush *self) {
  GpStatus status;
  GpHatchStyle style;

  status
    = GdipGetHatchStyle(cern_hatch_brush_get_native(self), &style);

  if (status != Ok) {
    return CernHatchStyle_Min;
  }

  return (CernHatchStyle) style;
}

CernColor
cern_hatch_brush_get_foreground_color(CernHatchBrush *self) {
  GpStatus status;
  ARGB color;

  status
    = GdipGetHatchForegroundColor(cern_hatch_brush_get_native(self), &color);

  if (status != Ok) {
    return cern_color_empty();
  }

  return cern_color_from_argb(color);
}

CernColor
cern_hatch_brush_get_background_color(CernHatchBrush *self) {
  GpStatus status;
  ARGB color;

  status
    = GdipGetHatchBackgroundColor(cern_hatch_brush_get_native(self), &color);

  if (status != Ok) {
    return cern_color_empty();
  }

  return cern_color_from_argb(color);
}

static
CernICloneable *
cern_hatch_clone(CernICloneable *self) {
  GpStatus status;
  gpointer native;
  CernHatchBrush *brush;

  brush = CERN_HATCH_BRUSH(self);
  status = GdipCloneBrush(cern_hatch_brush_get_native(brush), &native);

  if (status != Ok) {
    g_critical("Failed to clone hatch brush.");
    return NULL;
  }

  return CERN_ICLONEABLE(cern_hatch_brush_new_with_native(native));
}
