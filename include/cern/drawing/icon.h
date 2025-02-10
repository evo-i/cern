#ifndef ICON
#define ICON
#ifndef CERN_DRAWING_ICON_H
#define CERN_DRAWING_ICON_H 1

#include <glib-object.h>

#include "size.h"
#include "bitmap.h"

G_BEGIN_DECLS

#define CERN_TYPE_ICON (cern_icon_get_type())

G_DECLARE_FINAL_TYPE(CernIcon, cern_icon, CERN, ICON, GObject)

CernIcon *
cern_icon_new_from_path(gchar const *path);

CernIcon *
cern_icon_new_from_path_with_size(gchar const *path, CernSize *size);

CernIcon *
cern_icon_new_from_path_with_wh(gchar const *path,
                                guint32 width,
                                guint32 height);

CernIcon *
cern_icon_new_from_icon_with_size(CernIcon *original, CernSize *size);

CernIcon *
cern_icon_new_from_icon_and_wh(CernIcon *original,
                               guint32 width,
                               guint32 height);

CernIcon *
cern_icon_extract_associated_icon(gchar const *path);

CernIcon *
cern_icon_extract_associated_icon_with_index(gchar const *path, gint32 index);

gpointer
cern_icon_get_handle(CernIcon *self);

guint32
cern_icon_get_width(CernIcon *self);

guint32
cern_icon_get_height(CernIcon *self);

CernSize
cern_icon_get_size(CernIcon *self);

CernIcon *
cern_icon_clone(CernIcon *self);

CernBitmap *
cern_icon_to_bitmap(CernIcon *self);

CernIcon *
cern_icon_new_from_h_icon(gpointer h_icon);

void
cern_icon_unref(CernIcon *self);

G_END_DECLS

#endif /* CERN_DRAWING_ICON_H */


#endif /* ICON */
