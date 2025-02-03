#ifndef __CERN_DRAWING_BRUSH_H__
#define __CERN_DRAWING_BRUSH_H__ 1

#include <glib-object.h>

G_BEGIN_DECLS

#define CERN_TYPE_BRUSH (cern_brush_get_type())

G_DECLARE_DERIVABLE_TYPE(CernBrush, cern_brush, CERN, BRUSH, GObject)

struct _CernBrushClass {
    GObjectClass parent_class;

    CernBrush *
    (*clone)(CernBrush *self);

    gpointer padding[12];
};

/* Public methods */
CernBrush *
cern_brush_clone(CernBrush *self);

G_END_DECLS

#endif /* __CERN_DRAWING_BRUSH_H__ */
