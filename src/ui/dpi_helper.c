#include "cern/ui/dpi_helper.h"

#include <Windows.h>
#include <glib.h>
#include <minwindef.h>
#include <sysinfoapi.h>
#include <windef.h>
#include <winnt.h>
#include <winuser.h>
#include <processthreadsapi.h>

extern
WINAPI
DPI_AWARENESS_CONTEXT
GetDpiAwarenessContextForProcess(HANDLE process);

extern
WINAPI
BOOL
AreDpiAwarenessContextsEqual(
  DPI_AWARENESS_CONTEXT dpiContextA,
  DPI_AWARENESS_CONTEXT dpiContextB
);

static
gdouble
device_dpi = 96.0;

static
gboolean
is_initialized = FALSE;

static
gboolean
is_dpi_helper_quirks_initialized = FALSE;

void
cern_dpi_helper_initialize(void) {
  if (is_initialized) {
    return;
  }
 
  HDC hdc = GetDC(NULL);
  device_dpi = GetDeviceCaps(hdc, LOGPIXELSX);
  ReleaseDC(NULL, hdc);

  is_initialized = TRUE;
}

gdouble
cern_dpi_helper_get_device_dpi(void) {
  cern_dpi_helper_initialize();
  return device_dpi;
}

gboolean
cern_dpi_helper_get_is_awareness_value_set(void) {
  DPI_AWARENESS_CONTEXT context
    = GetDpiAwarenessContextForProcess(NULL);

  if (AreDpiAwarenessContextsEqual(context, DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE)
      || AreDpiAwarenessContextsEqual(context, DPI_AWARENESS_CONTEXT_SYSTEM_AWARE)
      || AreDpiAwarenessContextsEqual(context, DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2)) {
    return TRUE;
  }

  return FALSE;
}

void
cern_dpi_helper_initialize_helper_windows(void) {
  cern_dpi_helper_initialize();
  cern_dpi_helper_initialize_helper_quirks();
}

void
cern_dpi_helper_initialize_helper_quirks() {
  if (is_dpi_helper_quirks_initialized) {
    return;
  }

  OSVERSIONINFO os_info = {
    .dwOSVersionInfoSize = sizeof(OSVERSIONINFO),
    0
  };

  GetVersionEx(&os_info);

  OSVERSIONINFOEX os_info_ex = {
    .dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX),
    0
  };

  // GetVersionEx(&os_info_ex);
}

gboolean
cern_dpi_helper_get_enable_dpi_changed_high_dpi_improvements(void);

gboolean
cern_dpi_helper_get_enable_tool_strip_high_dpi_improvements(void);

gboolean
cern_dpi_hepler_get_enable_tool_strip_per_monitor_v2_high_dpi_improvements(void);

gboolean
cern_dpi_helper_get_enable_dpi_changed_message_handling(void);

gboolean
cern_dpi_helper_get_enable_checked_list_box_high_dpi_improvements(void);

gboolean
cern_dpi_helper_get_enable_single_pass_scaling_of_dpi_windows(void);

gboolean
cern_dpi_helper_get_enable_thread_exception_dialog_high_dpi_improvements(void);

gboolean
cern_dpi_helper_get_enable_data_grid_view_control_hihg_dpi_improvements(void);

gboolean
cern_dpi_helper_get_enable_anchor_layout_high_dpi_improvements(void);

gboolean
cern_dpi_helper_get_enable_month_calendar_high_dpi_improvements(void);

gdouble
cern_dpi_helper_get_device_dpi(void);

gboolean
cern_dpi_helper_get_is_scaling_required(void);

gint32
cern_dpi_helper_logical_to_device_units(gint32 value, gint32 device_pixels);

gdouble
cern_dpi_helper_logical_to_device_units_double(gdouble value, gint32 device_pixels);

gint32
cern_dpi_helper_logical_to_device_units_x(gint32 value);

gint32
cern_dpi_helper_logical_to_device_units_y(gint32 value);

CernSize
cern_dpi_helper_logical_to_device_units_size(CernSize logical_size, gint32 device_dpi);

CernBitmap *
cern_dpi_helper_create_resized_bitmap(CernBitmap *logical_image, CernSize target_size_image);

void
cern_dpi_helper_scale_bitmap_logical_to_device(CernBitmap **logical_image, gint32 device_dpi);

gint32
cern_dpi_helper_convert_to_given_dpi_pixel(gint32 value, gdouble pixel_factor);

void
cern_dpi_helper_scale_button_image_logical_to_device(CernButton *btn);

