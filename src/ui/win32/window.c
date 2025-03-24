#include "cern/ui/win32/window.h"

G_DEFINE_INTERFACE(CernWin32Window, cern_win32_window, G_TYPE_OBJECT)

void
static
cern_win32_window_default_init(CernWin32WindowInterface *iface) {
  // Add default initialization code here
}

gpointer
cern_win32_window_get_native(CernWin32Window *self) {
  CernWin32WindowInterface *iface;

  g_return_val_if_fail(CERN_IS_WIN32_WINDOW(self), NULL);

  iface = CERN_WIN32_WINDOW_GET_IFACE(self);

  g_return_val_if_fail(iface->get_native != NULL, NULL);

  return (iface->get_native)(self);
}
