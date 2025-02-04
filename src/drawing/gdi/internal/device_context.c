#include "cern/drawing/internal/device_context.h"
#include "cern/drawing/internal/i_device_context.h"
#include "cern/drawing/device_context_type.h"
#include "cern/drawing/internal/graphics_mode.h"

struct _CernDeviceContextResources {
  HPEN pen;
  HBRUSH brush;
  HBITMAP bitmap;
  HFONT font;
} typedef CDCR;

typedef struct _CernDeviceContextPrivate {
  HDC h_dc;
  enum CernDeviceContextType type;
  HWND h_window;

  CDCR initial;
  CDCR current;

  GQueue *stack;

} CernDeviceContextPrivate;

static
inline
gpointer
cern_device_context_get_instance_private(CernDeviceContext *self);

static
HDC
cern_device_context_iface_get_hdc(CernIDeviceContext *iface) {
  CernDeviceContext *self;
  CernDeviceContextPrivate *priv;

  self = CERN_DEVICE_CONTEXT(iface);
  priv = cern_device_context_get_instance_private(self);

  if (priv->h_dc == NULL) {
    if (priv->type != CernDeviceContextType_Display) {
      g_warn_message("cern drawing library", __FILE__, __LINE__, __func__,
                     "Calling get_hdc from a non display/window device.");
      priv->h_dc = GetDC(priv->h_window);
    }
  }

  return priv->h_dc;
}

static
void
cern_device_context_iface_release_dc(CernIDeviceContext *iface) {
  CernDeviceContext *self;
  CernDeviceContextPrivate *priv;

  self = CERN_DEVICE_CONTEXT(iface);
  priv = cern_device_context_get_instance_private(self);

  if (priv->h_dc != NULL && priv->type == CernDeviceContextType_Display) {
    ReleaseDC(priv->h_window, priv->h_dc);
    priv->h_dc = NULL;
  }
}

static
void
cern_device_context_init_interface(CernIDeviceContextInterface *iface) {
  iface->get_hdc = cern_device_context_iface_get_hdc;
  iface->release_hdc = cern_device_context_iface_release_dc;
}

G_DEFINE_TYPE_WITH_CODE(CernDeviceContext, cern_device_context,
  G_TYPE_OBJECT, G_ADD_PRIVATE(CernDeviceContext)
  G_IMPLEMENT_INTERFACE(CERN_TYPE_IDEVICE_CONTEXT,
    cern_device_context_init_interface));

static
void
carn_drawing_device_context_cache_initial_state(CernDeviceContext *self) {
  CernDeviceContextPrivate *priv;

  priv = cern_device_context_get_instance_private(self);

  priv->initial.pen = GetCurrentObject(priv->h_dc, OBJ_PEN);
  priv->initial.brush = GetCurrentObject(priv->h_dc, OBJ_BRUSH);
  priv->initial.bitmap = GetCurrentObject(priv->h_dc, OBJ_BITMAP);
  priv->initial.font = GetCurrentObject(priv->h_dc, OBJ_FONT);
}

static
void
cern_device_context_init(CernDeviceContext *self) {
  CernDeviceContextPrivate *priv;
  priv = cern_device_context_get_instance_private(self);
  priv->stack = g_queue_new();
}

static
enum CernDeviceContextType
cern_device_context_get_device_context_type_real(CernDeviceContext *self) {
  CernDeviceContextPrivate *priv;

  priv = cern_device_context_get_instance_private(self);

  return priv->type;
}

static
void
cern_device_context_delete_object_real(CernDeviceContext *self,
                                               HGDIOBJ obj,
                                               enum CernGdiObjectType type) {
  HGDIOBJ h_delete = NULL;
  CernDeviceContextPrivate *priv = cern_device_context_get_instance_private(self);

  switch (type) {
    case CernGdiObjectType_Pen: {
      if (obj == priv->current.pen) {
        HGDIOBJ current_pen = SelectObject(priv->h_dc, priv->initial.pen);
        if (current_pen == priv->current.pen) {
          DebugBreak();
        }
        priv->current.pen = NULL;
      }
      h_delete = obj;
    } break;
    case CernGdiObjectType_Brush: {
     if (obj == priv->current.pen) {
        HGDIOBJ current_brush = SelectObject(priv->h_dc, priv->initial.brush);
        if (current_brush == priv->current.brush) {
          DebugBreak();
        }
        priv->current.brush = NULL;
      }
      h_delete = obj;
    } break;
    case CernGdiObjectType_Bitmap: {
      if (obj == priv->current.pen) {
        HGDIOBJ current_bitmap = SelectObject(priv->h_dc, priv->initial.brush);
        if (current_bitmap == priv->current.bitmap) {
          DebugBreak();
        }
        priv->current.bitmap = NULL;
      }
      h_delete = obj;
    } break;
    default:
      g_warn_message("cern_drawing library", __FILE__, __LINE__, __func__,
        "cern_device_context_delete_object_real called with unknown object type.");
      break;
  }

  if (h_delete != NULL && h_delete != HGDI_ERROR) {
    DeleteObject(h_delete);
  }
}

