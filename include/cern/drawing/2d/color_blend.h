#ifndef CERN_DRAWING_2D_COLOR_BLEND_H
#define CERN_DRAWING_2D_COLOR_BLEND_H 1

#include <glib-object.h>
#include "../color.h"

G_BEGIN_DECLS

#define CERN_TYPE_COLOR_BLEND (cern_color_blend_get_type())

G_DECLARE_FINAL_TYPE(CernColorBlend, cern_color_blend, CERN, COLOR_BLEND, GObject)

CernColorBlend *
cern_color_blend_new(void);

CernColorBlend *
cern_color_blend_new_with_count(gsize count);

GArray *
cern_color_blend_get_colors(CernColorBlend *self);

void
cern_color_blend_set_colors(CernColorBlend *self, CernColor *colors, gsize count);

void
cern_color_blend_set_colors_array(CernColorBlend *self, GArray *colors);

GArray *
cern_color_blend_get_positions(CernColorBlend *self);

void
cern_color_blend_set_positions_array(CernColorBlend *self, GArray *positions);

void
cern_color_blend_set_positions(CernColorBlend *self, gfloat *positions, gsize count);

void
cern_color_blend_unref(CernColorBlend *self);

G_END_DECLS

#endif /* CERN_DRAWING_2D_COLOR_BLEND_H */
