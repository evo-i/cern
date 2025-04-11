#include "cern/drawing/color.h"
#include "cern/drawing/bitmap.h"

#include "cern/drawing/internal/color_translator.h"
#include "cern/drawing/internal/native_gdi_object.h"

#include <Windows.h>
#include <gdiplus.h>

#include <shlwapi.h>

extern
gpointer
cern_bitmap_data_get_native(CernBitmapData *self);

struct _CernBitmap {
  CernImage parent_instance;
};

G_DEFINE_FINAL_TYPE(CernBitmap, cern_bitmap, CERN_TYPE_IMAGE)

static
void
cern_bitmap_init(CernBitmap *self) {
  // Initialization code here
}

static
void
cern_bitmap_finalize(GObject *object) {
  CernBitmap *self = CERN_BITMAP(object);
  G_OBJECT_CLASS(cern_bitmap_parent_class)->finalize(object);
}

static
void
cern_bitmap_class_init(CernBitmapClass *klass) {
  GObjectClass *object_class = G_OBJECT_CLASS(klass);
  object_class->finalize = cern_bitmap_finalize;
}

static
gpointer
cern_bitmap_get_native(CernBitmap *self) {
  CernNativeGdiObject *native_gdi_object;
  native_gdi_object = CERN_NATIVE_GDI_OBJECT(self);
  return cern_native_gdi_object_get_native_handle(native_gdi_object);
}

static
void
cern_bitmap_set_native(CernBitmap *self, gpointer ptr) {
  CernNativeGdiObject *native_gdi_object;
  native_gdi_object = CERN_NATIVE_GDI_OBJECT(self);
  cern_native_gdi_object_set_native_handle(native_gdi_object, ptr);
}

CernBitmap *
cern_bitmap_new(guint32 width, guint32 height) {
  return
    cern_bitmap_new_with_format(width, height, CernPixelFormat_Format32bppArgb);
}

CernBitmap *
cern_bitmap_new_with_format(guint32 width, guint32 height, CernPixelFormat pixel_format) {
  GpStatus status;
  gpointer bitmap;
  CernBitmap *self;

  self = NULL;
  status
    = GdipCreateBitmapFromScan0(width, height, 0, (PixelFormat) pixel_format, NULL, &bitmap);

  if (status != Ok) {
    g_critical("cern_bitmap_new(...): GdipCreateBitmapFromScan0() Failed");
    return self;
  }

  self = g_object_new(CERN_TYPE_BITMAP, NULL);
  cern_bitmap_set_native(self, bitmap);

  return self;
}

CernBitmap *
cern_bitmap_new_from_file(const char *filename) {
  CernBitmap *self;
  GpStatus status;
  GpBitmap *bitmap;

  wchar_t *filename_w;

  filename_w = g_utf8_to_utf16(filename, -1, NULL, NULL, NULL);

  status = GdipCreateBitmapFromFile(filename_w, &bitmap);

  g_free(filename_w);

  if (status != Ok) {
    return NULL;
  }

  self = g_object_new(CERN_TYPE_BITMAP, NULL);

  cern_bitmap_set_native(self, bitmap);

  return self;
}

