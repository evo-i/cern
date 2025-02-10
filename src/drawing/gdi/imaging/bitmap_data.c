#include "cern/drawing/imaging/bitmap_data.h"

#include <Windows.h>
#include <gdiplus.h>

struct _CernBitmapData {
  GObject parent_instance;
  BitmapData data;
};

G_DEFINE_TYPE(CernBitmapData, cern_bitmap_data, G_TYPE_OBJECT)

gpointer
cern_bitmap_data_get_native(CernBitmapData *self) {
  return &self->data;
}

static void
cern_bitmap_data_init(CernBitmapData *self) {
  // Initialization code here
}

static void
cern_bitmap_data_finalize(GObject *object) {
  CernBitmapData *self = CERN_BITMAP_DATA(object);

  G_OBJECT_CLASS(cern_bitmap_data_parent_class)->finalize(object);
}

static void
cern_bitmap_data_class_init(CernBitmapDataClass *klass) {
  GObjectClass *object_class = G_OBJECT_CLASS(klass);
  object_class->finalize = cern_bitmap_data_finalize;
  // Class initialization code here
  // ...
}

CernBitmapData *
cern_bitmap_data_new(void) {
  return g_object_new(CERN_TYPE_BITMAP_DATA, NULL);
}

guint32
cern_bitmap_data_get_width(CernBitmapData *self) {
  return self->data.Width;
}

void
cern_bitmap_data_set_width(CernBitmapData *self, guint32 width) {
  self->data.Width = width;
}

guint32
cern_bitmap_data_get_height(CernBitmapData *self) {
  return self->data.Height;
}

void
cern_bitmap_data_set_height(CernBitmapData *self, guint32 height) {
  self->data.Height = height;
}

guint32
cern_bitmap_data_get_stride(CernBitmapData *self) {
  return self->data.Stride;
}

void
cern_bitmap_data_set_stride(CernBitmapData *self, guint32 stride) {
  self->data.Stride = stride;
}

guint8 *
cern_bitmap_data_get_scan0(CernBitmapData *self) {
  return self->data.Scan0;
}

void
cern_bitmap_data_set_scan0(CernBitmapData *self, guint8 *scan0) {
  self->data.Scan0 = scan0;
}

CernPixelFormat
cern_bitmap_data_get_pixel_format(CernBitmapData *self) {
  return self->data.PixelFormat;
}

void
cern_bitmap_data_set_pixel_format(CernBitmapData *self, CernPixelFormat pixel_format) {
  self->data.PixelFormat = pixel_format;
}
