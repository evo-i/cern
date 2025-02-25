#ifndef CERN_DRAWING_TEXT_INSTALLED_FONT_COLLECTION_H
#define CERN_DRAWING_TEXT_INSTALLED_FONT_COLLECTION_H 1

#include <glib-object.h>
#include "font_collection.h"

G_BEGIN_DECLS

#define CERN_TYPE_INSTALLED_FONT_COLLECTION (cern_installed_font_collection_get_type())

G_DECLARE_FINAL_TYPE(CernInstalledFontCollection,
  cern_installed_font_collection,
  CERN,
  INSTALLED_FONT_COLLECTION, CernFontCollection);

CernFontCollection *
cern_installed_font_collection_new(void);

G_END_DECLS

#endif /* CERN_DRAWING_TEXT_INSTALLED_FONT_COLLECTION_H */
