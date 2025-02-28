#include "cern/drawing/font.h"
#include "cern/drawing/graphics.h"
#include "cern/core/icloneable.h"
#include "cern/core/windows/handle.h"
#include "cern/drawing/internal/native_gdi_object.h"

#include <Windows.h>
#include <gdiplus.h>
#include <math.h>

struct _CernFont {
  GObject parent_instance;

  gpointer handle;
  gfloat size;
  CernFontStyle style;
  CernFontFamily *family;
  CernGraphicsUnit unit;
  guint8 gdi_char_set;
  gboolean gdi_vertical_font;
  gchar *system_font_name;
  gchar *original_name;
};

extern
CernFontFamily *
cern_font_family_new_internal_2(gchar const*, gboolean);

static
void
cern_font_iface_init_icloneable(CernICloneableInterface *iface) {

}

static
void
cern_font_iface_init_handle(CernHandleInterface *iface) {

}

G_DEFINE_FINAL_TYPE_WITH_CODE(CernFont, cern_font, G_TYPE_OBJECT,
  G_IMPLEMENT_INTERFACE(CERN_TYPE_ICLONEABLE, cern_font_iface_init_icloneable)
  G_IMPLEMENT_INTERFACE(CERN_TYPE_HANDLE, cern_font_iface_init_handle));

static
void
cern_font_init(CernFont *self) {

}

static
void
cern_font_class_init(CernFontClass *klass) {

}

static
CernFont *
cern_font_new_from_native(gpointer native_font,
                         guint8 gdi_char_set, gboolean gdi_vertical_font) {
  GpStatus status;
  gfloat size;
  Unit unit;
  CernFontStyle style;
  gpointer native_family = NULL;
  CernFontFamily *family;
  CernFont *self;

  g_return_val_if_fail(native_font != NULL, NULL);

  self = g_object_new(CERN_TYPE_FONT, NULL);
  self->handle = native_font;

  status = GdipGetFontUnit(native_font, &unit);
  if (status != Ok) {
    g_critical("%s(...): Failed to get font unit: %d", __func__, status);
    return NULL;
  }

  status = GdipGetFontSize(native_font, &size);
  if (status != Ok) {
    g_critical("%s(...): Failed to get font size: %d", __func__, status);
    return NULL;
  }

  status = GdipGetFontStyle(native_font, (INT *) &style);
  if (status != Ok) {
    g_critical("%s(...): Failed to get font style: %d", __func__, status);
    return NULL;
  }

  status = GdipGetFamily(native_font, &native_family);
  if (status != Ok) {
    g_critical("%s(...): Failed to get font family: %d", __func__, status);
    return NULL;
  }

  family = cern_font_family_new_internal_2(native_family, TRUE);
  if (!family) {
    return NULL;
  }

  self->family = family;
  self->size = size;
  self->style = style;
  self->unit = unit;
  self->gdi_char_set = gdi_char_set;
  self->gdi_vertical_font = gdi_vertical_font;

  return self;
}

static
gchar *
cern_font_strip_vertical_name(const gchar *family_name) {
  if (family_name != NULL
      && g_utf8_strlen(family_name, -1) > 1
      && family_name[0] == '@') {
    return g_utf8_substring(family_name, 1, -1);
  }
  return g_strdup(family_name);
}


static
gboolean
cern_font_is_vertical_name(const gchar *family_name) {
  return
    family_name != NULL
      && g_utf8_strlen(family_name, 0) > 0
      && family_name[0] == '@';
}


static
gboolean
cern_font_create_native(CernFont *self) {
  CernHandle *font_family_handle;
  gpointer font_family_ptr;
  GpStatus status;

  g_return_val_if_fail(self->handle == NULL, FALSE);
  g_return_val_if_fail(self->family != NULL, FALSE);

  font_family_handle = CERN_HANDLE(self->family);
  font_family_ptr = cern_handle_get(font_family_handle);

  status
    = GdipCreateFont(font_family_handle, self->size, self->style,
                     self->unit, &self->handle);

  if (status == FontStyleNotFound) {
    g_critical("%s(...): Font style not found for family '%s' with style %d",
      __func__, cern_font_family_get_name(self->family), self->style);
    return FALSE;
  }
  else if (status != Ok) {
    g_critical("%s(...): Failed to create font: %d", __func__, status);
    return FALSE;
  }

  return TRUE;
}


