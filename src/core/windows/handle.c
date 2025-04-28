#include "cern/core/windows/handle.h"

G_DEFINE_INTERFACE(CernHandle, cern_handle, G_TYPE_OBJECT)

static
void
cern_handle_default_init(CernHandleInterface *iface) {
  (void) iface;
}

gpointer
cern_handle_get(gpointer self) {
  g_return_val_if_fail(CERN_IS_HANDLE(self), NULL);
  CernHandle *handle = CERN_HANDLE(self);
  CernHandleInterface *iface = CERN_HANDLE_GET_IFACE(handle);
  g_return_val_if_fail(iface->get, NULL);
  return iface->get(handle);
}

void
cern_handle_set(gpointer self, gpointer value) {
  g_return_if_fail(CERN_IS_HANDLE(self));
  CernHandle *handle = CERN_HANDLE(self);
  CernHandleInterface *iface = CERN_HANDLE_GET_IFACE(handle);
  g_return_if_fail(iface->set);
  iface->set(handle, value);
}
