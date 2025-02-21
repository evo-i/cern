#include "cern/drawing/internal/device_context.h"
#include "cern/drawing/internal/i_device_context.h"
#include "cern/drawing/internal/device_context_type.h"
#include "cern/drawing/internal/graphics_mode.h"
#include "cern/drawing/internal/native_gdi_object.h"
#include "cern/drawing/internal/region_combine_mode.h"
#include "cern/drawing/region.h"

#include <Windows.h>
#include <gdiplus.h>

typedef struct _CernGraphicsState {
  gpointer h_pen;
  gpointer h_brush;
  gpointer h_bitmap;
  gpointer h_font;
} CernGraphicsState;

struct _CernDeviceContext {
  GObject parent_isntance;

  gpointer h_dc;
  gpointer h_window;

  CernGraphicsState initial;
  CernGraphicsState current;

  CernDeviceContextType dc_type;

  GQueue context_stack;
};

static
gpointer
cern_device_context_iface_get_hdc(CernIDeviceContext *iface) {
  CernDeviceContext *self;

  self = CERN_DEVICE_CONTEXT(iface);

  if (self->h_dc == NULL) {
    if (self->dc_type != CernDeviceContextType_Display) {
      g_warn_message("cern drawing library", __FILE__, __LINE__, __func__,
                     "Calling get_hdc from a non display/window device.");
      self->h_dc = GetDC(self->h_window);
    }
  }

  return self->h_dc;
}

static
void
cern_device_context_iface_release_dc(CernIDeviceContext *iface) {
  CernDeviceContext *self;
  self = CERN_DEVICE_CONTEXT(iface);

  if (self->h_dc != NULL && self->dc_type == CernDeviceContextType_Display) {
    ReleaseDC(self->h_window, self->h_dc);
    self->h_dc = NULL;
  }
}

static
void
cern_device_context_init_interface(CernIDeviceContextInterface *iface) {
  iface->get_hdc = cern_device_context_iface_get_hdc;
  iface->release_hdc = cern_device_context_iface_release_dc;
}

G_DEFINE_FINAL_TYPE_WITH_CODE(CernDeviceContext, cern_device_context,
  G_TYPE_OBJECT,
  G_IMPLEMENT_INTERFACE(CERN_TYPE_IDEVICE_CONTEXT,
    cern_device_context_init_interface));

static
void
carn_drawing_device_context_cache_initial_state(CernDeviceContext *self) {
  self->initial.h_pen = GetCurrentObject(self->h_dc, OBJ_PEN);
  self->initial.h_brush = GetCurrentObject(self->h_dc, OBJ_BRUSH);
  self->initial.h_bitmap = GetCurrentObject(self->h_dc, OBJ_BITMAP);
  self->initial.h_font = GetCurrentObject(self->h_dc, OBJ_FONT);
}

static
void
cern_device_context_init(CernDeviceContext *self) {
  self->context_stack = (GQueue) G_QUEUE_INIT;
}

CernDeviceContextType
cern_device_context_get_device_context_type(CernDeviceContext *self) {
  return self->dc_type;
}

void
cern_device_context_delete_object(CernDeviceContext *self,
                                  gpointer obj,
                                  CernGdiObjectType type) {
  HGDIOBJ h_delete = NULL;

  switch (type) {
    case CernGdiObjectType_Pen: {
      if (obj == self->current.h_pen) {
        HGDIOBJ current_pen = SelectObject(self->h_dc, self->initial.h_pen);
        if (current_pen == self->current.h_pen) {
          DebugBreak();
        }
        self->current.h_pen = NULL;
      }
      h_delete = obj;
    } break;
    case CernGdiObjectType_Brush: {
     if (obj == self->current.h_pen) {
        HGDIOBJ current_brush = SelectObject(self->h_dc, self->initial.h_brush);
        if (current_brush == self->current.h_brush) {
          DebugBreak();
        }
        self->current.h_brush = NULL;
      }
      h_delete = obj;
    } break;
    case CernGdiObjectType_Bitmap: {
      if (obj == self->current.h_pen) {
        HGDIOBJ current_bitmap = SelectObject(self->h_dc, self->initial.h_brush);
        if (current_bitmap == self->current.h_bitmap) {
          DebugBreak();
        }
        self->current.h_bitmap = NULL;
      }
      h_delete = obj;
    } break;
    default:
      g_warn_message("cern_drawing library", __FILE__, __LINE__, __func__,
        "cern_device_context_delete_object called with unknown object type.");
      break;
  }

  if (h_delete != NULL && h_delete != HGDI_ERROR) {
    DeleteObject(h_delete);
  }
}

