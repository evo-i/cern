#ifndef CERN_CORE_ICLONEABLE_H
#define CERN_CORE_ICLONEABLE_H 1

#include <glib-object.h>

G_BEGIN_DECLS

#define CERN_TYPE_ICLONEABLE (cern_i_cloneable_get_type())

G_DECLARE_INTERFACE (CernICloneable, cern_i_cloneable, CERN, ICLONEABLE, GObject)

struct _CernICloneableInterface {
  GTypeInterface parent_iface;

  CernICloneable *
  (*clone) (CernICloneable *self);
};

CernICloneable *
cern_i_cloneable_clone(CernICloneable *self);

G_END_DECLS

#endif // CERN_CORE_ICLONEABLE_H
