#ifndef CERN_UI_OLE_WINDOW_H
#define CERN_UI_OLE_WINDOW_H 1

#include <glib-object.h>

G_BEGIN_DECLS

#define CERN_TYPE_OLE_WINDOW cern_ole_window_get_type ()

G_DECLARE_INTERFACE(CernOleWindow, cern_ole_window, CERN, OLE_WINDOW, GObject)

struct _CernOleWindowInterface {
  GTypeInterface parent_interface;

  gint32
  (*get_window)(CernOleWindow *self, gpointer *value);

  void
  (*context_sensitive_help)(CernOleWindow *self, gint32 f_enter_mode);
};

gint32
cern_ole_window_get_window(CernOleWindow *self, gpointer *value);

void
cern_ole_window_context_sensitive_help(CernOleWindow *self, gint32 f_enter_mode);

G_END_DECLS

#endif /* CERN_UI_OLE_WINDOW_H */
