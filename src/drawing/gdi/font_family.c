#include "cern/drawing/font_family.h"
#include "cern/core/windows/handle.h"

#include <Windows.h>
#include <gdiplus.h>

#define LANG_NEUTRAL 0x00

struct _CernFontFamily {
  GObject parent;
  gpointer handle;
  gboolean create_default_on_fail;
};

static
gpointer
cern_font_family_get_handle(gpointer obj) {
  CernFontFamily *self;
  self = CERN_FONT_FAMILY(obj);
  return self->handle;
}

static
void
cern_font_family_set_handle(gpointer obj, gpointer handle) {
  CernFontFamily *self;
  self = CERN_FONT_FAMILY(obj);
  self->handle = handle;
}

static
void
cern_font_family_handle_interface_init(CernHandleInterface *iface) {
  iface->get = cern_font_family_get_handle;
  iface->set = cern_font_family_set_handle;
}

G_DEFINE_FINAL_TYPE_WITH_CODE(CernFontFamily, cern_font_family, G_TYPE_OBJECT,
  G_IMPLEMENT_INTERFACE(CERN_TYPE_HANDLE,
                        cern_font_family_handle_interface_init))

static
void
cern_font_family_finalize(GObject *obj) {
  CernFontFamily *self;
  self = CERN_FONT_FAMILY(obj);
  if (self->handle) {
    GdipDeleteFontFamily(self->handle);
    self->handle = NULL;
  }
  G_OBJECT_CLASS(cern_font_family_parent_class)->finalize(obj);
}

static
void
cern_font_family_class_init(CernFontFamilyClass *klass) {
  GObjectClass *object_class;
  object_class = G_OBJECT_CLASS(klass);
  object_class->finalize = cern_font_family_finalize;
}

static
void
cern_font_family_init(CernFontFamily *self) {
  self->handle = NULL;
  self->create_default_on_fail = TRUE;
}

CernFontFamily *
cern_font_family_new_internal_2(gchar const *name,
                                gboolean create_default_on_fail) {
  CernFontFamily *self;
  GpStatus status;
  gpointer handle = NULL;
  wchar_t *wname;

  self = g_object_new(CERN_TYPE_FONT_FAMILY, NULL);
  self->create_default_on_fail = create_default_on_fail;

  if (name != NULL) {
    wname = (wchar_t *)g_utf8_to_utf16(name, -1, NULL, NULL, NULL);
    status = GdipCreateFontFamilyFromName(wname, NULL, &handle);
    g_free(wname);

    if (status != Ok) {
      if (create_default_on_fail) {
        status = GdipGetGenericFontFamilySansSerif(&handle);
        if (status != Ok) {
          g_object_unref(self);
          return NULL;
        }
      } else {
        if (status == FontFamilyNotFound) {
          g_warning("Font family '%s' not found", name);
        } else if (status == NotTrueTypeFont) {
          g_warning("'%s' is not a TrueType font", name);
        }
        g_object_unref(self);
        return NULL;
      }
    }
  }

  self->handle = handle;
  return self;
}
CernFontFamily *
cern_font_family_new(gchar const *name) {
  return
    cern_font_family_new_with_collection(name, NULL);
}

CernFontFamily *
cern_font_family_new_with_collection(gchar const *name,
                                     CernFontCollection *collection) {

}

CernFontFamily *
cern_font_family_new_generic(CernGenericFontFamilies generic) {
  GpStatus status;
  gpointer handle;
  CernFontFamily *self;

  switch (generic) {
    case CernGenericFontFamilies_SansSerif: {
      status = GdipGetGenericFontFamilySansSerif(&handle);
    } break;
    case CernGenericFontFamilies_Serif: {
      status = GdipGetGenericFontFamilySerif(&handle);
    } break;
    case CernGenericFontFamilies_Monospace: {
      status = GdipGetGenericFontFamilyMonospace(&handle);
    } break;
    default: {
      g_critical("Unknown generic font family");
      return NULL;
    } break;
  }

  if (status != Ok) {
    g_warning("%s(...): Failed to get generic font family", __func__);
    return NULL;
  }

  self = cern_font_family_new_internal(handle);
  if (self == NULL) {
    g_critical("%s(...): Failed to create font family", __func__);
    return NULL;
  }

  return self;
}

CernFontFamily *
cern_font_family_new_internal(gpointer handle) {
  CernFontFamily *self;
  self = g_object_new(CERN_TYPE_FONT_FAMILY, NULL);
  self->handle = handle;
  self->create_default_on_fail = FALSE;
  return self;
}

gchar *
cern_font_family_get_name(CernFontFamily *self) {
  static WCHAR buffer[LF_FACESIZE] = { 0 };
  GdipGetFamilyName(self->handle, buffer, 0);
  return g_utf16_to_utf8(buffer, -1, NULL, NULL, NULL);
}

GPtrArray *
cern_font_family_get_families(void) {

}

CernFontFamily *
cern_font_family_get_generic_sans_serif(void);

CernFontFamily *
cern_font_family_get_generic_serif(void);

CernFontFamily *
cern_font_family_get_generic_monospace(void);

gboolean
cern_font_family_is_style_available(CernFontFamily *self, CernFontStyle style) {
  GpStatus status;
  BOOL result;

  status = GdipIsStyleAvailable(self->handle, (INT) style, &result);

  if (status != Ok) {
    g_critical("%s(...): Failed to check if style is available", __func__);
    return FALSE;
  }

  return result != 0;
}

gint32
cern_font_family_get_em_height(CernFontFamily *self, CernFontStyle style) {
  guint16 result = 0;
  GpStatus status;

  status = GdipGetEmHeight(self->handle, (INT) style, &result);

  if (status != Ok) {
    g_critical("%s(...): Failed to get em height", __func__);
    return 0;
  }

  return result;
}

gint32
cern_font_family_get_cell_ascent(CernFontFamily *self, CernFontStyle style) {
  guint16 result = 0;
  GpStatus status;

  status = GdipGetCellAscent(self->handle, (INT) style, &result);

  if (status != Ok) {
    g_critical("%s(...): Failed to get cell ascent", __func__);
    return 0;
  }

  return result;
}

gint32
cern_font_family_get_cell_descent(CernFontFamily *self, CernFontStyle style) {
  guint16 result = 0;
  GpStatus status;

  status = GdipGetCellDescent(self->handle, (INT) style, &result);

  if (status != Ok) {
    g_critical("%s(...): Failed to get cell descent", __func__);
    return 0;
  }

  return result;
}

gint32
cern_font_family_get_line_spacing(CernFontFamily *self, CernFontStyle style) {
  guint16 result = 0;
  GpStatus status;

  status = GdipGetLineSpacing(self->handle, (INT) style, &result);

  if (status != Ok) {
    g_critical("%s(...): Failed to get line spacing", __func__);
    return 0;
  }

  return result;
}

gboolean
cern_font_family_is_equals(CernFontFamily *self, CernFontFamily *other) {
  if (self == NULL || other == NULL) {
    return FALSE;
  }

  if (self == other) {
    return TRUE;
  }

  return self->handle == other->handle;
}

void
cern_font_family_unref(CernFontFamily *self) {
  g_object_unref(self);
}
