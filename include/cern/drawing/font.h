#ifndef CERN_DRAWING_FONT_H
#define CERN_DRAWING_FONT_H 1

#include <glib-object.h>

#include "font_style.h"
#include "font_family.h"
#include "graphics_unit.h"

G_BEGIN_DECLS

typedef struct _CernGraphics CernGraphics;

#define CERN_TYPE_FONT (cern_font_get_type())

G_DECLARE_FINAL_TYPE(CernFont, cern_font, CERN, FONT, GObject);

CernFont *
cern_font_new(CernFont *prototype, CernFontStyle new_style);

CernFont *
cern_font_new_2(CernFontFamily *family, gfloat em_size, CernFontStyle style,
               CernGraphicsUnit unit);

CernFont *
cern_font_new_3(CernFontFamily *family, gfloat em_size, CernFontStyle style,
                CernGraphicsUnit unit, guint8 gdi_char_set);

CernFont *
cern_font_new_4(CernFontFamily *family, gfloat em_size, CernFontStyle style,
                CernGraphicsUnit unit, guint8 gdi_char_set,
                gboolean gdi_vertical_font);

CernFont *
cern_font_new_5(gchar *family_name, gfloat em_size, CernFontStyle style,
                CernGraphicsUnit unit, guint8 gdi_char_set);

CernFont *
cern_font_new_6(gchar *family_name, gfloat em_size, CernFontStyle style,
                CernGraphicsUnit unit, guint8 gdi_char_set,
                gboolean gdi_vertical_font);

CernFont *
cern_font_new_7(CernFontFamily *family, gfloat em_size, CernFontStyle style);

CernFont *
cern_font_new_8(CernFontFamily *family, gfloat em_size, CernGraphicsUnit unit);

CernFont *
cern_font_new_9(CernFontFamily *family, gfloat em_size);

CernFont *
cern_font_new_10(gchar *family_name, gfloat em_size, CernFontStyle style,
                 CernGraphicsUnit unit);

CernFont *
cern_font_new_11(gchar *family_name, gfloat em_size, CernFontStyle style);

CernFont *
cern_font_new_12(gchar *family_name, gfloat em_size, CernGraphicsUnit unit);

CernFont *
cern_font_new_13(gchar *family_name, gfloat em_size);

CernFontFamily *
cern_font_get_family(CernFont *self);

gboolean
cern_font_is_bold(CernFont *self);

guint8
cern_font_get_gdi_char_set(CernFont *self);

gboolean
cern_font_is_gdi_vertical_font(CernFont *self);

gboolean
cern_font_is_italic(CernFont *self);

gchar *
cern_font_get_name(CernFont *self);

gchar *
cern_font_get_original_name(CernFont *self);

gboolean
cern_font_is_strikeout(CernFont *self);

gboolean
cern_font_is_underline(CernFont *self);

gfloat
cern_font_get_height(CernFont *self);

gfloat
cern_font_get_height_from_graphics(CernFont *self, CernGraphics *graphics);

gfloat
cern_font_get_height_for_dpi(CernFont *self, gfloat dpi);

CernFontStyle
cern_font_get_style(CernFont *self);

gfloat
cern_font_get_size(CernFont *self);

gfloat
cern_font_get_size_in_points(CernFont *self);

CernGraphicsUnit
cern_font_get_unit(CernFont *self);

gint32
cern_font_get_height_i(CernFont *self);

gboolean
cern_font_is_system_font(CernFont *self);

gchar *
cern_font_get_system_font_name(CernFont *self);

gboolean
cern_font_is_equals(CernFont *self, CernFont *other);

CernFont *
cern_font_clone(CernFont *self);

CernFont *
cern_font_ref(CernFont *self);

void
cern_font_unref(CernFont *self);

G_END_DECLS

#endif /* CERN_DRAWING_FONT_H */
