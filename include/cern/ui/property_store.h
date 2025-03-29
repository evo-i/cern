#ifndef CERN_UI_PROPERTY_STORE_H
#define CERN_UI_PROPERTY_STORE_H 1

#include <glib-object.h>

#include "padding.h"
#include "cern/drawing/size.h"
#include "cern/drawing/rectangle.h"

G_BEGIN_DECLS

typedef struct _CernColor CernColor;

#define CERN_TYPE_PROPERTY_STORE (cern_property_store_get_type())

G_DECLARE_DERIVABLE_TYPE(CernPropertyStore, cern_property_store,
                         CERN, PROPERTY_STORE, GObject)

struct _CernPropertyStoreClass {
  GObjectClass parent_class;

  gboolean
  (*contains_integer)(CernPropertyStore *self, gint32 key);

  gboolean
  (*contains_object)(CernPropertyStore *self, gint32 key);

  CernColor *
  (*get_color)(CernPropertyStore *self, gint32 key);

  CernColor *
  (*try_get_color)(CernPropertyStore *self, gint32 key, gboolean *found);

  CernPadding
  (*get_padding)(CernPropertyStore *self, gint32 key);

  CernPadding
  (*try_get_padding)(CernPropertyStore *self, gint32 key, gboolean *found);

  CernSize
  (*get_size)(CernPropertyStore *self, gint32 key);

  CernSize
  (*try_get_size)(CernPropertyStore *self, gint32 key, gboolean *found);

  CernRectangle
  (*get_rectangle)(CernPropertyStore *self, gint32 key);

  CernRectangle
  (*try_get_rectangle)(CernPropertyStore *self, gint32 key, gboolean *found);

  gint32
  (*get_integer)(CernPropertyStore *self, gint32 key);

  gint32
  (*try_get_integer)(CernPropertyStore *self, gint32 key, gboolean *found);

  gpointer
  (*get_object)(CernPropertyStore *self, gint32 key);

  gpointer
  (*try_get_object)(CernPropertyStore *self, gint32 key, gboolean *found);

  void
  (*set_color)(CernPropertyStore *self, gint32 key, CernColor *value);

  void
  (*set_padding)(CernPropertyStore *self, gint32 key, CernPadding value);

  void
  (*set_size)(CernPropertyStore *self, gint32 key, CernSize value);

  void
  (*set_rectangle)(CernPropertyStore *self, gint32 key, CernRectangle value);

  void
  (*set_integer)(CernPropertyStore *self, gint32 key, gint32 value);

  void
  (*set_object)(CernPropertyStore *self, gint32 key, gpointer value);
};

gint32
cern_property_store_create_key(void);

CernPropertyStore *
cern_property_store_new(void);

gboolean
cern_property_store_contains_integer(CernPropertyStore *self, gint32 key);

gboolean
cern_property_store_contains_object(CernPropertyStore *self, gint32 key);

CernPadding
cern_property_store_try_get_padding(CernPropertyStore *self, gint32 key, gboolean *found);

CernSize
cern_property_store_get_size(CernPropertyStore *self, gint32 key);

CernSize
cern_property_store_try_get_size(CernPropertyStore *self, gint32 key, gboolean *found);

CernRectangle
cern_property_store_get_rectangle(CernPropertyStore *self, gint32 key);

CernRectangle
cern_property_store_try_get_rectangle(CernPropertyStore *self, gint32 key, gboolean *found);

gint32
cern_property_store_get_integer(CernPropertyStore *self, gint32 key);

gint32
cern_property_store_try_get_integer(CernPropertyStore *self, gint32 key, gboolean *found);

gpointer
cern_property_store_get_object(CernPropertyStore *self, gint32 key);

gpointer
cern_property_store_try_get_object(CernPropertyStore *self, gint32 key, gboolean *found);

void
cern_property_store_set_color(CernPropertyStore *self, gint32 key, CernColor *value);

CernColor *
cern_property_store_get_color(CernPropertyStore *self, gint32 key);

CernColor *
cern_property_store_try_get_color(CernPropertyStore *self, gint32 key, gboolean *found);

void
cern_property_store_set_padding(CernPropertyStore *self, gint32 key, CernPadding value);

void
cern_property_store_set_size(CernPropertyStore *self, gint32 key, CernSize value);

void
cern_property_store_set_rectangle(CernPropertyStore *self, gint32 key, CernRectangle value);

void
cern_property_store_set_integer(CernPropertyStore *self, gint32 key, gint32 value);

void
cern_property_store_set_object(CernPropertyStore *self, gint32 key, gpointer value);

void
cern_property_store_remove_integer(CernPropertyStore *self, gint32 key);

void
cern_property_store_remove_object(CernPropertyStore *self, gint32 key);

G_END_DECLS

#endif /* CERN_UI_PROPERTY_STORE_H */
