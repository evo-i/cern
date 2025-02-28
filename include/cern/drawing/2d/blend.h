#ifndef CERN_DRAWING_2D_BLEND_H
#define CERN_DRAWING_2D_BLEND_H 1

#include <glib-object.h>

G_BEGIN_DECLS

#define CERN_TYPE_BLEND (cern_blend_get_type())

G_DECLARE_FINAL_TYPE(CernBlend, cern_blend, CERN, BLEND, GObject)

CernBlend *
cern_blend_new(void);

CernBlend *
cern_blend_new_with_count(gsize count);

GArray *
cern_blend_get_factors(CernBlend *self);

void
cern_blend_set_factors(CernBlend *self, gfloat *factors, gsize count);

void
cern_blend_set_factors_array(CernBlend *self, GArray *factors);

GArray *
cern_blend_get_positions(CernBlend *self);

void
cern_blend_set_positions_array(CernBlend *self, GArray *positions);

void
cern_blend_set_positions(CernBlend *self, gfloat *positions, gsize count);

void
cern_blend_unref(CernBlend *self);

G_END_DECLS

#endif /* CERN_DRAWING_2D_BLEND_H */
