#include "cern/drawing/2d/region_data.h"

struct _CernRegionData {
  GObject parent_instance;

  GByteArray *data;
};

G_DEFINE_FINAL_TYPE(CernRegionData, cern_region_data, G_TYPE_OBJECT)

static
void
cern_region_data_finalize(GObject *object) {
  CernRegionData *self = CERN_REGION_DATA(object);
  g_byte_array_unref(self->data);
  G_OBJECT_CLASS(cern_region_data_parent_class)->finalize(object);
}

static
void
 cern_region_data_class_init(CernRegionDataClass *klass) {
  GObjectClass *gobject_class = G_OBJECT_CLASS(klass);

  gobject_class->finalize = cern_region_data_finalize;
}

static
void
cern_region_data_init(CernRegionData *self) { }

CernRegionData *
cern_region_data_new(GByteArray *data) {
  CernRegionData *self;

  self = g_object_new(CERN_TYPE_REGION_DATA, NULL);
  self->data = g_byte_array_ref(data);

  return self;
}

GByteArray *
cern_region_data_get_data(CernRegionData *self) {
  return self->data;
}

void
cern_region_data_set_data(CernRegionData *self, GByteArray *data) {
  g_return_if_fail(CERN_IS_REGION_DATA(self));

  if (self->data) {
    g_byte_array_unref(self->data);
    self->data = data ? g_byte_array_ref(data) : NULL;
  }
}
