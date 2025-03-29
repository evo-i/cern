#ifndef CERN_UI_CONTROL_BINDINGS_COLLECTION_H
#define CERN_UI_CONTROL_BINDINGS_COLLECTION_H 1

#include <glib-object.h>

G_BEGIN_DECLS

#define CERN_CONTROL_BINDINGS_COLLECTION_TYPE (cern_control_bindings_collection_get_type())

G_DECLARE_FINAL_TYPE(CernControlBindingsCollection, cern_control_bindings_collection, CERN, CONTROL_BINDINGS_COLLECTION, GObject)


G_END_DECLS

#endif /* CERN_UI_CONTROL_BINDINGS_COLLECTION_H */
