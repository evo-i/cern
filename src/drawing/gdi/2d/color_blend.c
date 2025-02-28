#include "cern/drawing/2d/color_blend.h"

#include "cern/drawing/2d/color_blend.h"

struct _CernColorBlend {
  GObject parent_instance;

  struct _CernColorBlendColors {
    CernColor *colors;
    gsize count;
  } colors;

  struct _CernColorBlendPositions {
    gfloat *positions;
    gsize count;
  } positions;
};

G_DEFINE_FINAL_TYPE(CernColorBlend, cern_color_blend, G_TYPE_OBJECT);

static
void
cern_color_blend_init(CernColorBlend *self) { }

static
void
cern_color_blend_finalize(GObject *object) {
  CernColorBlend *self = CERN_COLOR_BLEND(object);
  g_free(self->colors.colors);
  g_free(self->positions.positions);

  G_OBJECT_CLASS(cern_color_blend_parent_class)->finalize(object);
}

static
void
cern_color_blend_class_init(CernColorBlendClass *klass) {
  GObjectClass *object_class = G_OBJECT_CLASS(klass);
  object_class->finalize = cern_color_blend_finalize;
}

CernColorBlend *
cern_color_blend_new(void) {
  CernColorBlend *self;

  self = g_object_new(CERN_TYPE_COLOR_BLEND, NULL);

  self->colors.count = 1;
  self->colors.colors = g_new0(CernColor, self->colors.count);

  self->positions.count = 1;
  self->positions.positions = g_new0(gfloat, self->positions.count);

  return self;
}

CernColorBlend *
cern_color_blend_new_with_count(gsize count) {
  CernColorBlend *self;

  self = g_object_new(CERN_TYPE_COLOR_BLEND, NULL);

  self->colors.count = count;
  self->colors.colors = g_new0(CernColor, self->colors.count);

  self->positions.count = count;
  self->positions.positions = g_new0(gfloat, self->positions.count);

  return self;
}

GArray *
cern_color_blend_get_colors(CernColorBlend *self) {
  GArray *array;
  array = g_array_new(FALSE, TRUE, sizeof(CernColor));

  return
    g_array_append_vals(array, self->colors.colors, self->colors.count);
}

void
cern_color_blend_set_colors(CernColorBlend *self, CernColor *colors, gsize count) {
  g_return_if_fail(self != NULL);
  g_return_if_fail(colors != NULL);
  g_return_if_fail(count > 0);

  self->colors.count = count;
  self->colors.colors = g_renew(CernColor, self->colors.colors, count);
  memcpy(self->colors.colors, colors, count * sizeof(CernColor));
}

void
cern_color_blend_set_colors_array(CernColorBlend *self, GArray *colors) {
  g_return_if_fail(self != NULL);
  g_return_if_fail(colors != NULL);
  g_return_if_fail(colors->len > 0);

  self->colors.count = colors->len;
  self->colors.colors = g_renew(CernColor, self->colors.colors, self->colors.count);

  memcpy(self->colors.colors, colors->data,
         self->colors.count * sizeof(CernColor));
}

GArray *
cern_color_blend_get_positions(CernColorBlend *self) {
  GArray *array;
  array = g_array_new(FALSE, TRUE, sizeof(gfloat));
  return
    g_array_append_vals(array, self->positions.positions, self->positions.count);
}

void
cern_color_blend_set_positions_array(CernColorBlend *self, GArray *positions) {
  g_return_if_fail(self != NULL);
  g_return_if_fail(positions != NULL);
  g_return_if_fail(positions->len > 0);

  self->positions.count = positions->len;
  self->positions.positions = g_renew(gfloat, self->positions.positions, self->positions.count);

  memcpy(self->positions.positions, positions->data,
         self->positions.count * sizeof(gfloat));
}

void
cern_color_blend_set_positions(CernColorBlend *self, gfloat *positions, gsize count) {
  g_return_if_fail(self != NULL);
  g_return_if_fail(positions != NULL);
  g_return_if_fail(count > 0);

  self->positions.count = count;
  self->positions.positions = g_renew(gfloat, self->positions.positions, self->positions.count);

  memcpy(self->positions.positions, positions, count * sizeof(gfloat));
}

void
cern_color_blend_unref(CernColorBlend *self) {
  g_object_unref(self);
}
