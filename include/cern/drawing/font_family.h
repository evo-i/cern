#ifndef CERN_DRAWING_FONT_FAMILY_H
#define CERN_DRAWING_FONT_FAMILY_H 1

#include <glib-object.h>
#include "font_style.h"
#include "text/font_collection.h"
#include "text/generic_font_families.h"

G_BEGIN_DECLS

#define CERN_TYPE_FONT_FAMILY (cern_font_family_get_type())

G_DECLARE_FINAL_TYPE(CernFontFamily, cern_font_family, CERN, FONT_FAMILY, GObject);

CernFontFamily *
cern_font_family_new(gchar const *name);

CernFontFamily *
cern_font_family_new_with_collection(gchar const *name,
                                     CernFontCollection *collection);

CernFontFamily *
cern_font_family_new_generic(CernGenericFontFamilies generic);

CernFontFamily *
cern_font_family_new_internal(gpointer handle);

gchar *
cern_font_family_get_name(CernFontFamily *self);

GPtrArray *
cern_font_family_get_families(void);

CernFontFamily *
cern_font_family_get_generic_sans_serif(void);

CernFontFamily *
cern_font_family_get_generic_serif(void);

CernFontFamily *
cern_font_family_get_generic_monospace(void);

gboolean
cern_font_family_is_style_available(CernFontFamily *self, CernFontStyle style);

gint32
cern_font_family_get_em_height(CernFontFamily *self, CernFontStyle style);

gint32
cern_font_family_get_cell_ascent(CernFontFamily *self, CernFontStyle style);

gint32
cern_font_family_get_cell_descent(CernFontFamily *self, CernFontStyle style);

gint32
cern_font_family_get_line_spacing(CernFontFamily *self, CernFontStyle style);

gboolean
cern_font_family_is_equals(CernFontFamily *self, CernFontFamily *other);

void
cern_font_family_unref(CernFontFamily *self);

G_END_DECLS

#endif /* CERN_DRAWING_FONT_FAMILY_H */
