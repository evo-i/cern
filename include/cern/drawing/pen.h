#ifndef CERN_DRAWING_PEN_H
#define CERN_DRAWING_PEN_H 1

#include <glib-object.h>

#include "color.h"
#include "brush.h"

#include "2d/line_cap.h"
#include "2d/dash_cap.h"
#include "2d/line_join.h"
#include "2d/pen_alignment.h"
#include "2d/matrix_order.h"
#include "2d/pen_type.h"
#include "2d/dash_style.h"

G_BEGIN_DECLS

typedef struct _CernCustomLineCap CernCustomLineCap;
typedef struct _CernMatrix CernMatrix;

#define CERN_TYPE_PEN (cern_pen_get_type())

G_DECLARE_FINAL_TYPE (CernPen, cern_pen, CERN, PEN, GObject)

CernPen *
cern_pen_new (CernColor *color);

CernPen *
cern_pen_new_width(CernColor *color, gfloat width);

CernPen *
cern_pen_new_brush(CernBrush *brush);

CernPen *
cern_pen_new_brush_and_width(CernBrush *brush, gfloat width);

/* Getters and setters */
gfloat
cern_pen_get_width(CernPen *self);

void
cern_pen_set_width(CernPen *self, gfloat width);

void
cern_pen_set_line_cap(CernPen *self, CernLineCap start_cap,
                      CernLineCap end_cap, CernLineCap dash_cap);

CernLineCap
cern_pen_get_start_cap(CernPen *self);

void
cern_pen_set_start_cap(CernPen *self, CernLineCap start_cap);

CernLineCap
cern_pen_get_end_cap(CernPen *self);

void
cern_pen_set_end_cap(CernPen *self, CernLineCap end_cap);

CernDashCap
cern_pen_get_dash_cap(CernPen *self);

void
cern_pen_set_dash_cap(CernPen *self, CernDashCap dash_cap);

CernLineJoin
cern_pen_get_line_join(CernPen *self);

void
cern_pen_set_line_join(CernPen *self, CernLineJoin line_join);

CernCustomLineCap *
cern_pen_get_custom_start_cap(CernPen *self);

void
cern_pen_set_custom_start_cap(CernPen *self, CernCustomLineCap *custom_line_cap);

CernCustomLineCap *
cern_pen_get_custom_end_cap(CernPen *self);

void
cern_pen_set_custom_end_cap(CernPen *self, CernCustomLineCap *custom_line_cap);

gfloat
cern_pen_get_miter_limit(CernPen *self);

void
cern_pen_set_miter_limit(CernPen *self, gfloat miter_limit);

CernPenAlignment
cern_pen_get_alignment(CernPen *self);

void
cern_pen_set_alignment(CernPen *self, CernPenAlignment alignment);

CernMatrix *
cern_pen_get_transform(CernPen *self);

void
cern_pen_set_transform(CernPen *self, CernMatrix *transform);

void
cern_pen_reset_transform(CernPen *self);

void
cern_pen_multiply_transform(CernPen *self, CernMatrix *transform);

void
cern_pen_muliply_transform_with_order(CernPen *self, CernMatrix *transform,
                                      CernMatrixOrder order);

void
cern_pen_translate_transform(CernPen *self, gfloat offset_x, gfloat offset_y);

void
cern_pen_translate_transform_with_order(CernPen *self,
                                        gfloat offset_x, gfloat offset_y,
                                        CernMatrixOrder order);

void
cern_pen_scale_transform(CernPen *self, gfloat scale_x, gfloat scale_y);

void
cern_pen_scale_transform_with_order(CernPen *self,
                                    gfloat scale_x, gfloat scale_y,
                                    CernMatrixOrder order);

void
cern_pen_rotate_transform(CernPen *self, gfloat angle);

void
cern_pen_rotate_transform_with_order(CernPen *self, gfloat angle,
                                     CernMatrixOrder order);

CernPenType
cern_pen_get_pen_type(CernPen *self);

CernColor
cern_pen_get_color(CernPen *self);

void
cern_pen_set_color(CernPen *self, CernColor *color);

CernBrush *
cern_pen_get_brush(CernPen *self);

void
cern_pen_set_brush(CernPen *self, CernBrush *brush);

CernDashStyle
cern_pen_get_dash_style(CernPen *self);

void
cern_pen_set_dash_style(CernPen *self, CernDashStyle dash_style);

gfloat
cern_pen_get_dash_offset(CernPen *self);

void
cern_pen_set_dash_offset(CernPen *self, gfloat dash_offset);

void
cern_pen_get_dash_pattern(CernPen *self, gfloat *pattern, gint32 *count);

void
cern_pen_set_dash_pattern(CernPen *self, gfloat *dash_pattern, gint32 count);

void
cern_pen_get_compound_array(CernPen *self, gfloat *array, gint32 *count);

void
cern_pen_set_compound_array(CernPen *self, gfloat const *array, gint32 count);

G_END_DECLS

#endif /* CERN_DRAWING_PEN_H */
