#ifndef CERN_DRAWING_SOLID_BRUSH_H
#define CERN_DRAWING_SOLID_BRUSH_H 1

#include <glib-object.h>

G_BEGIN_DECLS

#include "brush.h"

typedef struct _CernColor CernColor;

#define CERN_TYPE_SOLID_BRUSH (cern_solid_brush_get_type())

G_DECLARE_FINAL_TYPE(CernSolidBrush, cern_solid_brush, CERN, SOLID_BRUSH, CernBrush)

CernBrush *
cern_solid_brush_new(CernColor *color);

CernBrush *
cern_solid_brush_new_immutable(CernColor *color, gboolean is_immutable);

CernColor
cern_solid_brush_get_color(CernSolidBrush *self);

void
cern_solid_brush_set_color(CernSolidBrush *self, CernColor *color);

G_END_DECLS

#endif /* CERN_DRAWING_SOLID_BRUSH_H */
