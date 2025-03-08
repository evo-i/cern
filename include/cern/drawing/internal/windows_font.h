#ifndef WINDOWS_FONT
#define WINDOWS_FONT

#include <glib-object.h>
#include "../font_style.h"
#include "windows_font_quality.h"

G_BEGIN_DECLS

typedef struct _CernFont CernFont;
typedef struct _CernGraphics CernGraphics;

#define CERN_TYPE_WINDOWS_FONT cern_windows_font_get_type()

G_DECLARE_FINAL_TYPE (CernWindowsFont, cern_windows_font, CERN, WINDOWS_FONT, GObject)

CernWindowsFont *
cern_windows_font_new(gchar const *face_name);

CernWindowsFont *
cern_windows_font_new_2(gchar const *face_name, gfloat size);

CernWindowsFont *
cern_windows_font_new_3(gchar const *face_name, gfloat size, CernFontStyle style);

CernWindowsFont *
cern_windows_font_new_4(gchar const *face_name, gfloat size, CernFontStyle style,
                        guint8 char_set, CernWindowsFontQuality quality);

CernWindowsFont *
cern_windows_font_create_from_font(CernFont *font);

CernWindowsFont *
cern_windows_font_create_from_font_with_quality(CernFont *font, CernWindowsFontQuality quality);

CernWindowsFont *
cern_windows_font_create_from_hdc(gpointer hdc);

CernWindowsFont *
cern_windows_font_create_from_h_font(gpointer h_font);

gpointer
cern_windows_font_get_h_font(CernWindowsFont *font);

gboolean
cern_windows_font_is_italic(CernWindowsFont *font);

gboolean
cern_windows_font_is_owned_by_cache_manager(CernWindowsFont *font);

CernWindowsFontQuality
cern_windows_font_get_quality(CernWindowsFont *font);

CernFontStyle
cern_windows_font_get_style(CernWindowsFont *font);

gint32
cern_windows_font_get_height(CernWindowsFont *font);

guint8
cern_windows_font_get_char_set(CernWindowsFont *font);

gint32
cern_windows_font_get_log_font_height(CernWindowsFont *font);

gchar *
cern_windows_font_get_name(CernWindowsFont *font);

gfloat
cern_windows_font_get_size(CernWindowsFont *font);

CernWindowsFontQuality
cern_windows_font_quality_from_text_rendering_hint(CernGraphics *graphics);


G_END_DECLS

#endif /* WINDOWS_FONT */
