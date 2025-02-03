#include "cern/drawing/internal/native_gdi_object.h"

G_DEFINE_INTERFACE(CernNativeGdiObject, cern_native_gdi_object, G_TYPE_OBJECT)

static
void
cern_native_gdi_object_default_init(CernNativeGdiObjectInterface *iface) {
  // Initialization code here
}

gpointer
cern_native_gdi_object_get_native_handle(CernNativeGdiObject *self) {
  CernNativeGdiObjectInterface *iface;
  g_return_val_if_fail(CERN_IS_NATIVE_GDI_OBJECT(self), NULL);
  iface = CERN_NATIVE_GDI_OBJECT_GET_IFACE(self);
  g_return_val_if_fail(iface->get_native_handle != NULL, NULL);
  return iface->get_native_handle(self);
}

void
cern_native_gdi_object_set_native_handle(CernNativeGdiObject *self, gpointer handle) {
  CernNativeGdiObjectInterface *iface;
  g_return_if_fail(CERN_IS_NATIVE_GDI_OBJECT(self));
  iface = CERN_NATIVE_GDI_OBJECT_GET_IFACE(self);
  g_return_if_fail(iface->set_native_handle != NULL);
  iface->set_native_handle(self, handle);
}