CernDeviceContextGraphicsMode
cern_device_context_get_graphics_mode(CernDeviceContext *self) {
  return
    (CernDeviceContextGraphicsMode) GetGraphicsMode((HDC) self->h_dc);
}

CernDeviceContextGraphicsMode
cern_device_context_set_graphics_mode(CernDeviceContext *self, CernDeviceContextGraphicsMode mode) {
  return
    (CernDeviceContextGraphicsMode) SetGraphicsMode((HDC) self->h_dc, (int) mode);
}

gpointer
cern_device_context_get_hdc(CernDeviceContext *self) {
  CernIDeviceContext *iface;
  g_return_val_if_fail(CERN_IS_IDEVICE_CONTEXT(self), NULL);
  iface = CERN_IDEVICE_CONTEXT(self);
  return cern_i_device_context_get_hdc(iface);
}

void
cern_device_context_release_hdc(CernDeviceContext *self) {
  CernIDeviceContext *iface;
  g_return_if_fail(CERN_IS_IDEVICE_CONTEXT(self));
  iface = CERN_IDEVICE_CONTEXT(self);
  cern_i_device_context_release_hdc(iface);
}

static
void
cern_device_context_dispose(GObject *object) {
  GObjectClass *parent_class;
  CernDeviceContext *self;
  CernIDeviceContext *iface;

  self = CERN_DEVICE_CONTEXT(object);
  iface = CERN_IDEVICE_CONTEXT(object);
  parent_class = G_OBJECT_CLASS(cern_device_context_parent_class);

  switch (self->dc_type) {
    case CernDeviceContextType_Display: {
      cern_i_device_context_release_hdc(iface);
    } break;
    case CernDeviceContextType_Information:
    case CernDeviceContextType_NamedDevice: {
      DeleteDC((HDC) self->h_dc);
      self->h_dc = NULL;
    } break;
    case CernDeviceContextType_Memory: {
      DeleteDC((HDC) self->h_dc);
      self->h_dc = NULL;
    } break;
  }

  parent_class->dispose(object);
}

static
void
cern_device_context_finalize(GObject *obj) {
  CernDeviceContext *self;

  self = CERN_DEVICE_CONTEXT(obj);

  G_OBJECT_CLASS(cern_device_context_parent_class)->finalize(obj);
}

static
void
cern_device_context_class_init(CernDeviceContextClass *klass) {
  GObjectClass *parent_class = G_OBJECT_CLASS(klass);
  parent_class->dispose = cern_device_context_dispose;
  parent_class->finalize = cern_device_context_finalize;
}

CernDeviceContext *
cern_device_context_new_dc(gchar const *driver_name, gchar const *device_name,
                           gchar const *file_name, DEVMODEA const *dev_mode) {
  gpointer h_dc;

  h_dc = CreateDCA(driver_name, device_name, file_name, dev_mode);

  if (h_dc == NULL) {
    g_critical("%s(...): failed: CreateDCA: %d", __func__, GetLastError());
    return NULL;
  }

  return cern_device_context_new_hdc(h_dc, CernDeviceContextType_NamedDevice);
}

