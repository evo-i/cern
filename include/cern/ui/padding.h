#ifndef CERN_UI_PADDING_H
#define CERN_UI_PADDING_H 1

#include <glib-object.h>
#include "cern/drawing/size.h"

G_BEGIN_DECLS

#define CERN_TYPE_PADDING (cern_padding_get_type())

typedef struct _CernPadding {
  gint32 top;
  gint32 left;
  gint32 right;
  gint32 bottom;
} CernPadding;

GType
cern_padding_get_type(void);

CernPadding *
cern_padding_new(gint32 left, gint32 top, gint32 right, gint32 bottom);

CernPadding *
cern_padding_copy(CernPadding *self);

void
cern_padding_free(CernPadding *self);

inline
CernPadding
cern_padding_empty(void) {
  return (CernPadding) { 0 };
}

void
cern_padding_set_top(CernPadding *self, gint32 top);

void
cern_padding_set_left(CernPadding *self, gint32 left);

void
cern_padding_set_right(CernPadding *self, gint32 right);

void
cern_padding_set_bottom(CernPadding *self, gint32 bottom);

gint32
cern_padding_get_top(CernPadding *self);

gint32
cern_padding_get_left(CernPadding *self);

gint32
cern_padding_get_right(CernPadding *self);

gint32
cern_padding_get_bottom(CernPadding *self);

gint32
cern_padding_get_all(CernPadding *self);

gint32
cern_padding_get_horizontal(CernPadding *self);

gint32
cern_padding_get_vertical(CernPadding *self);

CernSize
cern_padding_get_size(CernPadding *self);

gboolean
cern_padding_equals(CernPadding *self, CernPadding *other);

G_END_DECLS

#endif /* CERN_UI_PADDING_H */