static
gboolean
cern_font_initialize(CernFont *self, gchar *family_name, float em_size,
                     CernFontStyle style, CernGraphicsUnit unit,
                     guint8 gdi_char_set, gboolean gdi_vertical_font) {
  CernFontFamily *family;
  gchar *stripped_name;

  if (family_name == NULL) {
    g_critical("%s(...): Failed: family_name must be not NULL.", __func__);
    return FALSE;
  }

  self->original_name = g_strdup(family_name);
  stripped_name = cern_font_strip_vertical_name(family_name);

  family = cern_font_family_new_internal_2(stripped_name, TRUE);
  g_free(stripped_name);

  if (family == NULL) {
    g_critical("%s(...): Failed to create font family.", __func__);
    return FALSE;
  }

  return
    cern_font_initialize(self, family_name, em_size, style, unit,
                        gdi_char_set, gdi_vertical_font);

}

static
gboolean
cern_font_initialize_2(CernFont *self, CernFontFamily *family, float em_size,
                       CernFontStyle style, CernGraphicsUnit unit,
                       guint8 gdi_char_set, gboolean gdi_vertical_font) {
  GpStatus status;
  gfloat actual_size;

  if (family == NULL) {
    g_critical("%s(...): Failed: family must be not NULL.", __func__);
    return FALSE;
  }

  if (isnan(em_size) || isinf(em_size) || em_size <= 0.0f) {
    g_critical("%s(...): Failed: em_size cannot be NAN or "
               "INFINITY or less than 0.0", __func__);
    return FALSE;
  }

  self->size = em_size;
  self->style = style;
  self->unit = unit;
  self->gdi_char_set = gdi_char_set;
  self->gdi_vertical_font = gdi_vertical_font;

  if (self->family == NULL) {
    self->family = family;
  }

  if (!self->handle) {
    if (!cern_font_create_native(self)) {
      return FALSE;
    }
  }

  status = GdipGetFontSize(self->handle, &actual_size);

  if (status != Ok) {
    g_critical("%s(...): Failed to get font size: %d", __func__, status);
    return FALSE;
  }

  self->size = actual_size;

  return TRUE;
}

CernFont *
cern_font_new(CernFont *prototype, CernFontStyle new_style) {
  CernFont *self;

  if (prototype == NULL) {
    g_critical("%s(...): Failed: prototype must be not NULL.", __func__);
    return NULL;
  }

  self = g_object_new(CERN_TYPE_FONT, NULL);

  self->original_name = g_strdup(prototype->original_name);

  if (!cern_font_initialize_2(self, prototype->family, prototype->size,
                              new_style, prototype->unit,
                              DEFAULT_CHARSET, FALSE)) {
    cern_font_unref(self);
    return NULL;
  }

  return self;
}

CernFont *
cern_font_new_2(CernFontFamily *family, gfloat em_size, CernFontStyle style,
               CernGraphicsUnit unit) {
  CernFont *self;
  gboolean status;

  self = g_object_new(CERN_TYPE_FONT, NULL);
  status
    = cern_font_initialize_2(self, family, em_size, style, unit,
                             DEFAULT_CHARSET, FALSE);

  if (!status) {
    cern_font_unref(self);
    return NULL;
  }

  return self;
}

CernFont *
cern_font_new_3(CernFontFamily *family, gfloat em_size, CernFontStyle style,
                CernGraphicsUnit unit, guint8 gdi_char_set) {
  CernFont *self;
  gboolean status;

  self = g_object_new(CERN_TYPE_FONT, NULL);
  status = cern_font_initialize_2(self, family, em_size, style, unit,
                                  gdi_char_set, FALSE);

  if (!status) {
    cern_font_unref(self);
    return NULL;
  }

  return self;
}

