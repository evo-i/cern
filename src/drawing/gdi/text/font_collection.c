#include "cern/drawing/text/font_collection.h"
#include "cern/drawing/font_family.h"
#include "cern/core/windows/handle.h"

#include <Windows.h>
#include <gdiplus.h>

static
inline
gpointer
cern_font_collection_get_instance_private(CernFontCollection *self);

typedef struct _CernFontCollectionPrivate {
  gpointer handle;
} CernFontCollectionPrivate;

static
gpointer
cern_font_collection_get_handle(CernFontCollection *self) {
  CernFontCollectionPrivate *priv;
  priv = cern_font_collection_get_instance_private(self);
  return priv->handle;
}

static
void
cern_font_collection_set_handle(CernFontCollection *self, gpointer handle) {
  CernFontCollectionPrivate *priv;
  priv = cern_font_collection_get_instance_private(self);
  priv->handle = handle;
}

static
void
fc_handle_interface_init(CernHandleInterface *iface) {
  iface->get
    = (gpointer(*)(gpointer)) cern_font_collection_get_handle;
  iface->set
    = (void(*)(gpointer, gpointer)) cern_font_collection_set_handle;
}

G_DEFINE_ABSTRACT_TYPE_WITH_CODE(CernFontCollection,
  cern_font_collection,
  G_TYPE_OBJECT,
  G_ADD_PRIVATE(CernFontCollection)
  G_IMPLEMENT_INTERFACE(CERN_TYPE_HANDLE, fc_handle_interface_init));

static
void
cern_font_collection_init(CernFontCollection *self) {
  CernFontCollectionPrivate *priv;
  priv = cern_font_collection_get_instance_private(self);
  priv->handle = NULL;
}

static
GPtrArray *
real_cern_font_collection_get_families(CernFontCollection *self) {
  GPtrArray *families;
  gpointer collection;
  GpStatus status;
  gint32 count;
  gint32 founded;

#ifndef __GNUC__
  GpFontCollection **list;
#endif

  collection = cern_font_collection_get_handle(self);
  status = GdipGetFontCollectionFamilyCount(collection, &count);

  if (status != Ok) {
    g_warning("real_cern_font_collection_get_families(...): GdipGetFontCollectionFamilyCount failed: %d", status);
    return NULL;
  }

#ifdef __GNUC__
  GpFontCollection *list[count];
#else
  list = g_alloca(sizeof(gpointer) * count);
#endif

  status
    = GdipGetFontCollectionFamilyList(collection, count, list, &founded);

  if (status != Ok) {
    g_warning("real_cern_font_collection_get_families(...): GdipGetFontCollectionFamilyList failed: %d", status);
    g_free(list);
    return NULL;
  }

  if (count != founded) {
    g_warning("real_cern_font_collection_get_families(...): GDI+ can't give a straight answer about how many fonts there are...");
  }

  families = g_ptr_array_new_with_free_func((void(*)(gpointer))cern_font_family_unref);

  for (gsize i = 0; i < count; i++) {
    gpointer native;
    status = GdipCloneFontFamily(list[i], &native);
    if (status != Ok) {
      g_warning("real_cern_font_collection_get_families(...): GdipCloneFontFamily failed: %d", status);
    }
    g_ptr_array_add(families, cern_font_family_new_internal(native));
  }

  return families;
}

static
void
cern_font_collection_finalize(GObject *object) {
  CernFontCollection *self;
  self = CERN_FONT_COLLECTION(object);

  /* TODO: What can be here?... it is abstract... */
  g_info("cern_font_collection_finalize: was called...");

  G_OBJECT_CLASS(cern_font_collection_parent_class)->finalize(object);
}

static
void
cern_font_collection_class_init(CernFontCollectionClass *klass) {
  GObjectClass *gobject_class;
  gobject_class = G_OBJECT_CLASS(klass);
  gobject_class->finalize = cern_font_collection_finalize;

  klass->get_families = real_cern_font_collection_get_families;
}

GPtrArray *
cern_font_collection_get_families(CernFontCollection *self) {
  CernFontCollectionClass *klass;

  if (!CERN_IS_FONT_COLLECTION(self)) {
    g_warning("cern_font_collection_get_families: invalid type");
    return NULL;
  }

  klass = CERN_FONT_COLLECTION_GET_CLASS(self);

  return klass->get_families(self);
}
