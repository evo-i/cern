#ifndef CERN_DRAWING_BRUSH_H
#define CERN_DRAWING_BRUSH_H 1

#include <glib-object.h>

G_BEGIN_DECLS

#define CERN_TYPE_BRUSH (cern_brush_get_type())

G_DECLARE_DERIVABLE_TYPE(CernBrush, cern_brush, CERN, BRUSH, GObject)

struct _CernBrushClass {
  GObjectClass parent_class;

  CernBrush *
  (*clone)(CernBrush *self);

  /* padding */
  gpointer padding[4];
};

CernBrush *
cern_brush_clone(CernBrush *self);

G_END_DECLS

#endif /* CERN_DRAWING_BRUSH_H */