CernFont *
cern_font_new_4(CernFontFamily *family, gfloat em_size, CernFontStyle style,
                CernGraphicsUnit unit, guint8 gdi_char_set,
                gboolean gdi_vertical_font) {
  CernFont *self;
  gboolean status;

  self = g_object_new(CERN_TYPE_FONT, NULL);
  status = cern_font_initialize_2(self, family, em_size, style, unit,
                                  gdi_char_set, gdi_vertical_font);

  if (!status) {
    cern_font_unref(self);
    return NULL;
  }

  return self;
}

CernFont *
cern_font_new_5(gchar *family_name, gfloat em_size, CernFontStyle style,
                CernGraphicsUnit unit, guint8 gdi_char_set) {
  CernFont *self;
  gboolean status;

  self = g_object_new(CERN_TYPE_FONT, NULL);
  status = cern_font_initialize(self, family_name, em_size, style, unit,
                              gdi_char_set, cern_font_is_vertical_name(family_name));

  if (!status) {
    cern_font_unref(self);
    return NULL;
  }

  return self;
}

CernFont *
cern_font_new_6(gchar *family_name, gfloat em_size, CernFontStyle style,
                CernGraphicsUnit unit, guint8 gdi_char_set,
                gboolean gdi_vertical_font) {
  CernFont *self;
  gboolean status;

  if (isnan(em_size) || isinf(em_size) || em_size <= 0) {
    g_warning("Invalid em_size value: %f", em_size);
    return NULL;
  }

  self = g_object_new(CERN_TYPE_FONT, NULL);
  status = cern_font_initialize(self, family_name, em_size, style, unit,
                                gdi_char_set, gdi_vertical_font);

  if (!status) {
    cern_font_unref(self);
    return NULL;
  }

  return self;
}

CernFont *
cern_font_new_7(CernFontFamily *family, gfloat em_size, CernFontStyle style) {
  CernFont *self;
  gboolean status;

  if (isnan(em_size) || isinf(em_size) || em_size <= 0) {
    g_warning("Invalid em_size value: %f", em_size);
    return NULL;
  }

  self = g_object_new(CERN_TYPE_FONT, NULL);
  status
    = cern_font_initialize_2(self, family, em_size, style,
                             CernGraphicsUnit_Point, DEFAULT_CHARSET, FALSE);

  if (!status) {
    cern_font_unref(self);
    return NULL;
  }

  return self;
}

CernFont *
cern_font_new_8(CernFontFamily *family, gfloat em_size, CernGraphicsUnit unit) {
  CernFont *self;
  gboolean status;
  gchar *family_name;

  if (isnan(em_size) || isinf(em_size) || em_size <= 0) {
    g_warning("Invalid em_size value: %f", em_size);
    return NULL;
  }

  self = g_object_new(CERN_TYPE_FONT, NULL);
  status
    = cern_font_initialize_2(self, family, em_size, CernFontStyle_Regular,
                             unit, DEFAULT_CHARSET, FALSE);

  if (!status) {
    cern_font_unref(self);
    return NULL;
  }

  return self;
}

CernFont *
cern_font_new_9(CernFontFamily *family, gfloat em_size) {
  CernFont *self;
  gboolean status;

  if (isnan(em_size) || isinf(em_size) || em_size <= 0) {
    g_warning("Invalid em_size value: %f", em_size);
    return NULL;
  }

  self = g_object_new(CERN_TYPE_FONT, NULL);
  status
    = cern_font_initialize_2(self, family, em_size, CernFontStyle_Regular,
                             CernGraphicsUnit_Point, DEFAULT_CHARSET, FALSE);

  if (!status) {
    cern_font_unref(self);
    return NULL;
  }

  return self;
}

