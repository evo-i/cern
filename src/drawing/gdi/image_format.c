#include "cern/drawing/image_format.h"

struct _CernImageFormat {
  GObject parent_instance;
  GUID guid;
};

G_DEFINE_FINAL_TYPE(CernImageFormat, cern_image_format, G_TYPE_OBJECT)

static
CernImageFormat *
cern_image_format_new(const GUID *guid) {
  CernImageFormat *self;
  self = g_object_new(CERN_TYPE_IMAGE_FORMAT, NULL);
  self->guid = *guid;
  return self;
}

static
const GUID *
cern_image_format_get_guid_real(CernImageFormat *self) {
  return &self->guid;
}

static
void
cern_image_format_init(CernImageFormat *self) { }

static
void
cern_image_format_class_init(CernImageFormatClass *klass) { };

CernImageFormat *
cern_image_format_create(const GUID *guid) {
  return cern_image_format_new(guid);
}

GUID const *
cern_image_format_get_guid(CernImageFormat *self) {
  return cern_image_format_get_guid_real(self);
}

#define CERN_IMAGE_GUID_MEM_BMP 0xb96b3caa
#define CERN_IMAGE_GUID_BMP     0xb96b3cab
#define CERN_IMAGE_GUID_EMF     0xb96b3cac
#define CERN_IMAGE_GUID_WMF     0xb96b3cad
#define CERN_IMAGE_GUID_JPEG    0xb96b3cae
#define CERN_IMAGE_GUID_PNG     0xb96b3caf
#define CERN_IMAGE_GUID_GIF     0xb96b3cb0
#define CERN_IMAGE_GUID_TIFF    0xb96b3cb1
#define CERN_IMAGE_GUID_EXIF    0xb96b3cb2
#define CERN_IMAGE_GUID_ICON    0xb96b3cb5

#define CERN_IMAGE_GUID_TAIL \
0x0728, 0x11d3, { 0x9d, 0x7b, 0x00, 0x00, 0xf8, 0x1e, 0xf3, 0x2e }

gchar const *
cern_image_format_get_name(CernImageFormat *self) {
  const GUID *guid
    = cern_image_format_get_guid(self);
  switch (guid->Data1) {
    case CERN_IMAGE_GUID_MEM_BMP:
      return "Memory BMP";
    case CERN_IMAGE_GUID_BMP:
      return "BMP";
    case CERN_IMAGE_GUID_EMF:
      return "EMF";
    case CERN_IMAGE_GUID_WMF:
      return "WMF";
    case CERN_IMAGE_GUID_JPEG:
      return "JPEG";
    case CERN_IMAGE_GUID_PNG:
      return "PNG";
    case CERN_IMAGE_GUID_GIF:
      return "GIF";
    case CERN_IMAGE_GUID_TIFF:
      return "TIFF";
    case CERN_IMAGE_GUID_EXIF:
      return "EXIF";
    case CERN_IMAGE_GUID_ICON:
      return "ICON";
    default:
      return "Unknown";
  }
  return "Unknown";
}

CernImageFormat *
cern_image_format_memory_bmp(void) {
  /* b96b3caa-0728-11d3-9d7b-0000f81ef32e */
  static const GUID guid = { CERN_IMAGE_GUID_MEM_BMP, CERN_IMAGE_GUID_TAIL };

  static CernImageFormat *cached_format = NULL;
  if (!cached_format) {
    cached_format = cern_image_format_create(&guid);
  }
  return cached_format;
}

CernImageFormat *
cern_image_format_bmp(void) {
  /* b96b3cab-0728-11d3-9d7b-0000f81ef32e */
  static const GUID guid = { CERN_IMAGE_GUID_BMP, CERN_IMAGE_GUID_TAIL };

  static CernImageFormat *cached_format = NULL;
  if (!cached_format) {
    cached_format = cern_image_format_create(&guid);
  }
  return cached_format;
}

CernImageFormat *
cern_image_format_emf(void) {
  /* b96b3cac-0728-11d3-9d7b-0000f81ef32e */
  static const GUID guid = { CERN_IMAGE_GUID_EMF, CERN_IMAGE_GUID_TAIL };

  static CernImageFormat *cached_format = NULL;
  if (!cached_format) {
    cached_format = cern_image_format_create(&guid);
  }
  return cached_format;
}

CernImageFormat *
cern_image_format_wmf(void) {
  /* b96b3cad-0728-11d3-9d7b-0000f81ef32e */
  static const GUID guid = { CERN_IMAGE_GUID_WMF, CERN_IMAGE_GUID_TAIL };
  static CernImageFormat *cached_format = NULL;
  if (!cached_format) {
    cached_format = cern_image_format_create(&guid);
  }
  return cached_format;
}

CernImageFormat *
cern_image_format_jpeg(void) {
  /* b96b3cae-0728-11d3-9d7b-0000f81ef32e */
  static const GUID guid = { CERN_IMAGE_GUID_JPEG, CERN_IMAGE_GUID_TAIL };
 static CernImageFormat *cached_format = NULL;
  if (!cached_format) {
    cached_format = cern_image_format_create(&guid);
  }
  return cached_format;
}

CernImageFormat *
cern_image_format_png(void) {
  /* b96b3caf-0728-11d3-9d7b-0000f81ef32e */
  static const GUID guid = { CERN_IMAGE_GUID_PNG, CERN_IMAGE_GUID_TAIL };
  static CernImageFormat *cached_format = NULL;
  if (!cached_format) {
    cached_format = cern_image_format_create(&guid);
  }
  return cached_format;
}

CernImageFormat *
cern_image_format_gif(void) {
  /* b96b3cb0-0728-11d3-9d7b-0000f81ef32e */
  static const GUID guid = { CERN_IMAGE_GUID_GIF, CERN_IMAGE_GUID_TAIL };
  static CernImageFormat *cached_format = NULL;
  if (!cached_format) {
    cached_format = cern_image_format_create(&guid);
  }
  return cached_format;
}

CernImageFormat *
cern_image_format_tiff(void) {
  /* b96b3cb1-0728-11d3-9d7b-0000f81ef32e */
  static const GUID guid = { CERN_IMAGE_GUID_TIFF, CERN_IMAGE_GUID_TAIL };
  static CernImageFormat *cached_format = NULL;
  if (!cached_format) {
    cached_format = cern_image_format_create(&guid);
  }
  return cached_format;
}

CernImageFormat *
cern_image_format_exif(void) {
  /* b96b3cb2-0728-11d3-9d7b-0000f81ef32e */
  static const GUID guid = { CERN_IMAGE_GUID_EXIF, CERN_IMAGE_GUID_TAIL };
  static CernImageFormat *cached_format = NULL;
  if (!cached_format) {
    cached_format = cern_image_format_create(&guid);
  }
  return cached_format;
}

CernImageFormat *
cern_image_format_icon(void) {
  /* b96b3cb5-0728-11d3-9d7b-0000f81ef32e */
  static const GUID guid = { CERN_IMAGE_GUID_ICON, CERN_IMAGE_GUID_TAIL };
  static CernImageFormat *cached_format = NULL;
  if (!cached_format) {
    cached_format = cern_image_format_create(&guid);
  }
  return cached_format;
}
