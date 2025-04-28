#include "cern/drawing/internal/windows_font.h"
#include "cern/core/icloneable.h"
#include "cern/drawing/font.h"
#include "cern/drawing/font_family.h"
#include "cern/drawing/font_style.h"
#include "cern/drawing/internal/device_context.h"
#include "cern/drawing/internal/device_context_map_mode.h"
#include "cern/drawing/internal/i_device_context.h"
#include "cern/drawing/internal/windows_font_quality.h"
#include "cern/drawing/internal/windows_graphics.h"
#include "glib-object.h"
#include "glib.h"

#include <math.h>
#include <string.h>
#include <windows.h>
#include <gdiplus.h>
#include <wingdi.h>
#include <winscard.h>

#define LOG_FONT_NAME_OFFSET  (28)
#define DEFAULT_FACE_NAME     "Microsoft Sans Serif"
#define DEFAULT_FONT_SIZE     (8.25f)
#define DEFAULT_FONT_HEIGHT   (13)
#define TEMP_DPI_VALUE        (96)

struct _CernWindowsFont {
  GObject parent_instance;
  gpointer handle;
  gfloat font_size;
  gint32 line_spacing;
  gboolean own_handle;
  gboolean owned_by_cache_manager;
  gboolean ever_owned_by_cache_mananger;
  LOGFONTA *log_font;
  CernFontStyle font_style;
};

G_DEFINE_FINAL_TYPE(CernWindowsFont, cern_windows_font, G_TYPE_OBJECT)

static
void
cern_windows_font_init(CernWindowsFont *self) {
  self->font_style = -1.0f;
  self->log_font = g_new(LOGFONTA, 1);
}

static
void
cern_windows_font_class_init(CernWindowsFontClass *klass) { (void) klass; }

static
void
cern_windows_font_create_font(CernWindowsFont *self) {
  g_assert(self->handle == NULL);
  g_assert(self->log_font != NULL);

  self->handle = (gpointer) CreateFontIndirectA(self->log_font);

  if (self->handle == NULL) {
    strncpy(self->log_font->lfFaceName, DEFAULT_FACE_NAME, strlen(DEFAULT_FACE_NAME));
    self->log_font->lfOutPrecision = OUT_TT_ONLY_PRECIS;
    self->handle = (gpointer) CreateFontIndirectA(self->log_font);
  }

  GetObjectA(self->handle, sizeof(LOGFONTA), self->log_font);
  self->own_handle = TRUE;
}

static
CernWindowsFont *
cern_windows_font_from_logfont(LOGFONTA *logfont, gboolean create_handle) {
  g_assert(logfont != NULL);

  CernWindowsFont *self
    = g_object_new(CERN_TYPE_WINDOWS_FONT, NULL);

  memcpy(logfont, self->log_font, sizeof(LOGFONTA));

  if (strlen(self->log_font->lfFaceName) == 0) {
    strncpy(self->log_font->lfFaceName, DEFAULT_FACE_NAME, strlen(DEFAULT_FACE_NAME));
  }

  self->font_style = CernFontStyle_Regular;
  if (logfont->lfWeight == FW_BOLD) {
    self->font_style |= CernFontStyle_Bold;
  }

  if (logfont->lfItalic == 1) {
    self->font_style |= CernFontStyle_Italic;
  }

  if (logfont->lfUnderline == 1) {
    self->font_style |= CernFontStyle_Underline;
  }

  if (logfont->lfStrikeOut == 1) {
    self->font_style |= CernFontStyle_Strikeout;
  }

  if (create_handle) {
    cern_windows_font_create_font(self);
  }

  return self;
}

CernWindowsFont *
cern_windows_font_new(gchar const *face_name) {
  return
    cern_windows_font_new_4(face_name, DEFAULT_FONT_SIZE,
                            CernFontStyle_Regular, DEFAULT_CHARSET,
                            CernWindowsFontQuality_Default);
}

