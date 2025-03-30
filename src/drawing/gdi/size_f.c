#include "cern/drawing/size_f.h"

G_DEFINE_BOXED_TYPE(CernSizeF, cern_size_f, cern_size_f_copy, cern_size_f_free)

CernSizeF *
cern_size_f_new(void) {
  return g_new0(CernSizeF, 1);
}

CernSizeF *
cern_size_f_copy(CernSizeF *src) {
  return g_memdup2(src, sizeof(CernSizeF));
}

void
cern_size_f_free(CernSizeF *self) {
  g_free(self);
}

CernSizeF
cern_size_f_create(gfloat width, gfloat height) {
  return (CernSizeF) {
    .width = width,
    .height = height
  };
}

gfloat
cern_size_f_get_width(CernSizeF *self) {
  return self->width;
}

void
cern_size_f_set_width(CernSizeF *self, gfloat width) {
  self->width = width;
}

gfloat
cern_size_f_get_height(CernSizeF *self) {
  return self->height;
}

void
cern_size_f_set_height(CernSizeF *self, gfloat height) {
  self->height = height;
}

