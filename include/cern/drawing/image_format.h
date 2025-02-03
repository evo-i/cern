#ifndef CERN_IMAGE_FORMAT_H
#define CERN_IMAGE_FORMAT_H 1

#include <glib-object.h>
#include <guiddef.h>

G_BEGIN_DECLS

#define CERN_TYPE_IMAGE_FORMAT (cern_image_format_get_type())

G_DECLARE_FINAL_TYPE(CernImageFormat, cern_image_format, CERN, IMAGE_FORMAT, GObject)

struct _CernImageFormatClass {
  GObjectClass parent_class;

  GUID const *
  (*get_guid)(CernImageFormat *self);

  gchar const *
  (*get_name)(CernImageFormat *self);
};

CernImageFormat *
cern_image_format_create(const GUID *guid);

GUID const *
cern_image_format_get_guid(CernImageFormat *self);

gchar const *
cern_image_format_get_name(CernImageFormat *self);

CernImageFormat *
cern_image_format_memory_bmp(void);

CernImageFormat *
cern_image_format_bmp(void);

CernImageFormat *
cern_image_format_emf(void);

CernImageFormat *
cern_image_format_wmf(void);

CernImageFormat *
cern_image_format_gif(void);

CernImageFormat *
cern_image_format_jpeg(void);

CernImageFormat *
cern_image_format_png(void);

CernImageFormat *
cern_image_format_tiff(void);

CernImageFormat *
cern_image_format_exif(void);

CernImageFormat *
cern_image_format_icon(void);

G_END_DECLS

#endif /* CERN_IMAGE_FORMAT_H */
