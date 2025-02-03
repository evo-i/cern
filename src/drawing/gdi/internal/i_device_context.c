#include "cern/drawing/internal/i_device_context.h"

G_DEFINE_INTERFACE(CernIDeviceContext, cern_i_device_context, G_TYPE_OBJECT)

static
void
cern_i_device_context_default_init(CernIDeviceContextInterface *iface) { }

HDC
cern_i_device_context_get_hdc(CernIDeviceContext *self) {
  CernIDeviceContextInterface *iface;

  g_return_val_if_fail(CERN_IS_IDEVICE_CONTEXT(self), NULL);

  iface = CERN_IDEVICE_CONTEXT_GET_IFACE(self);

  return (iface->get_hdc)(self);
}

void
cern_i_device_context_release_hdc(CernIDeviceContext *self) {
  CernIDeviceContextInterface *iface;

  g_return_if_fail(CERN_IS_IDEVICE_CONTEXT(self));

  iface = CERN_IDEVICE_CONTEXT_GET_IFACE(self);

  (iface->release_hdc)(self);
}
