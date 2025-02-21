#ifndef CERN_DRAWING_GRAPHICS_CONTEXT_H
#define CERN_DRAWING_GRAPHICS_CONTEXT_H 1

#include <glib-object.h>

#include "region.h"
#include "point_f.h"

G_BEGIN_DECLS

typedef struct _CernGraphics CernGraphics;

#define CERN_TYPE_GRAPHICS_CONTEXT (cern_graphics_context_get_type())
G_DECLARE_FINAL_TYPE(CernGraphicsContext, cern_graphics_context, CERN, GRAPHICS_CONTEXT, GObject)

CernGraphicsContext *
cern_graphics_context_new(CernGraphics *graphics);

guint
cern_graphics_context_get_state(CernGraphicsContext *self);

void
cern_graphics_context_set_state(CernGraphicsContext *self, guint state);

CernPointF
cern_graphics_context_get_transform_offset(CernGraphicsContext *self);

CernRegion *
cern_graphics_context_get_clip(CernGraphicsContext *self);

CernGraphicsContext *
cern_graphics_context_get_next(CernGraphicsContext *self);

void
cern_graphics_context_set_next(CernGraphicsContext *self, CernGraphicsContext *next);

CernGraphicsContext *
cern_graphics_context_get_previous(CernGraphicsContext *self);

void
cern_graphics_context_set_previous(CernGraphicsContext *self, CernGraphicsContext *prev);

gboolean
cern_graphics_context_get_is_cumulative(CernGraphicsContext *self);

void
cern_graphics_context_set_is_cumulative(CernGraphicsContext *self, gboolean is_cumulative);

G_END_DECLS

#endif /* CERN_DRAWING_GRAPHICS_CONTEXT_H */
