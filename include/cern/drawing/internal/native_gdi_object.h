#ifndef CERN_INTERNAL_NATIVE_GDI_OBJECT_H
#define CERN_INTERNAL_NATIVE_GDI_OBJECT_H 1

#include <glib-object.h>

G_BEGIN_DECLS

#define CERN_TYPE_NATIVE_GDI_OBJECT (cern_native_gdi_object_get_type())

G_DECLARE_INTERFACE(
  CernNativeGdiObject,
  cern_native_gdi_object,
  CERN, NATIVE_GDI_OBJECT,
  GObject)

struct _CernNativeGdiObjectInterface {
  GTypeInterface parent_interface;

  gpointer (*get_native_handle)(CernNativeGdiObject *self);
  void (*set_native_handle)(CernNativeGdiObject *self, gpointer handle);
};

gpointer
cern_native_gdi_object_get_native_handle(CernNativeGdiObject *self);

void
cern_native_gdi_object_set_native_handle(CernNativeGdiObject *self, gpointer handle);

G_END_DECLS

#endif /* CERN_INTERNAL_NATIVE_GDI_OBJECT_H */