CernBitmap *
cern_bitmap_new_from_stream(GInputStream *stream) {
  GpStatus status;
  CernBitmap *self;
  GError *error;
  gpointer bitmap;
  gpointer temp_memory;

  IStream *mem_stream;
  goffset mem_stream_size;
  GSeekable *mem_stream_seekable;
  GMemoryInputStream *mem_input_stream;

  error = NULL;

  mem_stream_seekable = G_SEEKABLE(stream);
  mem_input_stream = G_MEMORY_INPUT_STREAM(stream);

  if (!g_seekable_seek(mem_stream_seekable, 0, G_SEEK_END, NULL, &error)) {
    g_critical("cern_bitmap_new_from_stream(...): Failed to seek to the end of the stream: %s", error->message);
    g_error_free(error);
    return NULL;
  }

  mem_stream_size = g_seekable_tell(mem_stream_seekable);

  g_seekable_seek(mem_stream_seekable, 0, G_SEEK_SET, NULL, &error);

  temp_memory = g_new(gchar, mem_stream_size);

  if (!temp_memory) {
    g_critical("cern_bitmap_new_from_stream(...): Failed to allocate memory for temporary buffer");
    if (error) {
      g_error_free(error);
    }
    return NULL;
  }

  g_input_stream_read(stream, temp_memory, mem_stream_size, NULL, &error);

  mem_stream = SHCreateMemStream(temp_memory, mem_stream_size);

  if (!mem_stream) {
    g_critical("cern_bitmap_new_from_stream(...): Failed to create memory stream");
    if (error) {
      g_error_free(error);
    }
    g_free(temp_memory);
    return NULL;
  }

  status = GdipCreateBitmapFromStream(mem_stream, &bitmap);

  if (status != Ok) {
    g_critical("cern_bitmap_new_from_stream(...): GdipCreateBitmapFromStream() Failed");
    mem_stream->lpVtbl->Release(mem_stream);
    if (error) {
      g_error_free(error);
    }
    g_free(temp_memory);
    return NULL;
  }

  if (error) {
    g_error_free(error);
  }
  mem_stream->lpVtbl->Release(mem_stream);
  g_free(temp_memory);

  self = g_object_new(CERN_TYPE_BITMAP, NULL);
  cern_bitmap_set_native(self, bitmap);
  return self;
}

CernBitmap *
cern_bitmap_new_from_h_icon(gpointer h_icon) {
  CernBitmap *self;
  GpStatus status;
  gpointer bitmap;

  status = GdipCreateBitmapFromHICON(h_icon, &bitmap);

  if (status != Ok) {
    g_warning("%s: GdipCreateBitmapFromHICON failed\n", __func__);
    return NULL;
  }

  self = g_object_new(CERN_TYPE_BITMAP, NULL);
  cern_bitmap_set_native(self, bitmap);
  return self;
}

CernBitmap *
cern_bitmap_new_from_h_bitmap(gpointer h_bitmap) {
  return cern_bitmap_new_from_h_bitmap_with_palette(h_bitmap, NULL);
}

CernBitmap *
cern_bitmap_new_from_h_bitmap_with_palette(gpointer h_bitmap, gpointer h_palette) {
  CernBitmap *self;
  GpStatus status;
  gpointer bitmap;

  status = GdipCreateBitmapFromHBITMAP(h_bitmap, h_palette, &bitmap);

  if (status != Ok) {
    g_critical("%s: GdipCreateBitmapFromHBITMAP failed\n", __func__);
    return NULL;
  }

  self = g_object_new(CERN_TYPE_BITMAP, NULL);
  cern_bitmap_set_native(self, bitmap);
  return self;
}

/* TODO: Somewhere is it needed? */
CernBitmap *
cern_bitmap_new_from_data(const char *data, int width, int height);

gpointer
cern_bitmap_get_h_bitmap(CernBitmap *self) {
  CernColor bg = cern_color_light_gray();
  return cern_bitmap_get_h_bitmap_with_background(self, &bg);
}

gpointer
cern_bitmap_get_h_bitmap_with_background(CernBitmap *self, CernColor *bg) {
  HBITMAP h_bitmap;
  GpStatus status;
  GpBitmap *bitmap;
  gint32 win32_color;

  bitmap = cern_bitmap_get_native(self);

  win32_color = cern_color_translator_to_win32(bg);
  status
    = GdipCreateHBITMAPFromBitmap(bitmap, &h_bitmap, win32_color);

  if (status != Ok) {
    g_warning("%s: GdipCreateHBITMAPFromBitmap failed\n", __func__);
    return NULL;
  }

  return h_bitmap;
}

gpointer
cern_bitmap_get_h_icon(CernBitmap *self) {
  HICON h_icon;
  GpStatus status;
  GpBitmap *bitmap;

  bitmap = cern_bitmap_get_native(self);
  status = GdipCreateHICONFromBitmap(bitmap, &h_icon);

  if (status != Ok) {
    g_warning("%s: GdipCreateHICONFromBitmap failed\n", __func__);
    return NULL;
  }

  return h_icon;
}

