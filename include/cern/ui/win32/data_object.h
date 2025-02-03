#ifndef CERN_UI_DATA_OBJECT
#define CERN_UI_DATA_OBJECT 1

#include <glib-object.h>

G_BEGIN_DECLS

G_DECLARE_INTERFACE(CernDataObject, cern_data_object, CERN, DATA_OBJECT, GObject);

struct _CernDataObjectInterface {
  GTypeInterface parent_iface;
};

G_END_DECLS

#endif /* CERN_UIDAT_A_OBJECT */
