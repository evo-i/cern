#ifndef CERN_DRAWING_INTERNAL_WINDOWS_GRAPHICS_H
#define CERN_DRAWING_INTERNAL_WINDOWS_GRAPHICS_H 1

#include <glib-object.h>

#include "device_context.h"
#include "apply_graphics_properties.h"

G_BEGIN_DECLS

typedef struct _CernGraphics CernGraphics;

#define CERN_TYPE_WINDOWS_GRAPHICS cern_windows_graphics_get_type ()

G_DECLARE_FINAL_TYPE(CernWindowsGraphics, cern_windows_graphics, CERN, WINDOWS_GRAPHICS, GObject)

CernWindowsGraphics *
cern_windows_graphics_new(CernDeviceContext *device_context);

CernWindowsGraphics *
cern_windows_graphics_measurement(void);

CernWindowsGraphics *
cern_windows_graphics_measurement_from_screen_dc(gpointer screen_dc);

CernWindowsGraphics *
cern_windows_graphics_from_hwnd(gpointer h_wnd);

CernWindowsGraphics *
cern_windows_graphics_from_hdc(gpointer h_dc);

CernWindowsGraphics *
cern_windows_graphics_from_graphics(CernGraphics *graphics);

CernWindowsGraphics *
cern_windows_graphics_from_graphics_with_properties(CernGraphics *graphics,
                                                    CernApplyGraphicsProperties properties);

CernDeviceContext *
cern_windows_graphics_get_device_context(CernWindowsGraphics *self);

gpointer
cern_windows_graphics_get_hdc(CernWindowsGraphics *self);

void
cern_windows_graphics_release_hdc(CernWindowsGraphics *self);

G_END_DECLS

#endif /* CERN_DRAWING_INTERNAL_WINDOWS_GRAPHICS_H */
