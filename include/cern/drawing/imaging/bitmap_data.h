#ifndef CERN_BITMAP_DATA_H
#define CERN_BITMAP_DATA_H 1

#include <glib-object.h>
#include "../pixel_format.h"

G_BEGIN_DECLS

#define CERN_TYPE_BITMAP_DATA (cern_bitmap_data_get_type ())

G_DECLARE_FINAL_TYPE (CernBitmapData, cern_bitmap_data, CERN, BITMAP_DATA, GObject)

CernBitmapData *
cern_bitmap_data_new(void);

guint32
cern_bitmap_data_get_width(CernBitmapData *self);

void
cern_bitmap_data_set_width(CernBitmapData *self, guint32 width);

guint32
cern_bitmap_data_get_height(CernBitmapData *self);

void
cern_bitmap_data_set_height(CernBitmapData *self, guint32 height);

guint32
cern_bitmap_data_get_stride(CernBitmapData *self);

void
cern_bitmap_data_set_stride(CernBitmapData *self, guint32 stride);

guint8 *
cern_bitmap_data_get_scan0(CernBitmapData *self);

void
cern_bitmap_data_set_scan0(CernBitmapData *self, guint8 *scan0);

CernPixelFormat
cern_bitmap_data_get_pixel_format(CernBitmapData *self);

void
cern_bitmap_data_set_pixel_format(CernBitmapData *self, CernPixelFormat pixel_format);

G_END_DECLS

#endif /* CERN_BITMAP_DATA_H */
