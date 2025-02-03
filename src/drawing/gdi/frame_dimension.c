#include "cern/drawing/frame_dimension.h"

struct _CernFrameDimension {
  GObject parent_instance;

  GUID guid;
};

G_DEFINE_TYPE(CernFrameDimension, cern_frame_dimension, G_TYPE_OBJECT)

static
void
cern_frame_dimension_finalize(GObject *object) {
  CernFrameDimension *self = CERN_FRAME_DIMENSION(object);
  G_OBJECT_CLASS(cern_frame_dimension_parent_class)->finalize(object);
}

static
void
cern_frame_dimension_class_init(CernFrameDimensionClass *klass) {
  GObjectClass *object_class = G_OBJECT_CLASS(klass);
  object_class->finalize = cern_frame_dimension_finalize;
}

static
void
cern_frame_dimension_init(CernFrameDimension *self) { }

/* 6aedbd6d-3fb5-418a-83a6-7f45229dc872 */
const GUID
cern_frame_dimension_time_guid = {
  0x6aedbd6d, 0x3fb5, 0x418a, { 0x83, 0xa6, 0x7f, 0x45, 0x22, 0x9d, 0xc8, 0x72 }
};

/* 84236f7b-3bd3-428f-8dab-4ea1439ca315 */
const GUID
cern_frame_dimension_resolution_guid = {
  0x84236f7b, 0x3bd3, 0x428f, { 0x8d, 0xab, 0x4e, 0xa1, 0x43, 0x9c, 0xa3, 0x15 }
};

/* 7462dc86-6180-4c7e-8e3f-ee7333a7a483 */
const GUID
cern_frame_dimension_page_guid = {
  0x7462dc86, 0x6180, 0x4c7e, { 0x8e, 0x3f, 0xee, 0x73, 0x33, 0xa7, 0xa4, 0x83 }
};

CernFrameDimension *
cern_frame_dimension_new(GUID const *guid) {
  CernFrameDimension *self;
  self = g_object_new(CERN_TYPE_FRAME_DIMENSION, NULL);
  self->guid = *guid;
  return self;
}

CernFrameDimension const *
cern_frame_dimension_time(void) {
  static CernFrameDimension *time_dimension = NULL;

  if (time_dimension == NULL) {
    time_dimension = cern_frame_dimension_new(&cern_frame_dimension_time_guid);
  }

  return time_dimension;
}

CernFrameDimension const *
cern_frame_dimension_resolution(void) {
  static CernFrameDimension *resolution_dimension = NULL;

  if (resolution_dimension == NULL) {
    resolution_dimension = cern_frame_dimension_new(&cern_frame_dimension_resolution_guid);
  }

  return resolution_dimension;
}

CernFrameDimension const *
cern_frame_dimension_page(void) {
  static CernFrameDimension *page_dimension = NULL;
  if (page_dimension == NULL) {
    page_dimension = cern_frame_dimension_new(&cern_frame_dimension_page_guid);
  }
  return page_dimension;
}

GUID const *
cern_frame_dimension_get_guid(CernFrameDimension *self) {
  return &self->guid;
}

gchar const *
cern_frame_dimension_get_name(CernFrameDimension *self) {
  if (memcmp(&self->guid, &cern_frame_dimension_time_guid, sizeof(GUID)) == 0) {
    return "Time";
  } else if (memcmp(&self->guid, &cern_frame_dimension_resolution_guid, sizeof(GUID)) == 0) {
    return "Resolution";
  } else if (memcmp(&self->guid, &cern_frame_dimension_page_guid, sizeof(GUID)) == 0) {
    return "Page";
  }

  static gchar buffer[128] = { 0 };
  g_snprintf(buffer, sizeof(buffer),
    "[FrameDimension: %08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x]", 11,
    self->guid.Data1, self->guid.Data2, self->guid.Data3,
    self->guid.Data4[0], self->guid.Data4[1], self->guid.Data4[2],
    self->guid.Data4[3], self->guid.Data4[4], self->guid.Data4[5],
    self->guid.Data4[6], self->guid.Data4[7]);

  return buffer;
}

gboolean
cern_frame_dimension_equals(CernFrameDimension *self, CernFrameDimension *other) {
  return memcmp(&self->guid, &other->guid, sizeof(GUID)) == 0;
}
