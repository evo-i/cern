#include "cern/drawing/property_item.h"
#include <glib/gprintf.h>

struct _CernPropertyItem {
  GObject parent_instance;

  guint32 id;
  guint32 length;
  guint16 type;
  GByteArray *value;
};

G_DEFINE_TYPE(CernPropertyItem, cern_property_item, G_TYPE_OBJECT)

static void
cern_property_item_finalize(GObject *object) {
  CernPropertyItem *self = CERN_PROPERTY_ITEM(object);

  if (self->value) {
    g_byte_array_unref(self->value);
  }

  G_OBJECT_CLASS(cern_property_item_parent_class)->finalize(object);
}

static void
cern_property_item_class_init(CernPropertyItemClass *klass) {
  GObjectClass *object_class = G_OBJECT_CLASS(klass);
  object_class->finalize = cern_property_item_finalize;
}

static void
cern_property_item_init(CernPropertyItem *self) {
  self->id = 0;
  self->length = 0;
  self->type = 0;
  self->value = NULL;
}

CernPropertyItem *
cern_property_item_new(void) {
  return g_object_new(CERN_TYPE_PROPERTY_ITEM, NULL);
}

void
cern_property_item_set_id(CernPropertyItem *self, gint32 id) {
  g_return_if_fail(CERN_IS_PROPERTY_ITEM(self));
  self->id = id;
}

guint32
cern_property_item_id(CernPropertyItem *self) {
  g_return_val_if_fail(CERN_IS_PROPERTY_ITEM(self), 0);
  return self->id;
}

void
cern_property_item_set_length(CernPropertyItem *self, gint32 length) {
  g_return_if_fail(CERN_IS_PROPERTY_ITEM(self));
  self->length = length;
}

guint32
cern_property_item_length(CernPropertyItem *self) {
  g_return_val_if_fail(CERN_IS_PROPERTY_ITEM(self), 0);
  return self->length;
}

void
cern_property_item_set_type(CernPropertyItem *self, gint16 type) {
  g_return_if_fail(CERN_IS_PROPERTY_ITEM(self));
  self->type = type;
}

void
cern_property_item_set_value(CernPropertyItem *self, GByteArray *value) {
  g_return_if_fail(CERN_IS_PROPERTY_ITEM(self));

  if (self->value)
    g_byte_array_unref(self->value);

  self->value = value ? g_byte_array_ref(value) : NULL;
}

GByteArray const *
cern_property_item_value(CernPropertyItem *self) {
  g_return_val_if_fail(CERN_IS_PROPERTY_ITEM(self), NULL);
  return self->value;
}

gint16
cern_property_item_type(CernPropertyItem *self) {
  g_return_val_if_fail(CERN_IS_PROPERTY_ITEM(self), 0);
  return self->type;
}
