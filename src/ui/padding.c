#include "cern/ui/padding.h"

G_DEFINE_BOXED_TYPE(CernPadding, cern_padding,
                    cern_padding_copy, cern_padding_free);

static
CernPadding *
cern_padding_init(CernPadding *self,
                  gint32 left, gint32 top,
                  gint32 right, gint32 bottom) {
  self->left = left;
  self->top = top;
  self->right = right;
  self->bottom = bottom;
  return self;
}

CernPadding *
cern_padding_new(gint32 left, gint32 top, gint32 right, gint32 bottom) {
  CernPadding *self = g_new0(CernPadding, 1);
  return cern_padding_init(self, left, top, right, bottom);
}

CernPadding *
cern_padding_copy(CernPadding *self) {
  CernPadding *copy = g_new0(CernPadding, 1);
  return
    cern_padding_init(copy, self->left, self->top, self->right, self->bottom);
}

void
cern_padding_free(CernPadding *self) {
  g_free(self);
}

void
cern_padding_set_top(CernPadding *self, gint32 top) {
  self->top = top;
}

void
cern_padding_set_left(CernPadding *self, gint32 left) {
  self->left = left;
}

void
cern_padding_set_right(CernPadding *self, gint32 right) {
  self->right = right;
}

void
cern_padding_set_bottom(CernPadding *self, gint32 bottom) {
  self->bottom = bottom;
}

gint32
cern_padding_get_top(CernPadding *self) {
  return self->top;
}

gint32
cern_padding_get_left(CernPadding *self) {
  return self->left;
}

gint32
cern_padding_get_right(CernPadding *self) {
  return self->right;
}

gint32
cern_padding_get_bottom(CernPadding *self) {
  return self->bottom;
}

gint32
cern_padding_get_all(CernPadding *self) {
  if (self->left == self->top
      && self->top == self->right
      && self->right == self->bottom) {
    return self->left;
  }

  return -1;
}

gint32
cern_padding_get_horizontal(CernPadding *self) {
  return self->left + self->right;
}

gint32
cern_padding_get_vertical(CernPadding *self) {
  return self->top + self->bottom;
}

CernSize
cern_padding_get_size(CernPadding *self) {
  CernSize size = {
    .width = cern_padding_get_horizontal(self),
    .height = cern_padding_get_vertical(self)
  };

  return size;
}

gboolean
cern_padding_equals(CernPadding *self, CernPadding *other) {
  return self->left == other->left
    && self->top == other->top
    && self->right == other->right
    && self->bottom == other->bottom;
}
