#ifndef CERN_DRAWING_2D_GRAPHICS_STATE
#define CERN_DRAWING_2D_GRAPHICS_STATE 1

#include <glib-object.h>

G_BEGIN_DECLS

#define CERN_TYPE_GRAPHICS_STATE (cern_graphics_state_get_type())

G_DECLARE_FINAL_TYPE(CernGraphicsState, cern_graphics_state, CERN, GRAPHICS_STATE, GObject)

CernGraphicsState *
cern_graphics_state_new(guint32 state);

guint32
cern_graphics_state_get_value(CernGraphicsState *self);

G_END_DECLS

#endif /* CERN_DRAWING_2D_GRAPHICS_STATE */
