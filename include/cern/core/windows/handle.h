#ifndef CERN_CORE_HANDLE_H
#define CERN_CORE_HANDLE_H 1

#include <glib-object.h>

G_BEGIN_DECLS

#define CERN_TYPE_HANDLE cern_handle_get_type()

G_DECLARE_INTERFACE(CernHandle, cern_handle, CERN, HANDLE, GObject);

struct _CernHandleInterface {
  GTypeInterface parent_interface;

  gpointer
  (*get)(gpointer self);

  void
  (*set)(gpointer self, gpointer value);
};

gpointer
cern_handle_get(gpointer self);

void
cern_handle_set(gpointer self, gpointer value);

G_END_DECLS

#endif /* CERN_CORE_HANDLE_H */