CernFont *
cern_font_new_10(gchar *family_name, gfloat em_size, CernFontStyle style,
                 CernGraphicsUnit unit) {
  CernFont *self;
  gboolean status;
  CernFontFamily *family;

  if (isnan(em_size) || isinf(em_size) || em_size <= 0) {
    g_warning("Invalid em_size value: %f", em_size);
    return NULL;
  }

  self = g_object_new(CERN_TYPE_FONT, NULL);
  family = cern_font_family_new(family_name);
  status
    = cern_font_initialize_2(self, family, em_size, style,
                             unit, DEFAULT_CHARSET,
                             cern_font_is_vertical_name(family_name));

  if (!status) {
    cern_font_unref(self);
    return NULL;
  }

  return self;
}

CernFont *
cern_font_new_11(gchar *family_name, gfloat em_size, CernFontStyle style) {
  CernFont *self;
  gboolean status;

  if (isnan(em_size) || isinf(em_size) || em_size <= 0) {
    g_warning("Invalid em_size value: %f", em_size);
    return NULL;
  }

  self = g_object_new(CERN_TYPE_FONT, NULL);
  status
    = cern_font_initialize(self, family_name, em_size, style,
                           CernGraphicsUnit_Point, DEFAULT_CHARSET,
                           cern_font_is_vertical_name(family_name));

  if (!status) {
    cern_font_unref(self);
    return NULL;
  }

  return self;
}

CernFont *
cern_font_new_12(gchar *family_name, gfloat em_size, CernGraphicsUnit unit) {
  CernFont *self;
  gboolean status;

  if (isnan(em_size) || isinf(em_size) || em_size <= 0) {
    g_warning("Invalid em_size value: %f", em_size);
    return NULL;
  }

  self = g_object_new(CERN_TYPE_FONT, NULL);
  status
    = cern_font_initialize(self, family_name, em_size, CernFontStyle_Regular,
                            unit, DEFAULT_CHARSET,
                            cern_font_is_vertical_name(family_name));

  if (!status) {
    cern_font_unref(self);
    return NULL;
  }

  return self;
}

CernFont *
cern_font_new_13(gchar *family_name, gfloat em_size) {
  CernFont *self;
  gboolean status;

  if (isnan(em_size) || isinf(em_size) || em_size <= 0) {
    g_warning("Invalid em_size value: %f", em_size);
    return NULL;
  }

  self = g_object_new(CERN_TYPE_FONT, NULL);
  status
    = cern_font_initialize(self, family_name, em_size, CernFontStyle_Regular,
                          CernGraphicsUnit_Point, DEFAULT_CHARSET,
                          cern_font_is_vertical_name(family_name));

  if (!status) {
    cern_font_unref(self);
    return NULL;
  }

  return self;
}

CernFontFamily *
cern_font_get_family(CernFont *self) {
  if (self->family != NULL) {
    return g_object_ref(self->family);
  }

  return NULL;
}

gboolean
cern_font_is_bold(CernFont *self) {
  return self->style & CernFontStyle_Bold;
}

guint8
cern_font_get_gdi_char_set(CernFont *self) {
  return self->gdi_char_set;
}

gboolean
cern_font_is_gdi_vertical_font(CernFont *self) {
  return self->gdi_vertical_font;
}

gboolean
cern_font_is_italic(CernFont *self) {
  return self->style & CernFontStyle_Italic;
}

gchar *
cern_font_get_name(CernFont *self) {
  return
    cern_font_family_get_name(self->family);
}

gchar *
cern_font_get_original_name(CernFont *self) {
  return g_strdup(self->original_name);
}

gboolean
cern_font_is_strikeout(CernFont *self) {
  return self->style & CernFontStyle_Strikeout;
}

gboolean
cern_font_is_underline(CernFont *self) {
  return self->style & CernFontStyle_Underline;
}

