#include "cern/drawing/text/installed_font_collection.h"
#include "cern/core/windows/handle.h"

#include <Windows.h>
#include <gdiplus.h>

struct _CernInstalledFontCollection {
  CernFontCollection parent;
};

G_DEFINE_FINAL_TYPE(CernInstalledFontCollection,
  cern_installed_font_collection,
  CERN_TYPE_FONT_COLLECTION);

static
void
cern_installed_font_collection_class_init(CernInstalledFontCollectionClass *klass) { }

static
void
cern_installed_font_collection_init(CernInstalledFontCollection *self) { }

CernFontCollection *
cern_installed_font_collection_new(void) {
  gpointer font_handle;
  CernHandle *handle;
  CernInstalledFontCollection *self;
  GpStatus status;

  status = GdipNewInstalledFontCollection(&font_handle);

  if (status != Ok) {
    g_error("%s(...): GdipNewInstalledFontCollection failed", __func__);
    return NULL;
  }

  self = g_object_new(CERN_TYPE_INSTALLED_FONT_COLLECTION, NULL);
  handle = CERN_HANDLE(self);

  cern_handle_set(handle, font_handle);

  return CERN_FONT_COLLECTION(self);
}
