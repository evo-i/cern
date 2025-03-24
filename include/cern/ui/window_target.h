#ifndef CERN_UI_WINDOW_TARGET_H
#define CERN_UI_WINDOW_TARGET_H 1

#include <glib-object.h>
#include "message.h"

G_BEGIN_DECLS

#define CERN_TYPE_WINDOW_TARGET cern_window_target_get_type ()

G_DECLARE_INTERFACE(CernWindowTarget, cern_window_target, CERN, WINDOW_TARGET, GObject)

struct _CernWindowTargetInterface {
  GTypeInterface parent_interface;

  void
  (*on_handle_change)(CernWindowTarget *self, gpointer new_handle);

  void
  (*on_message)(CernWindowTarget *self, CernMessage *message);
};

void
cern_window_target_on_handle_change(CernWindowTarget *self, gpointer new_handle);

void
cern_window_target_on_message(CernWindowTarget *self, CernMessage *message);

G_END_DECLS

#endif /* CERN_UI_WINDOW_TARGET_H */