CernDeviceContext *
cern_device_context_new_ic(gchar const *driver_name, gchar const *device_name,
                           gchar const *file_name, DEVMODEA const *dev_mode) {
  gpointer h_dc;

  h_dc = CreateICA(driver_name, device_name, file_name, dev_mode);

  if (h_dc == NULL) {
    g_critical("%s(...): failed: CreateICA: %d", __func__, GetLastError());
    return NULL;
  }

  return cern_device_context_new_hdc(h_dc, CernDeviceContextType_Information);
}

CernDeviceContext *
cern_device_context_new_from_compatible_dc(gpointer h_dc) {
  return
    cern_device_context_new_hdc((gpointer) CreateCompatibleDC((HDC) h_dc),
                                CernDeviceContextType_Memory);
}

CernDeviceContext *
cern_device_context_new(gpointer h_window) {
  CernDeviceContext *self;

  self = g_object_new(CERN_TYPE_DEVICE_CONTEXT, NULL);

  self->h_window = h_window;
  self->dc_type = CernDeviceContextType_Display;
  return self;
}

CernDeviceContext *
cern_device_context_new_hdc(gpointer h_dc, CernDeviceContextType type) {
  CernDeviceContext *self;

  self = g_object_new(CERN_TYPE_DEVICE_CONTEXT, NULL);

  self->h_dc = h_dc;
  self->dc_type = type;

  carn_drawing_device_context_cache_initial_state(self);

  if (type == CernDeviceContextType_Display) {
    self->h_window = (gpointer) WindowFromDC((HDC) self->h_dc);
  }

  return self;
}

void
cern_device_context_restore_hdc(CernDeviceContext *self) {
  RestoreDC(self->h_dc, -1);

  if (g_queue_is_empty(&self->context_stack)) {
    g_critical("Someone call cern_device_context_restore_hdc before calling cern_device_context_save_hdc.");
    return;
  }

  CernGraphicsState *state;

  state = g_queue_pop_tail(&self->context_stack);

  self->current = *state;

  g_free(state);
}

void
cern_device_context_save_hdc(CernDeviceContext *self) {
  CernGraphicsState *state;
  SaveDC(self->h_dc);

  state = g_new0(CernGraphicsState, 1);
  *state = self->current;
  g_queue_push_tail(&self->context_stack, state);
}

void
cern_device_context_set_clip(CernDeviceContext *self,
                             CernWindowsRegion *region) {
  CernNativeGdiObject *native_region;
  gpointer native_handle;

  native_region = CERN_NATIVE_GDI_OBJECT(region);
  native_handle = cern_native_gdi_object_get_native_handle(native_region);

  SelectClipRgn(self->h_dc,native_handle);
}

void
cern_device_context_intersect_clip(CernDeviceContext *self,
                                   CernWindowsRegion *region) {
  CernWindowsRegion *clip;
  CernNativeGdiObject *native_region, *native_clip;
  gpointer native_region_handle, native_clip_handle;

  native_region = CERN_NATIVE_GDI_OBJECT(region);
  native_region_handle = cern_native_gdi_object_get_native_handle(native_region);

  if (native_region_handle == NULL) {
    return;
  }

  clip = cern_windows_region_new_with_rectangle(0, 0, 0, 0);
  native_clip = CERN_NATIVE_GDI_OBJECT(clip);
  native_clip_handle = cern_native_gdi_object_get_native_handle(native_clip);

  int result = GetClipRgn(self->h_dc, native_clip_handle);

  if (result == 1) {
    cern_windows_region_combine(region, clip, region, CernRegionCombineMode_And);
  }

  cern_device_context_set_clip(self, region);

  g_object_unref(clip);
}

void
cern_device_context_translate_transform(CernDeviceContext *self,
                                        gint32 dx, gint32 dy) {
  POINT pt = { 0 };

  OffsetViewportOrgEx(self->h_dc, dx, dy, &pt);
}

gboolean
cern_device_context_equals(CernDeviceContext *self, CernDeviceContext *other) {
  return self->h_dc == other->h_dc;
}
