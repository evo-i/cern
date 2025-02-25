#include "cern/drawing/text/private_font_collection.h"
#include "cern/core/windows/handle.h"

#include <Windows.h>
#include <gdiplus.h>

struct _CernPrivateFontCollection {
  CernFontCollection parent;

  GList *gdi_gonts;
};

static
gpointer
cern_private_font_collection_get(CernHandle *self) {
  return cern_handle_get(self);
}

static
void
cern_private_font_collection_set(CernHandle *self, gpointer handle) {
  cern_handle_set(self, handle);
}

G_DEFINE_FINAL_TYPE(CernPrivateFontCollection, cern_private_font_collection, CERN_TYPE_FONT_COLLECTION);

static
void
cern_private_font_collection_init(CernPrivateFontCollection *self) {
  self->gdi_gonts = NULL;
}

static
void
cern_private_font_collection_each(gpointer data) {
  BOOL status = RemoveFontResourceEx((gchar const *) data, FR_PRIVATE, NULL);
  g_free(data);
}

static
void
cern_font_collection_finalize(GObject *object) {
  CernPrivateFontCollection *self;
  self = CERN_PRIVATE_FONT_COLLECTION(object);

  g_list_free_full(self->gdi_gonts, cern_private_font_collection_each);

  G_OBJECT_CLASS(cern_private_font_collection_parent_class)->finalize(object);
}

static
void
cern_private_font_collection_class_init(CernPrivateFontCollectionClass *klass) {
  GObjectClass *object_class;
  object_class =  G_OBJECT_CLASS(klass);
  object_class->finalize = cern_font_collection_finalize;
}

CernFontCollection *
cern_private_font_collection_new(void) {
  gpointer font_handle;
  CernHandle *handle;
  CernPrivateFontCollection *self;
  GpStatus status;

  status = GdipNewPrivateFontCollection(&font_handle);

  if (status != Ok) {
    g_critical("%s(...): GdipNewPrivateFontCollection failed: %d", __func__, status);
    return NULL;
  }

  self = g_object_new(CERN_TYPE_PRIVATE_FONT_COLLECTION, NULL);

  handle = CERN_HANDLE(self);
  cern_handle_set(handle, font_handle);

  return CERN_FONT_COLLECTION(self);
}

void
cern_private_font_collection_add_font_file(CernPrivateFontCollection *self,
                                           gchar const *file_name) {
  CernHandle *handle;
  gchar *gdi_font;
  GpStatus status;
  gpointer font_collection;

  handle = CERN_HANDLE(self);
  font_collection = cern_handle_get(handle);

  GError *error = NULL;

  gunichar2 *utf16 = g_utf8_to_utf16(file_name, -1, NULL, NULL, &error);

  status = GdipPrivateAddFontFile(font_collection, utf16);

  g_free(utf16);
  if (status != Ok) {
    g_critical("%s(...): GdipPrivateAddFontFile failed: %d", __func__, status);
    return;
  }

  gdi_font = g_strdup(file_name);
  BOOL status2 = AddFontResourceEx(gdi_font, FR_PRIVATE, NULL);
  self->gdi_gonts = g_list_append(self->gdi_gonts, gdi_font);
}

void
cern_private_font_collection_add_font_memory(CernPrivateFontCollection *self,
                                             gpointer memory, gsize length) {
  CernHandle *handle;
  GpStatus status;
  gpointer font_collection;

  handle = CERN_HANDLE(self);
  font_collection = cern_handle_get(handle);

  status
    = GdipPrivateAddMemoryFont(font_collection, memory, length);

  if (status != Ok) {
    g_critical("%s(...): GdipPrivateAddMemoryFont failed: %d", __func__, status);
    return;
  }
}

void
cern_private_font_collection_unref(CernPrivateFontCollection *self) {
  g_object_unref(self);
}
