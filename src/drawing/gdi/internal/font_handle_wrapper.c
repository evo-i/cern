#include "cern/drawing/internal/font_handle_wrapper.h"
#include "cern/drawing/font.h"

struct _CernFontHandleWrapper {
  GObject parent_instance;

  CernFont *font;
};

G_DEFINE_TYPE(CernFontHandleWrapper, cern_font_handle_wrapper, CERN_TYPE_FONT_HANDLE_WRAPPER);

static
void
cern_font_handle_wrapper_dispose(GObject *obj) {
  CernFontHandleWrapper *self = CERN_FONT_HANDLE_WRAPPER(obj);

  if (self->font) {
    g_clear_object(&self->font);
  }

  G_OBJECT_CLASS(cern_font_handle_wrapper_parent_class)->dispose(obj);
}

static
void
cern_font_handle_wrapper_finalize(GObject *obj) {
  G_OBJECT_CLASS(cern_font_handle_wrapper_parent_class)->finalize(obj);
}

static
void
cern_font_handle_wrapper_class_init(CernFontHandleWrapperClass *klass) {

}

static
void
cern_font_handle_wrapper_init(CernFontHandleWrapper *self) {

}

CernFontHandleWrapper *
cern_font_handle_wrapper_new(CernFont *font) {
  CernFontHandleWrapper *self = g_object_new(CERN_TYPE_FONT_HANDLE_WRAPPER, NULL);
  self->font = font;
  return self;
}

CernFont *
cern_font_handle_wrapper_get_font(CernFontHandleWrapper *self) {
  return self->font;
}

void
cern_font_handle_wrapper_set_font(CernFontHandleWrapper *self, CernFont *font) {
  if (cern_font_handle_wrapper_get_font(self) != NULL) {
    g_clear_object(&self->font);
  }
  self->font = font;
}

void
cern_font_handle_wrapper_unref(CernFontHandleWrapper *self) {
  g_object_unref(self);
}

