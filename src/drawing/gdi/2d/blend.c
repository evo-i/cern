#include "cern/drawing/2d/blend.h"

struct _CernBlend {
  GObject parent_instance;

  struct _CernBlendFactors {
    gfloat *factors;
    gsize count;
  } factors;

  struct _CernBlendPositions {
    gfloat *positions;
    gsize count;
  } positions;
};

G_DEFINE_FINAL_TYPE(CernBlend, cern_blend, G_TYPE_OBJECT);

static
void
cern_blend_init(CernBlend *self) { }

static
void
cern_blend_finalize(GObject *object) {
  CernBlend *self = CERN_BLEND(object);
  g_free(self->factors.factors);
  g_free(self->positions.positions);

  G_OBJECT_CLASS(cern_blend_parent_class)->finalize(object);
}

static
void
cern_blend_class_init(CernBlendClass *klass) {
  GObjectClass *object_class = G_OBJECT_CLASS(klass);
  object_class->finalize = cern_blend_finalize;
}

CernBlend *
cern_blend_new(void) {
  CernBlend *self;

  self = g_object_new(CERN_TYPE_BLEND, NULL);

  self->factors.count = 1;
  self->factors.factors = g_new0(gfloat, self->factors.count);

  self->positions.count = 1;
  self->positions.positions = g_new0(gfloat, self->positions.count);

  return self;
}

CernBlend *
cern_blend_new_with_count(gsize count) {
  CernBlend *self;

  self = g_object_new(CERN_TYPE_BLEND, NULL);

  self->factors.count = count;
  self->factors.factors = g_new0(gfloat, self->factors.count);

  self->positions.count = count;
  self->positions.positions = g_new0(gfloat, self->positions.count);

  return self;
}

GArray *
cern_blend_get_factors(CernBlend *self) {
  GArray *array;
  array = g_array_new(FALSE, TRUE, sizeof(gfloat));

  return
    g_array_append_vals(array, self->factors.factors, self->factors.count);
}

void
cern_blend_set_factors(CernBlend *self, gfloat *factors, gsize count) {
  g_return_if_fail(self != NULL);
  g_return_if_fail(factors != NULL);
  g_return_if_fail(count > 0);

  self->factors.count = count;
  self->factors.factors = g_renew(gfloat, self->factors.factors, count);
  memcpy(self->factors.factors, factors, count * sizeof(gfloat));
}

void
cern_blend_set_factors_array(CernBlend *self, GArray *factors) {
  g_return_if_fail(self != NULL);
  g_return_if_fail(factors != NULL);
  g_return_if_fail(factors->len > 0);

  self->factors.count = factors->len;
  self->factors.factors
    = g_renew(gfloat, self->factors.factors, self->factors.count);

  memcpy(self->factors.factors, factors->data,
         self->factors.count * sizeof(gfloat));
}

GArray *
cern_blend_get_positions(CernBlend *self) {
  GArray *array;
  array = g_array_new(FALSE, TRUE, sizeof(gfloat));
  return
    g_array_append_vals(array, self->positions.positions, self->positions.count);
}

void
cern_blend_set_positions_array(CernBlend *self, GArray *positions) {
  g_return_if_fail(self != NULL);
  g_return_if_fail(positions != NULL);
  g_return_if_fail(positions->len > 0);

  self->positions.count = positions->len;
  self->positions.positions
    = g_renew(gfloat, self->positions.positions, self->positions.count);

  memcpy(self->positions.positions, positions->data,
         self->positions.count * sizeof(gfloat));
}

void
cern_blend_set_positions(CernBlend *self, gfloat *positions, gsize count) {
  g_return_if_fail(self != NULL);
  g_return_if_fail(positions != NULL);
  g_return_if_fail(count > 0);

  self->positions.count = count;
  self->positions.positions
    = g_renew(gfloat, self->positions.positions, self->positions.count);

  memcpy(self->positions.positions, positions, count * sizeof(gfloat));
}

void
cern_blend_unref(CernBlend *self) {
  g_object_unref(self);
}
