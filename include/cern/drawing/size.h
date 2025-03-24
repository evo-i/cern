#ifndef CERN_SIZE_H
#define CERN_SIZE_H

#include <glib-object.h>
#include <glib.h>

G_BEGIN_DECLS

typedef struct _CernSize {
  gint32 width;
  gint32 height;
} CernSize;

#define CERN_TYPE_SIZE (cern_size_get_type())

GType
cern_size_get_type (void);

inline
CernSize
cern_size_empty (void) {
  return (CernSize) { 0 };
}

CernSize *
cern_size_new (gint32 width, gint32 height);

CernSize *
cern_size_copy (CernSize *self);

void
cern_size_free (CernSize *self);

CernSize
cern_size_create(gint32 width, gint32 height);

gint32
cern_size_get_width(CernSize *self);

void
cern_size_set_width(CernSize *self, gint32 width);

gint32
cern_size_get_height(CernSize *self);

void
cern_size_set_height(CernSize *self, gint32 height);

gboolean
cern_size_equals(CernSize *first, CernSize *second);

G_END_DECLS

#endif /* CERN_SIZE_H */
