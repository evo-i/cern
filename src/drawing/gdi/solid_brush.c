#include "cern/drawing/color.h"
#include "cern/drawing/solid_brush.h"
#include "cern/drawing/internal/native_gdi_object.h"

#include <Windows.h>
#include <gdiplus.h>

struct _CernSolidBrush {
  CernBrush parent_instance;
  gboolean is_immutable;
};

G_DEFINE_FINAL_TYPE(CernSolidBrush, cern_solid_brush, CERN_TYPE_BRUSH)

static
gpointer
cern_solid_brush_get_native(CernSolidBrush *self);

void
cern_solid_brush_set_native(CernSolidBrush *self, gpointer brush);

static
CernBrush *
cern_solid_brush_clone(CernBrush *brush);

static
void
cern_solid_brush_class_init(CernSolidBrushClass *klass) {
  CernBrushClass *brush_class = CERN_BRUSH_CLASS(klass);
  brush_class->clone = cern_solid_brush_clone;
}

static
void
cern_solid_brush_init(CernSolidBrush *self) { }

static
CernSolidBrush *
cern_solid_brush_new_internal(void) {
  return g_object_new(CERN_TYPE_SOLID_BRUSH, NULL);
}

static
CernSolidBrush *
cern_solid_brush_new_with_native(gpointer native_brush) {
  CernSolidBrush *self;
  self = cern_solid_brush_new_internal();
  cern_solid_brush_set_native(self, native_brush);
  return self;
}

CernBrush *
cern_solid_brush_new(CernColor *color) {
  CernSolidBrush *self;

  if (color == NULL) {
    g_warning("Color cannot be NULL");
    return NULL;
  }

  return cern_solid_brush_new_immutable(color, FALSE);
}

CernBrush *
cern_solid_brush_new_immutable(CernColor *color, gboolean is_immutable) {
  CernSolidBrush *self;
  GpSolidFill *native_brush;
  GpStatus status;

  if (color == NULL) {
    g_warning("Color cannot be NULL");
    return NULL;
  }

  status = GdipCreateSolidFill(cern_color_to_argb(color), &native_brush);

  if (status != Ok) {
    g_warning("Failed to create solid brush: %d", status);
    return NULL;
  }

  self = cern_solid_brush_new_internal();
  cern_solid_brush_set_native(self, native_brush);

  return CERN_BRUSH(self);
}

CernColor *
cern_solid_brush_get_color(CernSolidBrush *self) {
  GpStatus status;
  ARGB color;

  status = GdipGetSolidFillColor(cern_solid_brush_get_native(self), &color);

  if (status != Ok) {
    g_warning("Failed to get solid brush color: %d", status);
    return NULL;
  }

  return cern_color_from_argb(color);
}

void
cern_solid_brush_set_color(CernSolidBrush *self, CernColor *color) {
  GpSolidFill *native_brush;
  GpStatus status;

  if (self->is_immutable) {
    g_warning("Cannot change immutable brush");
    return;
  }

  native_brush = cern_solid_brush_get_native(self);
  status = GdipSetSolidFillColor(native_brush, cern_color_to_argb(color));
  if (status != Ok) {
    g_warning("Failed to set solid brush color: %d", status);
  }
}

CernBrush *
cern_solid_brush_clone(CernBrush *brush) {
  GpStatus status;
  GpBrush *clone_brush;

  status
    = GdipCloneBrush(cern_solid_brush_get_native(CERN_SOLID_BRUSH(brush)),
                     &clone_brush);

  if (status != Ok) {
    g_warning("Failed to clone solid brush: %d", status);
    return NULL;
  }

  return CERN_BRUSH(cern_solid_brush_new_with_native(clone_brush));
}

static
gpointer
cern_solid_brush_get_native(CernSolidBrush *self) {
  CernNativeGdiObject *native_object;
  native_object = CERN_NATIVE_GDI_OBJECT(self);
  return cern_native_gdi_object_get_native_handle(native_object);
}

void
cern_solid_brush_set_native(CernSolidBrush *self, gpointer brush) {
  CernNativeGdiObject *native_object;
  native_object = CERN_NATIVE_GDI_OBJECT(self);
  cern_native_gdi_object_set_native_handle(native_object, brush);
}
