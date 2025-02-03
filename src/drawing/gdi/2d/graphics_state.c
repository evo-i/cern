#include "cern/drawing/2d/graphics_state.h"

struct _CernGraphicsState {
  GObject parent_instance;

  guint32 value;
};

G_DEFINE_FINAL_TYPE(CernGraphicsState, cern_graphics_state, G_TYPE_OBJECT)

static
void
cern_graphics_state_class_init(CernGraphicsStateClass *klass) { }

static
void
cern_graphics_state_init(CernGraphicsState *self) { }

CernGraphicsState *
cern_graphics_state_new(guint32 state) {
  return g_object_new(cern_graphics_state_get_type(), NULL);
}

guint32
cern_graphics_state_get_value(CernGraphicsState *self) {
  g_return_val_if_fail(self != NULL, 0);
  g_return_val_if_fail(CERN_IS_GRAPHICS_STATE(self), 0);
  return self->value;
}
