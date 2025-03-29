#include "cern/ui/window_target.h"

G_DEFINE_INTERFACE(CernWindowTarget, cern_window_target, G_TYPE_OBJECT)

static
void
cern_window_target_default_init(CernWindowTargetInterface *iface) { }

void
cern_window_target_on_handle_change(CernWindowTarget *self, gpointer new_handle) {
  CernWindowTargetInterface *iface;
  iface = CERN_WINDOW_TARGET_GET_IFACE(self);
  iface->on_handle_change(self, new_handle);
}

void
cern_window_target_on_message(CernWindowTarget *self, CernMessage *message) {
  CernWindowTargetInterface *iface;
  iface = CERN_WINDOW_TARGET_GET_IFACE(self);
  iface->on_message(self, message);
}
