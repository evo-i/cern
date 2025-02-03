#include "cern/drawing/2d/graphics_container.h"

struct _CernGraphicsContainer {
  GObject parent_instance;

  guint32 value;
};

G_DEFINE_TYPE(CernGraphicsContainer, cern_graphics_container, G_TYPE_OBJECT)

static void
cern_graphics_container_class_init(CernGraphicsContainerClass *klass) {
  // Add class initialization code here
}

static void
cern_graphics_container_init(CernGraphicsContainer *self) {
  // Add instance initialization code here
}

CernGraphicsContainer *
cern_graphics_container_new(guint32 container) {
  CernGraphicsContainer *self;

  self = g_object_new(CERN_TYPE_GRAPHICS_CONTAINER, NULL);
  self->value = container;

  return self;
}

guint32
cern_graphics_container_get_value(CernGraphicsContainer *self) {
  g_return_val_if_fail(self != NULL, 0);
  g_return_val_if_fail(CERN_IS_GRAPHICS_CONTAINER(self), 0);
  return self->value;
}
