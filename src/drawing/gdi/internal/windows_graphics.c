#include "cern/drawing/internal/windows_graphics.h"
#include "cern/drawing/internal/i_device_context.h"
#include "cern/drawing/internal/windows_region.h"
#include "cern/drawing/2d/matrix.h"
#include "cern/drawing/graphics.h"

struct _CernWindowsGraphics {
  GObject parent_instance;

  gboolean finalize_dc;
  CernDeviceContext *device_context;
  CernGraphics *graphics;
};

void
cern_windows_graphics_device_context_interface_init(CernIDeviceContextInterface *iface) {
  iface->get_hdc
    = (gpointer(*)(CernIDeviceContext *)) cern_windows_graphics_get_hdc;

  iface->release_hdc
    = (void(*)(CernIDeviceContext *)) cern_windows_graphics_release_hdc;
}

G_DEFINE_FINAL_TYPE_WITH_CODE(CernWindowsGraphics,
    cern_windows_graphics,
    G_TYPE_OBJECT,
    G_IMPLEMENT_INTERFACE(CERN_TYPE_IDEVICE_CONTEXT,
                          cern_windows_graphics_device_context_interface_init));

static
void
cern_windows_graphics_init(CernWindowsGraphics *self) {
  self->finalize_dc = FALSE;
}

static
void
cern_windows_graphics_finalize(GObject *object) {
  CernWindowsGraphics *self = CERN_WINDOWS_GRAPHICS(object);

  if (self->finalize_dc) {
    cern_device_context_restore_hdc(self->device_context);
  }

  g_clear_object(&self->graphics);
  g_clear_object(&self->device_context);
  G_OBJECT_CLASS(cern_windows_graphics_parent_class)->finalize(object);
}

static
void
cern_windows_graphics_class_init(CernWindowsGraphicsClass *klass) {
  GObjectClass *object_class = G_OBJECT_CLASS(klass);
  object_class->finalize = cern_windows_graphics_finalize;
}

CernWindowsGraphics *
cern_windows_graphics_new(CernDeviceContext *device_context) {
  CernWindowsGraphics *self;

  if (device_context == NULL) {
    g_critical("Device context is NULL");
    return NULL;
  }

  self = g_object_new(CERN_TYPE_WINDOWS_GRAPHICS, NULL);
  self->device_context = device_context;
  cern_device_context_save_hdc(self->device_context);
  return self;
}

CernWindowsGraphics *
cern_windows_graphics_measurement(void) {
  return
    cern_windows_graphics_measurement_from_screen_dc(NULL);
}

CernWindowsGraphics *
cern_windows_graphics_measurement_from_screen_dc(gpointer screen_dc) {
  CernWindowsGraphics *self;
  CernDeviceContext *device_context;

  device_context
    = cern_device_context_new_hdc(screen_dc, CernDeviceContextType_Unknown);

  if (device_context == NULL) {
    g_critical("Failed to create device context from screen DC");
    return NULL;
  }

  self = cern_windows_graphics_new(device_context);
  self->finalize_dc = TRUE;

  return self;
}

CernWindowsGraphics *
cern_windows_graphics_from_hwnd(gpointer h_wnd) {
  CernWindowsGraphics *self;
  CernDeviceContext *device_context;

  device_context = cern_device_context_new(h_wnd);

  if (device_context == NULL) {
    g_critical("Failed to create device context from HWND");
    return NULL;
  }

  self = cern_windows_graphics_new(device_context);
  self->finalize_dc = TRUE;
  return self;
}

CernWindowsGraphics *
cern_windows_graphics_from_hdc(gpointer h_dc) {
  CernWindowsGraphics *self;
  CernDeviceContext *device_context;

  device_context
    = cern_device_context_new_hdc(h_dc, CernDeviceContextType_Unknown);

  if (device_context == NULL) {
    g_critical("Failed to create device context from HDC");
    return NULL;
  }

  self = cern_windows_graphics_new(device_context);
  self->finalize_dc = TRUE;
  return self;
}

CernWindowsGraphics *
cern_windows_graphics_from_graphics(CernGraphics *graphics) {
  return
  cern_windows_graphics_from_graphics_with_properties(graphics, CernApplyGraphicsProperties_All);
}

CernWindowsGraphics *
cern_windows_graphics_from_graphics_with_properties(CernGraphics *graphics,
                                                    CernApplyGraphicsProperties properties) {
  CernWindowsGraphics *self;
  CernRegion *clip_region;
  CernWindowsRegion *windows_region;
  CernMatrix *world_transform;
  gfloat elements[6] = { 0 };
  gsize elements_count = 0;

  if (graphics == NULL) {
    g_critical("Graphics is NULL");
    return NULL;
  }

  if ((properties & CernApplyGraphicsProperties_TranslateTransform)
      || (properties & CernApplyGraphicsProperties_Clipping)) {
    GObject *instance = cern_graphics_get_context_info(graphics);
    if (instance == NULL) {
      g_critical("Failed to get context info from graphics");
      return NULL;
    }

    clip_region = g_object_get_data(instance, "clip");
    world_transform = g_object_get_data(instance, "transform");

    g_clear_object(&instance);

    if (world_transform != NULL) {
      if (properties & CernApplyGraphicsProperties_TranslateTransform) {
        cern_matrix_get_elements(world_transform, elements, &elements_count);
      }
      g_clear_object(&world_transform);
    }

    if (clip_region != NULL) {
      if (properties & CernApplyGraphicsProperties_Clipping) {
        if (!cern_region_is_infinite(clip_region, graphics)) {
          windows_region = cern_windows_region_new_from_region(clip_region, graphics);
        }
      }
    }

    g_clear_object(&clip_region);
  }

  if (windows_region != NULL) {
    g_clear_object(&windows_region);
  }

  self = cern_windows_graphics_from_hdc(cern_graphics_get_hdc(graphics));
  self->graphics = graphics;

  if (windows_region != NULL) {
    cern_device_context_intersect_clip(self->device_context, windows_region);
  }

  if (elements_count > 0) {
    cern_device_context_translate_transform(self->device_context, elements[4], elements[5]);
  }

  return self;
}

CernDeviceContext *
cern_windows_graphics_get_device_context(CernWindowsGraphics *self) {
  return self->device_context;
}

gpointer
cern_windows_graphics_get_hdc(CernWindowsGraphics *self) {
  return cern_device_context_get_hdc(self->device_context);
}

void
cern_windows_graphics_release_hdc(CernWindowsGraphics *self) {
  g_clear_object(&self->device_context);
}