CernColor
cern_bitmap_get_pixel(CernBitmap *self, int x, int y) {
  CernColor color;
  GpStatus status;
  GpBitmap *bitmap;
  guint32 argb = 0;
  ARGB gb = 0;

  color = cern_color_empty();

  if (x < 0 || x >= cern_image_get_width(CERN_IMAGE(self))) {
    g_warning("%s: x is out of range\n", __func__);
    return color;
  }

  if (y < 0 || y >= cern_image_get_height(CERN_IMAGE(self))) {
    g_warning("%s: y is out of range\n", __func__);
    return color;
  }

  bitmap = cern_bitmap_get_native(self);

  status = GdipBitmapGetPixel(bitmap, x, y, (ARGB *) &argb);

  if (status != Ok) {
    g_warning("%s: GdipBitmapGetPixel failed\n", __func__);
    return color;
  }

  color = cern_color_from_argb(argb);

  return color;
}

void
cern_bitmap_set_pixel(CernBitmap *self, int x, int y, CernColor *color) {
  GpStatus status;
  GpBitmap *bitmap;
  gint32 argb;

  if (x < 0 || x >= cern_image_get_width(CERN_IMAGE(self))) {
    g_warning("%s: x is out of range\n", __func__);
    return;
  }

  if (y < 0 || y >= cern_image_get_height(CERN_IMAGE(self))) {
    g_warning("%s: y is out of range\n", __func__);
    return;
  }

  bitmap = cern_bitmap_get_native(self);
  argb = cern_color_to_argb(color);
  status = GdipBitmapSetPixel(bitmap, x, y, argb);

  if (status != Ok) {
    g_warning("%s: GdipBitmapSetPixel failed\n", __func__);
  }
}

void
cern_bitmap_set_resolution(CernBitmap *self, gfloat dpi_x, gfloat dpi_y) {
  GpStatus status;
  GpBitmap *bitmap;

  bitmap = cern_bitmap_get_native(self);

  status = GdipBitmapSetResolution(bitmap, dpi_x, dpi_y);

  if (status != Ok) {
    g_warning("%s: GdipBitmapSetResolution failed\n", __func__);
  }
}

CernBitmapData *
cern_bitmap_lock_bits(CernBitmap *self, CernRectangle *rect,
                      CernImageLockMode flags, CernPixelFormat pixel_format,
                      CernBitmapData *bitmap_data) {
  GpStatus status;
  GpBitmap *bitmap;
  BitmapData *bitmap_data_native;

  bitmap = cern_bitmap_get_native(self);

  bitmap_data_native = cern_bitmap_data_get_native(bitmap_data);

  GpRect rect_native;

  rect_native.X = cern_rectangle_get_x(rect);
  rect_native.Y = cern_rectangle_get_y(rect);
  rect_native.Width = cern_rectangle_get_width(rect);
  rect_native.Height = cern_rectangle_get_height(rect);


  status
    = GdipBitmapLockBits(bitmap, &rect_native, flags, pixel_format, bitmap_data_native);

  if (status != Ok) {
    g_warning("%s: GdipBitmapLockBits failed\n", __func__);
    return NULL;
  }

  return bitmap_data;
}

void
cern_bitmap_unlock_bits(CernBitmap *self, CernBitmapData *bitmap_data) {
  GpStatus status;
  GpBitmap *bitmap;
  BitmapData *bitmap_data_native;

  bitmap = cern_bitmap_get_native(self);
  bitmap_data_native = cern_bitmap_data_get_native(bitmap_data);

  status = GdipBitmapUnlockBits(bitmap, bitmap_data_native);

  if (status != Ok) {
    g_warning("%s: GdipBitmapUnlockBits failed\n", __func__);
  } else {
    g_clear_object(&bitmap_data);
  }
}
