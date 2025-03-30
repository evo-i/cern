#include "cern/drawing/size.h"
#include <glib.h>

G_DEFINE_BOXED_TYPE(CernSize, cern_size, cern_size_copy, cern_size_free)

CernSize *
cern_size_new (gint32 width, gint32 height) {
  return g_new0(CernSize, 1);
}

CernSize *
cern_size_copy(CernSize *self) {
  return g_memdup2(self, sizeof(CernSize));
}

void
cern_size_free(CernSize *self) {
  g_free(self);
}

CernSize
cern_size_create(gint32 width, gint32 height) {
  return (CernSize) {
    .width = width,
    .height = height
  };
}

gint32
cern_size_get_width(CernSize *self) {
  return self->width;
}

void
cern_size_set_width(CernSize *self, gint32 width) {
  self->width = width;
}

gint32
cern_size_get_height(CernSize *self) {
  return self->height;
}

void
cern_size_set_height(CernSize *self, gint32 height) {
  self->height = height;
}

gboolean
cern_size_equals(CernSize *first, CernSize *second) {
  return 
    first->width == second->width
      && first->height == second->height;
}

