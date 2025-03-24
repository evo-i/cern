#include "cern/core/icloneable.h"

G_DEFINE_INTERFACE (CernICloneable, cern_i_cloneable, G_TYPE_OBJECT)

static void
cern_i_cloneable_default_init (CernICloneableInterface *iface) { }

CernICloneable *
cern_i_cloneable_clone(CernICloneable *self) {
  g_return_val_if_fail (CERN_IS_ICLONEABLE(self), NULL);
  return CERN_ICLONEABLE_GET_IFACE (self)->clone (self);
}
