#ifndef CERN_DRAWING_DEVICE_CONTEXT_H
#define CERN_DRAWING_DEVICE_CONTEXT_H 1

#include <glib-object.h>

G_BEGIN_DECLS

#include "cern/drawing/device_context_type.h"
#include "cern/drawing/internal/graphics_mode.h"
#include "cern/drawing/internal/gdi_object_type.h"
#include <Windows.h>

#define CERN_TYPE_DEVICE_CONTEXT cern_device_context_get_type ()

G_DECLARE_DERIVABLE_TYPE(CernDeviceContext,
                         cern_device_context,
                         CERN, DEVICE_CONTEXT, GObject);

struct _CernDeviceContextClass {
  GObjectClass parent_class;

  enum CernDeviceContextType
  (*get_device_context_type)(CernDeviceContext *self);

  void
  (*delete_object)(CernDeviceContext *self, HGDIOBJ obj, enum CernGdiObjectType type);

  HDC
  (*get_hdc)(CernDeviceContext *self);

  void
  (*release_hdc)(CernDeviceContext *self);

  enum CernGraphicsMode
  (*get_graphics_mode)(CernDeviceContext *self);

  enum CernGraphicsMode
  (*set_graphics_mode)(CernDeviceContext *self, enum CernGraphicsMode new_mode);
};

CernDeviceContext *
cern_device_context_new(HWND h_window);

CernDeviceContext *
cern_device_context_from_hdc(HDC h_dc, enum CernDeviceContextType type);

HDC
cern_device_context_get_hdc(CernDeviceContext *self);

G_END_DECLS

#endif /* CERN_DRAWING_DEVICE_CONTEXT_H */
