#ifndef CERN_UI_CONTROL_COLLECTION_H
#define CERN_UI_CONTROL_COLLECTION_H 1

#include <glib-object.h>

G_BEGIN_DECLS

#define CERN_CONTROL_COLLECTION_TYPE (cern_control_collection_get_type())

G_DECLARE_FINAL_TYPE(CernControlCollection, cern_control_collection, CERN, CONTROL_COLLECTION, GObject)

G_END_DECLS

#endif /* CERN_UI_CONTROL_COLLECTION_H */
