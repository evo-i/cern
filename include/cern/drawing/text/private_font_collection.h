#ifndef CERN_DRAWING_TEXT_PRIVATE_FONT_COLLECTION_H
#define CERN_DRAWING_TEXT_PRIVATE_FONT_COLLECTION_H 1

#include <glib-object.h>
#include "font_collection.h"

G_BEGIN_DECLS

#define CERN_TYPE_PRIVATE_FONT_COLLECTION (cern_private_font_collection_get_type())

G_DECLARE_FINAL_TYPE(CernPrivateFontCollection,
                     cern_private_font_collection,
                     CERN, PRIVATE_FONT_COLLECTION, CernFontCollection);

CernFontCollection *
cern_private_font_collection_new(void);

void
cern_private_font_collection_add_font_file(CernPrivateFontCollection *self,
                                           gchar const *file_name);

void
cern_private_font_collection_add_font_memory(CernPrivateFontCollection *self,
                                             gpointer memory, gsize length);

void
cern_private_font_collection_unref(CernPrivateFontCollection *self);

G_END_DECLS

#endif /* CERN_DRAWING_TEXT_PRIVATE_FONT_COLLECTION_H */
