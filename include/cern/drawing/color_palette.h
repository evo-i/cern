#ifndef CERN_COLOR_PALETTE_H
#define CERN_COLOR_PALETTE_H 1

#include <glib-object.h>

G_BEGIN_DECLS

#define CERN_TYPE_COLOR_PALETTE (cern_color_palette_get_type())

G_DECLARE_FINAL_TYPE(CernColorPalette, cern_color_palette, CERN, COLOR_PALETTE, GObject)

CernColorPalette *
cern_color_palette_new(void);

CernColorPalette *
cern_color_palette_new_with_count(gint32 count);

gint32
cern_color_palette_get_flags(CernColorPalette *self);

GList *
cern_color_palette_get_entries(CernColorPalette *self);

void
cern_color_palette_read_from_memry(CernColorPalette *self, gpointer memory);

gpointer
cern_color_palette_write_to_memory(CernColorPalette *self);

G_END_DECLS

#endif /* CERN_COLOR_PALETTE_H */
