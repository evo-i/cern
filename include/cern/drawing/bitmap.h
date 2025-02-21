#ifndef CERN_BITMAP_H
#define CERN_BITMAP_H 1

#include <glib-object.h>

#include "image.h"
#include "color.h"
#include "rectangle.h"
#include "imaging/bitmap_data.h"
#include "imaging/lock_mode.h"

G_BEGIN_DECLS

#define CERN_TYPE_BITMAP (cern_bitmap_get_type ())

G_DECLARE_FINAL_TYPE (CernBitmap, cern_bitmap, CERN, BITMAP, CernImage)

CernBitmap *
cern_bitmap_new(guint32 width, guint32 height);

CernBitmap *
cern_bitmap_new_with_format(guint32 width, guint32 height, CernPixelFormat pixel_format);

CernBitmap *
cern_bitmap_new_from_file(const char *filename);

CernBitmap *
cern_bitmap_new_from_stream(GInputStream *stream);

CernBitmap *
cern_bitmap_new_from_h_icon(gpointer h_icon);

CernBitmap *
cern_bitmap_new_from_h_bitmap(gpointer h_bitmap);

CernBitmap *
cern_bitmap_new_from_h_bitmap_with_palette(gpointer h_bitmap, gpointer h_palette);

CernBitmap *
cern_bitmap_new_from_data(const char *data, int width, int height);

gpointer
cern_bitmap_get_h_bitmap(CernBitmap *self);

gpointer
cern_bitmap_get_h_bitmap_with_background(CernBitmap *self, CernColor *bg);

gpointer
cern_bitmap_get_h_icon(CernBitmap *self);

CernColor
cern_bitmap_get_pixel(CernBitmap *self, int x, int y);

void
cern_bitmap_set_pixel(CernBitmap *self, int x, int y, CernColor *color);

void
cern_bitmap_set_resolution(CernBitmap *self, gfloat dpi_x, gfloat dpi_y);

CernBitmapData *
cern_bitmap_lock_bits(CernBitmap *self, CernRectangle *rect,
                      CernImageLockMode flags, CernPixelFormat pixel_format,
                      CernBitmapData *bitmap_data);

void
cern_bitmap_unlock_bits(CernBitmap *self, CernBitmapData *bitmap_data);

G_END_DECLS

#endif /* CERN_BITMAP_H */