CernWindowsFont *
cern_windows_font_new_2(gchar const *face_name, gfloat size) {
  return
    cern_windows_font_new_4(face_name, size,
                            CernFontStyle_Regular, DEFAULT_CHARSET,
                            CernWindowsFontQuality_Default);
}

CernWindowsFont *
cern_windows_font_new_3(gchar const *face_name, gfloat size, CernFontStyle style) {
  return
    cern_windows_font_new_4(face_name, size,
                            style, DEFAULT_CHARSET,
                            CernWindowsFontQuality_Default);
}

CernWindowsFont *
cern_windows_font_new_4(gchar const *face_name, gfloat size, CernFontStyle style,
                        guint8 char_set, CernWindowsFontQuality quality) {
  g_assert(size > 0);

  CernWindowsFont *self
    = g_object_new(CERN_TYPE_WINDOWS_FONT, NULL);

  gint32 pixels_y = ceilf(TEMP_DPI_VALUE * size / 72);

  LOGFONTA *lf = self->log_font;

  lf->lfHeight = -pixels_y;
  if (face_name != NULL) {
    strncpy(lf->lfFaceName, face_name, strlen(face_name));
  } else {
    strncpy(lf->lfFaceName, DEFAULT_FACE_NAME, strlen(DEFAULT_FACE_NAME));
  }

  lf->lfCharSet       = char_set;
  lf->lfOutPrecision  = OUT_TT_PRECIS;
  lf->lfQuality       = (BYTE) quality;
  lf->lfWeight        = (style & CernFontStyle_Bold) ? FW_BOLD : FW_NORMAL;
  lf->lfItalic        = (style & CernFontStyle_Italic) ? TRUE : FALSE;
  lf->lfUnderline     = (style & CernFontStyle_Underline) ? TRUE : FALSE;
  lf->lfStrikeOut     = (style & CernFontStyle_Strikeout) ? TRUE : FALSE;

  self->font_style = style;

  cern_windows_font_create_font(self);

  return self;
}

CernWindowsFont *
cern_windows_font_create_from_font(CernFont *font) {
  return 
    cern_windows_font_create_from_font_with_quality(font, CernWindowsFontQuality_Default);
}

CernWindowsFont *
cern_windows_font_create_from_font_with_quality(CernFont *font, CernWindowsFontQuality quality) {
  CernFontFamily *font_family
    = cern_font_get_family(font);

  gchar *family_name
    = cern_font_family_get_name(font_family);

  if (family_name != NULL
      && strlen(family_name) > 0
      && family_name[0] == '@') {
    family_name = &family_name[1];
  }

  return
    cern_windows_font_new_4(family_name,
                            cern_font_get_size_in_points(font),
                            cern_font_get_style(font),
                            cern_font_get_gdi_char_set(font),
                            quality);
}

CernWindowsFont *
cern_windows_font_create_from_hdc(gpointer hdc) {
  gpointer h_font
    = GetCurrentObject((HDC) hdc, OBJ_FONT);

  return
    cern_windows_font_create_from_h_font(h_font);
}

CernWindowsFont *
cern_windows_font_create_from_h_font(gpointer h_font) {
  return 
    cern_windows_font_create_from_h_font_ex(h_font, FALSE);
}

CernWindowsFont *
cern_windows_font_create_from_h_font_ex(gpointer h_font, gboolean take_ownership) {
  LOGFONT lf = { 0 };

  GetObject(h_font, sizeof(lf), &lf);
  
  CernWindowsFont *font = cern_windows_font_from_logfont(&lf, FALSE);
  
  font->handle = h_font;
  font->own_handle = take_ownership;

  return font;
}

gpointer
cern_windows_font_get_h_font(CernWindowsFont *font) {
  return font->handle;
}

gboolean
cern_windows_font_is_italic(CernWindowsFont *font) {
  return (font->font_style & CernFontStyle_Italic);
}

