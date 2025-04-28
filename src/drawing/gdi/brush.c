#include "cern/drawing/brush.h"
#include "cern/core/icloneable.h"

#include "cern/drawing/internal/native_gdi_object.h"

#include <windows.h>
#include <gdiplus.h>

typedef struct _CernBrushPrivate {
  GpBrush *native_brush;
} CernBrushPrivate;

static
inline
gpointer
cern_brush_get_instance_private(CernBrush *self);

static
gpointer
cern_brush_get_native(CernBrush *self);

static
void
cern_brush_set_native(CernBrush *self, gpointer brush);


static
CernICloneable *
cern_brush_real_clone(CernICloneable *iface) {
  CernBrush *clone, *self;
  CernBrushPrivate *clone_priv;
  CernBrushPrivate *priv;
  GpBrush *current_brush;
  GpBrush *clone_brush;

  self = CERN_BRUSH(iface);

  priv = cern_brush_get_instance_private(self);

  if (priv->native_brush == NULL) {
    return NULL;
  }

  current_brush = priv->native_brush;
  clone_brush = NULL;
  clone = NULL;
  clone_priv = NULL;

  if(GdipCloneBrush(current_brush, &clone_brush) == Ok) {
    clone = g_object_new(CERN_TYPE_BRUSH, NULL);
    clone_priv = cern_brush_get_instance_private(clone);
    clone_priv->native_brush = clone_brush;
  }

  return CERN_ICLONEABLE(clone);
}

static
void
cern_brush_cloneable_init(CernICloneableInterface *iface) {
  iface->clone = cern_brush_real_clone;
}

static
void
cern_brush_native_gdi_init(CernNativeGdiObjectInterface *iface) {
  iface->get_native_handle
    = (gpointer (*)(CernNativeGdiObject *)) cern_brush_get_native;
  iface->set_native_handle
    = (void (*)(CernNativeGdiObject *, gpointer)) cern_brush_set_native;
}

#define CERN_BRUSH_CLONEABLE()\
G_IMPLEMENT_INTERFACE(CERN_TYPE_ICLONEABLE, cern_brush_cloneable_init)

#define CERN_BRUSH_NATIVE_GDI()\
G_IMPLEMENT_INTERFACE(CERN_TYPE_NATIVE_GDI_OBJECT, cern_brush_native_gdi_init)

G_DEFINE_ABSTRACT_TYPE_WITH_CODE(
  CernBrush,
  cern_brush,
  G_TYPE_OBJECT,
  G_ADD_PRIVATE(CernBrush)
  CERN_BRUSH_CLONEABLE()
  CERN_BRUSH_NATIVE_GDI())

static
void
cern_brush_finalize(GObject *object);

static
void
cern_brush_class_init(CernBrushClass *klass) {
  G_OBJECT_CLASS(klass)->finalize = cern_brush_finalize;
}

static
void
cern_brush_init(CernBrush *self) {
  (void) self;
  // Initialize instance variables here
}

CernBrush *
cern_brush_clone(CernBrush *self) {
  CernBrushClass *klass = CERN_BRUSH_GET_CLASS(self);
  return klass->clone(self);
}

gpointer
cern_brush_get_native(CernBrush *self) {
  CernBrushPrivate *priv;

  g_return_val_if_fail(CERN_IS_BRUSH(self), NULL);

  priv = cern_brush_get_instance_private(self);

  return priv->native_brush;
}

void
cern_brush_set_native(CernBrush *self, gpointer brush) {
  CernBrushPrivate *priv;

  g_return_if_fail(CERN_IS_BRUSH(self));

  priv = cern_brush_get_instance_private(self);

  priv->native_brush = brush;
}

static
void
cern_brush_finalize(GObject *object) {
  CernBrush *self;

  self = CERN_BRUSH(object);

  if (cern_brush_get_native(self) != NULL) {
    GdipDeleteBrush(cern_brush_get_native(self));
    cern_brush_set_native(self, NULL);
  }

  G_OBJECT_CLASS(cern_brush_parent_class)->finalize(object);
}