enum CernGraphicsMode
cern_device_context_get_graphics_mode_real(CernDeviceContext *self) {
  CernDeviceContextPrivate *priv;

  priv = cern_device_context_get_instance_private(self);

  return GetGraphicsMode(priv->h_dc);
}

enum CernGraphicsMode
cern_device_context_set_graphics_mode_real(CernDeviceContext *self, enum CernGraphicsMode mode) {
  CernDeviceContextPrivate *priv;

  priv = cern_device_context_get_instance_private(self);

  SetGraphicsMode(priv->h_dc, (int) mode);
}

static
HDC
cern_device_context_get_hdc_real(CernDeviceContext *self) {
  CernIDeviceContext *iface;
  iface = CERN_IDEVICE_CONTEXT(self);
  return cern_i_device_context_get_hdc(iface);
}

static
void
cern_device_context_release_hdc_real(CernDeviceContext *self) {
  CernIDeviceContext *iface;
  iface = CERN_IDEVICE_CONTEXT(self);
  cern_i_device_context_release_hdc(iface);
}

static
void
cern_device_context_dispose(GObject *object) {
  GObjectClass *parent_class;
  CernDeviceContext *self;
  CernDeviceContextPrivate *priv;
  CernIDeviceContext *iface;

  self = CERN_DEVICE_CONTEXT(object);
  priv = cern_device_context_get_instance_private(self);
  iface = CERN_IDEVICE_CONTEXT(object);
  parent_class = G_OBJECT_CLASS(cern_device_context_parent_class);

  switch (priv->type) {
    case CernDeviceContextType_Display: {
      cern_i_device_context_release_hdc(iface);
    } break;
    case CernDeviceContextType_Information:
    case CernDeviceContextType_NamedDevice: {
      DeleteDC(priv->h_dc);
      priv->h_dc = NULL;
    } break;
    case CernDeviceContextType_Memory: {
      DeleteDC(priv->h_dc);
      priv->h_dc = NULL;
    } break;
  }

  parent_class->dispose(object);
}

static
void
cern_device_context_class_init(CernDeviceContextClass *klass) {
  GObjectClass *parent_class = G_OBJECT_CLASS(klass);
  parent_class->dispose = cern_device_context_dispose;

  klass->get_device_context_type  = cern_device_context_get_device_context_type_real;
  klass->get_hdc                  = cern_device_context_get_hdc_real;
  klass->delete_object            = cern_device_context_delete_object_real;
  klass->release_hdc              = cern_device_context_release_hdc_real;
  klass->set_graphics_mode        = cern_device_context_set_graphics_mode_real;
  klass->get_graphics_mode        = cern_device_context_get_graphics_mode_real;
}

HDC
cern_device_context_get_hdc(CernDeviceContext *self) {
  g_return_val_if_fail(CERN_IS_DEVICE_CONTEXT(self), NULL);
  CernDeviceContextClass *klass;
  klass = CERN_DEVICE_CONTEXT_GET_CLASS(self);
  return klass->get_hdc(self);
}

CernDeviceContext *
cern_device_context_new(HWND h_window) {
  CernDeviceContext *self;
  CernDeviceContextPrivate *priv;

  self = g_object_new(CERN_TYPE_DEVICE_CONTEXT, NULL);
  priv = cern_device_context_get_instance_private(self);

  priv->h_window = h_window;
  priv->type = CernDeviceContextType_Display;
  return self;
}

CernDeviceContext *
cern_device_context_from_hdc(HDC h_dc, enum CernDeviceContextType type) {
  CernDeviceContext *self;
  CernDeviceContextPrivate *priv;

  self = g_object_new(CERN_TYPE_DEVICE_CONTEXT, NULL);
  priv = cern_device_context_get_instance_private(self);

  priv->h_dc = h_dc;
  priv->type = type;

  carn_drawing_device_context_cache_initial_state(self);

  if (type == CernDeviceContextType_Display) {
    priv->h_window = WindowFromDC(priv->h_dc);
  }

  return self;
}
