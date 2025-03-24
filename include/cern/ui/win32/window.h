#ifndef CERN_UI_WIN32_WINDOW
#define CERN_UI_WIN32_WINDOW 1

#include <glib-object.h>
#include <Windows.h>

G_BEGIN_DECLS

#define CERN_TYPE_WIN32_WINDOW cern_win32_window_get_type ()

G_DECLARE_INTERFACE(CernWin32Window, cern_win32_window, CERN, WIN32_WINDOW, GObject);

struct _CernWin32WindowInterface {
  GTypeInterface parent_iface;

  gpointer (*get_native)(CernWin32Window *self);
};

gpointer
cern_win32_window_get_native(CernWin32Window *self);

G_END_DECLS

#endif /* CERN_UI_WIN32_WINDOW */
