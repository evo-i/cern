#ifndef CERN_IMAGE_H
#define CERN_IMAGE_H 1

#include <glib-object.h>
#include <gio/gio.h>

#include "image_format.h"
#include "pixel_format.h"
#include "frame_dimension.h"
#include "rotate_flip_type.h"
#include "graphics_unit.h"
#include "property_item.h"
#include "color_palette.h"

G_BEGIN_DECLS

typedef struct _CernSizeF CernSizeF;
typedef struct _CernRectangleF CernRectangleF;
typedef struct _CernEncoderParameters CernEncoderParameters;

#define CERN_TYPE_IMAGE (cern_image_get_type())

G_DECLARE_DERIVABLE_TYPE(CernImage, cern_image, CERN, IMAGE, GObject)

struct _CernImageClass {
  GObjectClass parent_class;

  /* Properties */
  gint32
  (*get_flags)(CernImage *self);

  GList *
  (*get_frame_dimension_list)(CernImage *self);

  guint32
  (*get_height)(CernImage *self);

  guint32
  (*get_width)(CernImage *self);

  gfloat
  (*get_resolution)(CernImage *self);

  CernColorPalette *
  (*get_palette)(CernImage *self);

  void
  (*set_palette)(CernImage *self, CernColorPalette *palette);

  gfloat
  (*get_physical_dimension)(CernImage *self);

  CernPixelFormat
  (*get_pixel_format)(CernImage *self);

  GList *
  (*get_property_id_list)(CernImage *self);

  GList *
  (*get_property_items)(CernImage *self);

  CernImageFormat *
  (*get_raw_format)(CernImage *self);

  CernSizeF
  (*get_size)(CernImage *self);

  gfloat
  (*get_vertical_resolution)(CernImage *self);

  /* Functions */

  CernImage *
  (*clone)(CernImage *self);

  CernRectangleF
  (*get_bounds)(CernImage *self, CernGraphicsUnit unit);

  CernEncoderParameters *
  (*get_encoder_parameter_list)(CernImage *self, GUID const *encoder);

  guint32
  (*get_frame_count)(CernImage *self, CernFrameDimension *dimension);

  CernPropertyItem *
  (*get_property_item)(CernImage *self, gint32 property_id);

  CernImage *
  (*get_thumbnail)(CernImage *self, gint32 thumb_width, gint32 thumb_height);

  void
  (*remove_property_item)(CernImage *self, gint32 property_id);

  void
  (*rotate_flip)(CernImage *self, CernRotateFlipType flip_type);

  void
  (*save_self)(CernImage *self, CernImageFormat *format);

  void
  (*save)(CernImage *self, gchar const *filename, CernImageFormat *format);

  void
  (*save_to_stream)(CernImage *self, GOutputStream *stream);

  void
  (*set_property_item)(CernImage *self, CernPropertyItem *item);
};

CernImage *
cern_iamge_create_from_hbitmap(gpointer h_bitmap);

CernImage *
cern_image_create_from_hicon(gpointer h_icon);

CernImage *
cern_image_create_from_hicon_with_size(gpointer h_icon, gint32 width, gint32 height);

CernImage *
cern_image_create_from_hbitmap(gpointer h_bitmap);

CernImage *
cern_image_create_from_file(gchar const *filename);

gint32
cern_image_get_flags(CernImage *self);

GList *
cern_image_get_frame_dimension_list(CernImage *self);

guint32
cern_image_get_height(CernImage *self);

guint32
cern_image_get_width(CernImage *self);

gfloat
cern_image_get_resolution(CernImage *self);

CernColorPalette *
cern_image_get_palette(CernImage *self);

void
cern_image_set_palette(CernImage *self, CernColorPalette *palette);

gfloat
cern_image_get_physical_dimension(CernImage *self);

CernPixelFormat
cern_image_get_pixel_format(CernImage *self);

GList *
cern_image_get_property_id_list(CernImage *self);

GList *
cern_image_get_property_items(CernImage *self);

CernImageFormat *
cern_image_get_raw_format(CernImage *self);

CernSizeF
cern_image_get_size(CernImage *self);

gfloat
cern_image_get_vertical_resolution(CernImage *self);

CernImage *
cern_image_clone(CernImage *self);

CernRectangleF
cern_image_get_bounds(CernImage *self, CernGraphicsUnit unit);

CernEncoderParameters *
cern_image_get_encoder_parameter_list(CernImage *self, GUID const *encoder);

guint32
cern_image_get_frame_count(CernImage *self, CernFrameDimension *dimension);

CernPropertyItem *
cern_image_get_property_item(CernImage *self, gint32 property_id);

CernImage *
cern_image_get_thumbnail(CernImage *self, gint32 thumb_width, gint32 thumb_height);

void
cern_image_remove_property_item(CernImage *self, gint32 property_id);

void
cern_image_rotate_flip(CernImage *self, CernRotateFlipType flip_type);

void
cern_image_save_self(CernImage *self, CernImageFormat *format);

void
cern_image_save(CernImage *self, gchar const *filename, CernImageFormat *format);

void
cern_image_save_to_stream(CernImage *self, GOutputStream *stream);

void
cern_image_set_property_item(CernImage *self, CernPropertyItem *item);

G_END_DECLS

#endif /* CERN_IMAGE_H */