gfloat
cern_font_get_height(CernFont *self) {
  HDC screen_dc;
  gfloat height = 0.0f;
  CernGraphics *graphics;

  screen_dc = GetDC(NULL);
  if (screen_dc) {
    graphics = cern_graphics_new_from_hdc(screen_dc);
    if (graphics) {
      height = cern_font_get_height_from_graphics(self, graphics);
      g_object_unref(graphics);
    }
    ReleaseDC(NULL, screen_dc);
  }

  return height;
}

gfloat
cern_font_get_height_from_graphics(CernFont *self, CernGraphics *graphics) {
  gfloat height = 0.0f;
  GpStatus status;
  CernNativeGdiObject *native_gfx;
  gpointer native_gfx_handle;

  native_gfx = CERN_NATIVE_GDI_OBJECT(graphics);
  native_gfx_handle = cern_native_gdi_object_get_native_handle(native_gfx);

  if (!graphics)
    return 0.0f;

  status
    = GdipGetFontHeight(self->handle, native_gfx_handle, &height);

  if (status != Ok) {
    return 0.0f;
  }

  return height;
}

gfloat
cern_font_get_height_for_dpi(CernFont *self, gfloat dpi) {
  GpStatus status;

  gfloat height = 0.0f;

  status = GdipGetFontHeightGivenDPI(self->handle, dpi, &height);

  if (status != Ok) {
    return 0.0f;
  }

  return height;
}

CernFontStyle
cern_font_get_style(CernFont *self) {
  return self->style;
}

gfloat
cern_font_get_size(CernFont *self) {
  return self->size;
}

gfloat
cern_font_get_size_in_points(CernFont *self) {
  if (self->unit == CernGraphicsUnit_Point) {
    return self->size;
  } else {
    gfloat em_height_in_points = 0.0f;
    HDC screen_dc = GetDC(NULL);

    if (screen_dc) {
      CernGraphics *graphics = cern_graphics_new_from_hdc(screen_dc);
      if (graphics) {
        gfloat pixels_per_point = cern_graphics_get_dpy_y(graphics) / 72.0f;
        gfloat line_spacing_in_pixels
          = cern_font_get_height_from_graphics(self, graphics);
        gfloat em_height_in_pixels
          = line_spacing_in_pixels
              * cern_font_family_get_em_height(self->family, self->style)
              / cern_font_family_get_line_spacing(self->family, self->style);

        em_height_in_points = em_height_in_pixels / pixels_per_point;

        g_object_unref(graphics);
      }
      ReleaseDC(NULL, screen_dc);
    }

    return em_height_in_points;
  }
}

CernGraphicsUnit
cern_font_get_unit(CernFont *self) {
  return self->unit;
}

gint32
cern_font_get_height_i(CernFont *self) {
  return (gint32) ceilf(cern_font_get_height(self));
}

gboolean
cern_font_is_system_font(CernFont *self) {
  return
    self->system_font_name != NULL
      || g_utf8_strlen(self->system_font_name, 0) > 0;
}

gchar *
cern_font_get_system_font_name(CernFont *self) {
  return g_strdup(self->system_font_name);
}

gboolean
cern_font_is_equals(CernFont *self, CernFont *other) {
  if (self == other) {
    return TRUE;
  }

  if (!other) {
    return FALSE;
  }

  return
    cern_font_family_is_equals(self->family, other->family)
      && self->gdi_vertical_font == other->gdi_vertical_font
      && self->gdi_char_set == other->gdi_char_set
      && self->style == other->style
      && self->size == other->size
      && self->unit == other->unit;
}

CernFont *
cenr_font_clone(CernFont *self) {
  GpStatus status;
  CernFont *clone;
  gpointer clone_handle;

  status = GdipCloneFont(self->handle, &clone_handle);

  if (status != Ok) {
    return NULL;
  }

  return
    cern_font_new_from_native(clone_handle, self->gdi_char_set,
                              self->gdi_vertical_font);
}

CernFont *
cern_font_ref(CernFont *self) {
  return CERN_FONT(g_object_ref(self));
}

void
cern_font_unref(CernFont *self) {
  g_object_unref(self);
}
