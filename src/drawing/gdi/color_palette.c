#include "cern/drawing/color_palette.h"
#include "cern/drawing/color.h"
#include <windows.h>
#include <gdiplus.h>

struct _CernColorPalette {
  GObject parent_instance;
  ColorPalette *palette;
};

G_DEFINE_TYPE(CernColorPalette, cern_color_palette, G_TYPE_OBJECT)

static
void
cern_color_palette_init(CernColorPalette *self) { }

void
cern_color_palette_finalize(GObject *object) {
  CernColorPalette *self = CERN_COLOR_PALETTE(object);
  g_free(self->palette);
  G_OBJECT_CLASS(cern_color_palette_parent_class)->finalize(object);
}

static
void
cern_color_palette_class_init(CernColorPaletteClass *klass) {
  GObjectClass *object_class = G_OBJECT_CLASS(klass);
  object_class->finalize = cern_color_palette_finalize;
}

CernColorPalette *
cern_color_palette_new(void) {
  CernColorPalette *self;

  self = g_object_new(CERN_TYPE_COLOR_PALETTE, NULL);
  self->palette = g_malloc0(sizeof(ColorPalette));

  self->palette->Flags = 0;
  self->palette->Count = 0;

  return self;
}

CernColorPalette *
cern_color_palette_new_with_count(gint32 count) {
  CernColorPalette *self;
  ColorPalette *palette;
  self = g_object_new(CERN_TYPE_COLOR_PALETTE, NULL);
  palette = g_malloc0(sizeof(ColorPalette) + sizeof(palette->Entries[0]) * count);
  palette->Flags = 0;
  palette->Count = count;
  self->palette = palette;
  return self;
}

gint32
cern_color_palette_get_flags(CernColorPalette *self) {
  g_return_val_if_fail(CERN_IS_COLOR_PALETTE(self), 0);
  return self->palette->Flags;
}

GList *
cern_color_palette_get_entries(CernColorPalette *self) {
  GList *entries = NULL;
  gint32 i;

  g_return_val_if_fail(CERN_IS_COLOR_PALETTE(self), NULL);

  for (i = 0; i < self->palette->Count; i++) {
    entries = g_list_append(entries, &self->palette->Entries[i]);
  }

  return entries;
}

void
cern_color_palette_read_from_memry(CernColorPalette *self, gpointer memory) {
  g_return_if_fail(CERN_IS_COLOR_PALETTE(self));
  g_return_if_fail(memory != NULL);
  gint32 count = ((ColorPalette *)memory)->Count;
  g_free(self->palette);
  self->palette = g_malloc0(sizeof(ColorPalette) + sizeof(self->palette->Entries[0]) * count);
  memcpy(self->palette, memory, sizeof(ColorPalette) + sizeof(self->palette->Entries[0]) * count);
}

gpointer
cern_color_palette_write_to_memory(CernColorPalette *self) {
  g_return_val_if_fail(CERN_IS_COLOR_PALETTE(self), NULL);
  gint32 count = self->palette->Count;
  gpointer memory = g_malloc0(sizeof(ColorPalette) + sizeof(self->palette->Entries[0]) * count);
  memcpy(memory, self->palette, sizeof(ColorPalette) + sizeof(self->palette->Entries[0]) * count);
  return memory;
}