gboolean
cern_windows_font_is_owned_by_cache_manager(CernWindowsFont *font) {
  /* FIXME: Add cache mananger. */
  (void) font;
  return FALSE;
}

CernWindowsFontQuality
cern_windows_font_get_quality(CernWindowsFont *font) {
  return (CernWindowsFontQuality) font->log_font->lfQuality;
}

CernFontStyle
cern_windows_font_get_style(CernWindowsFont *font) {
  return font->font_style;
}

gint32
cern_windows_font_get_height(CernWindowsFont *font) {
  if (font->line_spacing == 0) {
    CernWindowsGraphics *wg
      = cern_windows_graphics_measurement();

    CernDeviceContext *device_context
      = cern_windows_graphics_get_device_context(wg);

    cern_device_context_select_font(device_context, font);
    TEXTMETRICA tm = { 0 };
    CernDeviceContextMapMode mode
      = cern_device_context_get_map_mode(device_context);

    gboolean setup_dc = mode != CernDeviceContextMapMode_Text;

    if (setup_dc) {
      cern_device_context_save_hdc(device_context);
    }

    if (setup_dc) {
      mode
        = cern_device_context_set_map_mode(device_context, CernDeviceContextMapMode_Text);
    }

    GetTextMetricsA(cern_device_context_get_hdc(device_context), &tm);

    if (setup_dc) {
      cern_device_context_restore_hdc(device_context);
    }

    font->line_spacing = tm.tmHeight;
  }

  return font->line_spacing;
}

guint8
cern_windows_font_get_char_set(CernWindowsFont *font) {
  return font->log_font->lfCharSet;
}

gint32
cern_windows_font_get_log_font_height(CernWindowsFont *font) {
  return font->log_font->lfHeight;
}

gchar *
cern_windows_font_get_name(CernWindowsFont *font) {
  return font->log_font->lfFaceName;
}

gfloat
cern_windows_font_get_size(CernWindowsFont *font) {
  if (font->font_size < 0.0f) {
    CernWindowsGraphics *wg
      = cern_windows_graphics_measurement();
    CernDeviceContext *device_context
      = cern_windows_graphics_get_device_context(wg);

    cern_device_context_select_font(device_context, font);

    TEXTMETRICA tm = { 0 };

    CernDeviceContextMapMode mode
      = cern_device_context_get_map_mode(device_context);

    gboolean setup_dc = mode != CernDeviceContextMapMode_Text;

    if (setup_dc) {
      cern_device_context_save_hdc(device_context);
    }

    if (setup_dc) {
      mode
        = cern_device_context_set_map_mode(device_context, CernDeviceContextMapMode_Text);
    }

    GetTextMetricsA(cern_device_context_get_hdc(device_context), &tm);

    if (setup_dc) {
      cern_device_context_restore_hdc(device_context);
    }

    gint32 height
      = font->log_font->lfHeight > 0
          ? tm.tmHeight
          : (tm.tmHeight - tm.tmInternalLeading);

    font->font_size
      = height * 72.0f / cern_device_context_get_dpi_y(device_context);
  }

  return font->font_size;
}

CernWindowsFontQuality
cern_windows_font_quality_from_text_rendering_hint(CernGraphics *graphics) {
  /* TODO: Add return */
  (void) graphics;
  return 0;
}

gboolean
cern_windows_font_equals(CernWindowsFont *self, CernWindowsFont *other) {
  return
    (strcmp(cern_windows_font_get_name(self), cern_windows_font_get_name(other)) == 0)
      && cern_windows_font_get_log_font_height(self) == cern_windows_font_get_log_font_height(other)
      && cern_windows_font_get_style(self) == cern_windows_font_get_style(other)
      && cern_windows_font_get_char_set(self) == cern_windows_font_get_char_set(other)
      && cern_windows_font_get_quality(self) == cern_windows_font_get_quality(other);
}
