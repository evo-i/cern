#include "cern/ui/dpi_helper.h"

#include <Windows.h>

static
gdouble
device_dpi = 96.0;

static
gboolean
is_initialized = FALSE;

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
