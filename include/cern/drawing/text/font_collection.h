#ifndef CERN_DRAWING_FONT_COLLECTION_H
#define CERN_DRAWING_FONT_COLLECTION_H 1

#include <glib-object.h>

G_BEGIN_DECLS

#define CERN_TYPE_FONT_COLLECTION (cern_font_collection_get_type())

G_DECLARE_DERIVABLE_TYPE(CernFontCollection, cern_font_collection, CERN, FONT_COLLECTION, GObject)

struct _CernFontCollectionClass {
  GObjectClass parent_class;

  GPtrArray *
  (*get_families)(CernFontCollection *self);
};

GPtrArray *
cern_font_collection_get_families(CernFontCollection *self);

G_END_DECLS

#endif /* CERN_DRAWING_FONT_COLLECTION_H */
