#ifndef CERN_PROPERTY_ITEM_H
#define CERN_PROPERTY_ITEM_H 1

#include <glib-object.h>

G_BEGIN_DECLS

#define CERN_TYPE_PROPERTY_ITEM (cern_property_item_get_type())

G_DECLARE_FINAL_TYPE(CernPropertyItem, cern_property_item, CERN, PROPERTY_ITEM, GObject)

guint32
cern_property_item_id(CernPropertyItem *self);

void
cern_property_item_set_id(CernPropertyItem *self, gint32 id);

guint32
cern_property_item_length(CernPropertyItem *self);

void
cern_property_item_set_length(CernPropertyItem *self, gint32 length);

gint16
cern_property_item_type(CernPropertyItem *self);

void
cern_property_item_set_type(CernPropertyItem *self, gint16 type);

GByteArray const *
cern_property_item_value(CernPropertyItem *self);

void
cern_property_item_set_value(CernPropertyItem *self, GByteArray *value);

G_END_DECLS

#endif /* CERN_PROPERTY_ITEM_H */
