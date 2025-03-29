#include "cern/ui/ambient_properties.h"

struct _CernAmbientProperties {
  GObject parent_instance;

  CernFont *font;
  CernCursor *cursor;
  CernColor back_color;
  CernColor fore_color;
};

G_DEFINE_FINAL_TYPE(CernAmbientProperties, cern_ambient_properties, G_TYPE_OBJECT);

static
void
cern_ambient_properties_finalize(GObject *object) {
  CernAmbientProperties *self = CERN_AMBIENT_PROPERTIES(object);
  G_OBJECT_CLASS(cern_ambient_properties_parent_class)->finalize(object);
}

static
void
cern_ambient_properties_dispose(GObject *object) {
  CernAmbientProperties *self = CERN_AMBIENT_PROPERTIES(object);

  if (self->cursor) {
    g_clear_object(&self->cursor);
  }

  if (self->font) {
    g_clear_object(&self->font);
  }

  G_OBJECT_CLASS(cern_ambient_properties_parent_class)->dispose(object);
}

static
void
cern_ambient_properties_class_init(CernAmbientPropertiesClass *klass) {
  GObjectClass *object_class = G_OBJECT_CLASS(klass);
  object_class->finalize = cern_ambient_properties_finalize;
  object_class->dispose = cern_ambient_properties_dispose;
}

CernAmbientProperties *
cern_ambient_properties_new(void) {
  return g_object_new(CERN_TYPE_AMBIENT_PROPERTIES, NULL);
}

CernFont *
cern_ambient_properties_get_font(CernAmbientProperties *self) {
  if (self->font) {
    return g_object_ref(self->font);
  }

  return NULL;
}

void
cern_ambient_properties_set_font(CernAmbientProperties *self, CernFont *value) {
  if (self->font) {
    g_clear_object(&self->font);
  }

  if (value) {
    self->font = g_object_ref(value);
  }
}

CernCursor *
cern_ambient_properties_get_cursor(CernAmbientProperties *self) {
  if (self->cursor) {
    return g_object_ref(self->cursor);
  }

  return NULL;
}

void
cern_ambient_properties_set_cursor(CernAmbientProperties *self, CernCursor *value) {
  if (self->cursor) {
    g_clear_object(&self->cursor);
  }

  if (value) {
    self->cursor = g_object_ref(value);
  }
}

CernColor
cern_ambient_properties_get_back_color(CernAmbientProperties *self) {
  return self->back_color;
}

void
cern_ambient_properties_set_back_color(CernAmbientProperties *self, CernColor *value) {
  self->back_color = *value;
}

CernColor
cern_ambient_properties_get_fore_color(CernAmbientProperties *self) {
  return self->fore_color;
}

void
cern_ambient_properties_set_fore_color(CernAmbientProperties *self, CernColor *value) {
  self->fore_color = *value;
}
