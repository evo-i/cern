#ifndef CERN_DRAWING_2D_HATCH_BRUSH_H
#define CERN_DRAWING_2D_HATCH_BRUSH_H 1

#include <glib-object.h>

#include "../brush.h"
#include "../color.h"

#include "hatch_style.h"

G_BEGIN_DECLS

#define CERN_TYPE_HATCH_BRUSH (cern_hatch_brush_get_type())

G_DECLARE_FINAL_TYPE(CernHatchBrush, cern_hatch_brush, CERN, HATCH_BRUSH, CernBrush)

CernBrush *
cern_hatch_brush_new(CernHatchStyle style, CernColor *foreground_color);

CernBrush *
cern_hatch_brush_new_with_color(CernHatchStyle style,
                                CernColor *foreground_color,
                                CernColor *background_color);

CernBrush *
cern_hatch_brush_clone(CernHatchBrush *self);

CernHatchStyle
cern_hatch_brush_get_style(CernHatchBrush *self);

CernColor
cern_hatch_brush_get_foreground_color(CernHatchBrush *self);

CernColor
cern_hatch_brush_get_background_color(CernHatchBrush *self);

G_END_DECLS

#endif /* CERN_DRAWING_2D_HATCH_BRUSH_H */
