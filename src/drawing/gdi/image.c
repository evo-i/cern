#include "cern/drawing/image.h"
#include "cern/drawing/size.h"
#include "cern/drawing/size_f.h"
#include "cern/drawing/rectangle_f.h"

#include "cern/drawing/internal/native_gdi_object.h"

#include <windows.h>
#include <gdiplus.h>
#include <wchar.h>

struct _CernImagePrivate {
  gpointer image;
} typedef CernImagePrivate;

static
void
cern_image_set_native(CernImage *self, gpointer ptr);

static
gpointer
cern_image_get_native(CernImage *self);

static
inline
gpointer
cern_image_get_instance_private(CernImage *self);

/* TODO: CernNativeGdiObject will be replaced with CernHandle... */

static
void
cern_image_native_gdi_object_interface_init(CernNativeGdiObjectInterface *iface) {
  iface->get_native_handle
    = (gpointer(*)(CernNativeGdiObject *)) cern_image_get_native;
  iface->set_native_handle
    = (void(*)(CernNativeGdiObject *, gpointer)) cern_image_set_native;
}

static
gpointer
cern_image_get_(gpointer handle) {
  CernImagePrivate *priv;

  if (!CERN_IS_IMAGE(handle)) {
    return NULL;
  }
  priv = cern_image_get_instance_private(CERN_IMAGE(handle));

  return priv->image;
}

static
void
cern_image_set_(gpointer handle, gpointer value) {
  CernImagePrivate *priv;

  if (!CERN_IS_IMAGE(handle)) {
    return;
  }

  priv = cern_image_get_instance_private(CERN_IMAGE(handle));
  priv->image = value;
}

G_DEFINE_TYPE_WITH_CODE(CernImage,
  cern_image,
  G_TYPE_OBJECT,
  G_ADD_PRIVATE(CernImage)
  G_IMPLEMENT_INTERFACE(CERN_TYPE_NATIVE_GDI_OBJECT,
                        cern_image_native_gdi_object_interface_init))

static void
cern_image_init(CernImage *self) {
  // Initialization code here
}

enum {
  PROP_0,
  PROP_FLAGS,
  PROP_FRAME_DIMENSION_LIST,
  PROP_HEIGHT,
  PROP_WIDTH,
  PROP_RESOLUTION,
  PROP_PALETTE,
  PROP_PHYSICAL_DIMENSION,
  PROP_PIXEL_FORMAT,
  PROP_PROPERTY_ID_LIST,
  PROP_PROPERTY_ITEMS,
  PROP_RAW_FORMAT,
  PROP_SIZE,
  PROP_VERTICAL_RESOLUTION,
  N_PROPERTIES
};

static GParamSpec *obj_properties[N_PROPERTIES] = { NULL, };

static void
cern_image_finalize(GObject *object) {
  CernImage *self = CERN_IMAGE(object);

  if (cern_image_get_native(self)) {
    GdipDisposeImage(cern_image_get_native(self));
    cern_image_set_native(self, NULL);
  }

  G_OBJECT_CLASS(cern_image_parent_class)->finalize(object);
}

static void
cern_image_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec) {
  CernImage *self = CERN_IMAGE(object);

  switch (property_id) {
    case PROP_FLAGS:
      g_value_set_int(value, cern_image_get_flags(self));
      break;
    case PROP_FRAME_DIMENSION_LIST:
      g_value_set_pointer(value, cern_image_get_frame_dimension_list(self));
      break;
    case PROP_HEIGHT:
      g_value_set_uint(value, cern_image_get_height(self));
      break;
    case PROP_WIDTH:
      g_value_set_uint(value, cern_image_get_width(self));
      break;
    case PROP_RESOLUTION:
      g_value_set_float(value, cern_image_get_resolution(self));
      break;
    case PROP_PALETTE:
      g_value_set_pointer(value, cern_image_get_palette(self));
      break;
    case PROP_PHYSICAL_DIMENSION:
      g_value_set_float(value, cern_image_get_physical_dimension(self));
      break;
    case PROP_PIXEL_FORMAT:
      g_value_set_enum(value, cern_image_get_pixel_format(self));
      break;
    case PROP_PROPERTY_ID_LIST:
      g_value_set_pointer(value, cern_image_get_property_id_list(self));
      break;
    case PROP_PROPERTY_ITEMS:
      g_value_set_pointer(value, cern_image_get_property_items(self));
      break;
    case PROP_RAW_FORMAT:
      g_value_set_pointer(value, cern_image_get_raw_format(self));
      break;
    case PROP_SIZE: {
      CernSizeF size = cern_image_get_size(self);
      CernSizeF *copy = cern_size_f_copy(&size);
      g_value_take_boxed(value, copy);
    } break;
    case PROP_VERTICAL_RESOLUTION:
      g_value_set_float(value, cern_image_get_vertical_resolution(self));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
      break;
  }
}

static void
cern_image_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec) {
  CernImage *self = CERN_IMAGE(object);

  switch (property_id) {
    case PROP_PALETTE:
      cern_image_set_palette(self, g_value_get_pointer(value));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
      break;
  }
}

static
CernImage *
cern_image_new(void) {
  return g_object_new(CERN_TYPE_IMAGE, NULL);
}

static gint32
cern_image_real_get_flags(CernImage *self) {
  guint32 flags;

  flags = 0;

  GdipGetImageFlags(cern_image_get_native(self), &flags);

  return flags;
}

static GList *
cern_image_real_get_frame_dimension_list(CernImage *self) {
  g_critical("cern_image_real_get_frame_dimension_list() not implemented");
  return NULL;
}

static guint32
cern_image_real_get_height(CernImage *self) {
  guint32 height;
  GpStatus status;

  status = GdipGetImageHeight(cern_image_get_native(self), &height);

  if (status == Ok) {
    return height;
  }

  return 0u;
}

static guint32
cern_image_real_get_width(CernImage *self) {
  guint32 height;
  GpStatus status;

  status
    = GdipGetImageWidth(cern_image_get_native(self), &height);

  if (status == Ok) {
    return height;
  }

  return 0u;
}

static gfloat
cern_image_real_get_resolution(CernImage *self) {
  gfloat resolution;
  GpStatus status;

  status
    = GdipGetImageHorizontalResolution(cern_image_get_native(self), &resolution);

  if (status == Ok) {
    return resolution;
  }

  return 0.0;
}

static CernColorPalette *
cern_image_real_get_palette(CernImage *self) {
  g_critical("cern_image_real_get_palette() not implemented");
  return NULL;
}

static void
cern_image_real_set_palette(CernImage *self, CernColorPalette *palette) {
  g_critical("cern_image_real_set_palette() not implemented");
}

static gfloat
cern_image_real_get_physical_dimension(CernImage *self) {
  g_critical("cern_image_real_get_physical_dimension() not implemented");
  return 0.0;
}

static CernPixelFormat
cern_image_real_get_pixel_format(CernImage *self) {
  CernPixelFormat format;
  GpStatus status;
  PixelFormat *ptr_format;

  ptr_format = (PixelFormat *) &format;

  status
    = GdipGetImagePixelFormat(cern_image_get_native(self), ptr_format);

  if (status == Ok) {
    return format;
  }

  return CernPixelFormat_Undefined;
}

static GList *
cern_image_real_get_property_id_list(CernImage *self) {
  GpStatus status;
  GList *property_id_list;

  //status = GdipGetPropertyIdList(cern_image_get_native(self), &property_id_list);

  g_critical("cern_image_real_get_property_id_list() is not implemented");

  return NULL;
}

static GList *
cern_image_real_get_property_items(CernImage *self) {
  g_critical("cern_image_real_get_property_items() is not implemented");
  return NULL;
}

static CernImageFormat *
cern_image_real_get_raw_format(CernImage *self) {
  CernImageFormat *format;
  GpStatus status;
  GUID guid_format;

  status = GdipGetImageRawFormat(cern_image_get_native(self), &guid_format);

  if (status == Ok) {
    format = cern_image_format_create(&guid_format);
    return format;
  }

  return NULL;
}

static
CernSizeF
cern_image_real_get_size(CernImage *self) {
  return
    cern_size_f_create(cern_image_get_width(self), cern_image_get_height(self));
}

static
gfloat
cern_image_real_get_vertical_resolution(CernImage *self) {
  gfloat resolution;
  GpStatus status;

  status
    = GdipGetImageVerticalResolution(cern_image_get_native(self), &resolution);

  if (status == Ok) {
    return resolution;
  }

  g_warning("cern_image_real_get_vertical_resolution: failed\n");

  return 0.0;
}

static CernImage *
cern_image_real_clone(CernImage *self) {
  CernImagePrivate *clone_priv;
  GpImage *clone;
  GpStatus status;

  status = GdipCloneImage(cern_image_get_native(self), &clone);

  if (status == Ok) {
    CernImage *clone_image = cern_image_new();
    clone_priv = cern_image_get_instance_private(clone_image);
    cern_image_set_native(clone_image, clone);
    return clone_image;
  }

  return NULL;
}

static
CernRectangleF
cern_image_real_get_bounds(CernImage *self, CernGraphicsUnit unit) {
  CernRectangleF rect;
  GpStatus status;

  GpRectF rect_f = { 0 };

  status
    = GdipGetImageBounds(cern_image_get_native(self), &rect_f, (GpUnit *) &unit);

  if (status == Ok) {
    rect
      = cern_rectangle_f_create_with_rect(rect_f.X, rect_f.Y,
                                       rect_f.Width, rect_f.Height);
    return rect;
  }

  return (CernRectangleF) { 0 };
}

static
CernEncoderParameters *
cern_image_real_get_encoder_parameter_list(CernImage *self, GUID const *encoder) {
  g_critical("Not implemented yet");
  return NULL;
}

static guint32
cern_image_real_get_frame_count(CernImage *self, CernFrameDimension *dimension) {
  guint32 result;
  GpStatus status;
  GUID const *guid;

  guid = cern_frame_dimension_get_guid(dimension);

  status = GdipImageGetFrameCount(cern_image_get_native(self), guid, &result);

  return result;
}

static
CernPropertyItem *
cern_image_real_get_property_item(CernImage *self, gint32 property_id) {
  g_critical("Not implemented yet");
  return NULL;
}

static
CernImage *
cern_image_real_get_thumbnail(CernImage *self, gint32 thumb_width, gint32 thumb_height) {
  CernImage *thumb_image;
  GpStatus status;
  GpImage *thumb;

  status
    = GdipGetImageThumbnail(cern_image_get_native(self),
                            thumb_width, thumb_height, &thumb, NULL, NULL);

  if (status == Ok) {
    thumb_image = cern_image_new();
    cern_image_set_native(thumb_image, thumb);
    return thumb_image;
  }

  return NULL;
}

static void
cern_image_real_remove_property_item(CernImage *self, gint32 property_id) {
  GdipRemovePropertyItem(cern_image_get_native(self), property_id);
}

static void
cern_image_real_rotate_flip(CernImage *self, CernRotateFlipType flip_type) {
  GpStatus status;

  status = GdipImageRotateFlip(cern_image_get_native(self), (RotateFlipType) flip_type);

  if (status == Ok) {
    return;
  }

  g_printerr("[%p] Error rotating image\n", cern_image_get_native(self));
}

static void
cern_image_real_save_self(CernImage *self, CernImageFormat *format) {
  // Implementation here
  g_critical("Not implemented yet");
}

static void
cern_image_real_save(CernImage *self, gchar const *filename, CernImageFormat *format) {
  g_critical("Not implemented yet");
}

static void
cern_image_real_save_to_stream(CernImage *self, GOutputStream *stream) {
  g_critical("Not implemented yet");
}

static void
cern_image_real_set_property_item(CernImage *self, CernPropertyItem *item) {
  PropertyItem property_item = {
    .id = cern_property_item_id(item),
    .type = cern_property_item_type(item),
    .length = cern_property_item_length(item),
    .value = cern_property_item_value(item)->data,
  };

  GdipSetPropertyItem(cern_image_get_native(self), &property_item);
}

static void
cern_image_class_init(CernImageClass *klass) {
  GObjectClass *object_class = G_OBJECT_CLASS(klass);

  object_class->finalize = cern_image_finalize;
  object_class->get_property = cern_image_get_property;
  object_class->set_property = cern_image_set_property;

  klass->get_flags = cern_image_real_get_flags;
  klass->get_frame_dimension_list = cern_image_real_get_frame_dimension_list;
  klass->get_height = cern_image_real_get_height;
  klass->get_width = cern_image_real_get_width;
  klass->get_resolution = cern_image_real_get_resolution;
  klass->get_palette = cern_image_real_get_palette;
  klass->set_palette = cern_image_real_set_palette;
  klass->get_physical_dimension = cern_image_real_get_physical_dimension;
  klass->get_pixel_format = cern_image_real_get_pixel_format;
  klass->get_property_id_list = cern_image_real_get_property_id_list;
  klass->get_property_items = cern_image_real_get_property_items;
  klass->get_raw_format = cern_image_real_get_raw_format;
  klass->get_size = cern_image_real_get_size;
  klass->get_vertical_resolution = cern_image_real_get_vertical_resolution;
  klass->clone = cern_image_real_clone;
  klass->get_bounds = cern_image_real_get_bounds;
  klass->get_encoder_parameter_list = cern_image_real_get_encoder_parameter_list;
  klass->get_frame_count = cern_image_real_get_frame_count;
  klass->get_property_item = cern_image_real_get_property_item;
  klass->get_thumbnail = cern_image_real_get_thumbnail;
  klass->remove_property_item = cern_image_real_remove_property_item;
  klass->rotate_flip = cern_image_real_rotate_flip;
  klass->save_self = cern_image_real_save_self;
  klass->save = cern_image_real_save;
  klass->save_to_stream = cern_image_real_save_to_stream;
  klass->set_property_item = cern_image_real_set_property_item;

  obj_properties[PROP_FLAGS] = g_param_spec_int(
    "flags",
    "Flags",
    "Image flags",
    G_MININT,
    G_MAXINT,
    0,
    G_PARAM_READABLE
  );

  obj_properties[PROP_FRAME_DIMENSION_LIST] = g_param_spec_pointer(
    "frame-dimension-list",
    "Frame Dimension List",
    "List of frame dimensions",
    G_PARAM_READABLE
  );

  obj_properties[PROP_HEIGHT] = g_param_spec_int(
    "height",
    "Height",
    "Image height",
    G_MININT,
    G_MAXINT,
    0,
    G_PARAM_READABLE
  );

  obj_properties[PROP_WIDTH] = g_param_spec_int(
    "width",
    "Width",
    "Image width",
    G_MININT,
    G_MAXINT,
    0,
    G_PARAM_READABLE
  );

  obj_properties[PROP_RESOLUTION] = g_param_spec_float(
    "resolution",
    "Resolution",
    "Image resolution",
    -G_MAXFLOAT,
    G_MAXFLOAT,
    0.0,
    G_PARAM_READABLE
  );

  obj_properties[PROP_PALETTE] = g_param_spec_pointer(
    "palette",
    "Palette",
    "Image color palette",
    G_PARAM_READWRITE
  );

  obj_properties[PROP_PHYSICAL_DIMENSION] = g_param_spec_float(
    "physical-dimension",
    "Physical Dimension",
    "Physical dimension of the image",
    -G_MAXFLOAT,
    G_MAXFLOAT,
    0.0,
    G_PARAM_READABLE
  );

  obj_properties[PROP_PIXEL_FORMAT] = g_param_spec_int(
    "pixel-format",
    "Pixel Format",
    "Image pixel format",
    G_MININT32, G_MAXINT32,
    0,
    G_PARAM_READABLE
  );

  obj_properties[PROP_PROPERTY_ID_LIST] = g_param_spec_pointer(
    "property-id-list",
    "Property ID List",
    "List of property IDs",
    G_PARAM_READABLE
  );

  obj_properties[PROP_PROPERTY_ITEMS] = g_param_spec_pointer(
    "property-items",
    "Property Items",
    "List of property items",
    G_PARAM_READABLE
  );

  obj_properties[PROP_RAW_FORMAT] = g_param_spec_pointer(
    "raw-format",
    "Raw Format",
    "Raw image format",
    G_PARAM_READABLE
  );

  obj_properties[PROP_SIZE] = g_param_spec_pointer(
    "size",
    "Size",
    "Image size",
    G_PARAM_READABLE
  );

  obj_properties[PROP_VERTICAL_RESOLUTION] = g_param_spec_float(
    "vertical-resolution",
    "Vertical Resolution",
    "Vertical resolution of the image",
    -G_MAXFLOAT,
    G_MAXFLOAT,
    0.0,
    G_PARAM_READABLE
  );

  g_object_class_install_properties(object_class, N_PROPERTIES, obj_properties);
}

CernImage *
cern_image_create_from_hbitmap(gpointer h_bitmap) {
  CernImage *self;

  g_critical("cern_image_create_from_hbitmap() is not implemented");

  return NULL;
}


CernImage *
cern_image_create_from_file(gchar const *filename) {
  CernImage *self;

  GpImage *image = NULL;

  wchar_t *filename_w = g_utf8_to_utf16(filename, -1, NULL, NULL, NULL);
  GpStatus status = GdipLoadImageFromFile(filename_w, &image);
  g_free(filename_w);

  self = g_object_new(CERN_TYPE_IMAGE, NULL);

  // status = GdipImageForceValidation(priv->image);

  cern_image_set_native(self, image);
  return self;
}

CernImage *
cern_image_create_from_hicon(gpointer h_icon) {
  g_critical("cern_image_create_from_hicon() is not implemented");
  return NULL;
}

CernImage *
cern_image_create_from_hicon_with_size(gpointer h_icon, gint32 width, gint32 height) {
  g_critical("cern_image_create_from_hicon_with_size() is not implemented");
  return NULL;
}

gint32
cern_image_get_flags(CernImage *self) {
  CernImageClass *klass = CERN_IMAGE_GET_CLASS(self);
  if (klass->get_flags) {
    return klass->get_flags(self);
  }
  return 0;
}

GList *
cern_image_get_frame_dimension_list(CernImage *self) {
  CernImageClass *klass = CERN_IMAGE_GET_CLASS(self);
  if (klass->get_frame_dimension_list) {
    return klass->get_frame_dimension_list(self);
  }
  return NULL;
}

guint32
cern_image_get_height(CernImage *self) {
  CernImageClass *klass = CERN_IMAGE_GET_CLASS(self);
  if (klass->get_height) {
    return klass->get_height(self);
  }
  return 0;
}

guint32
cern_image_get_width(CernImage *self) {
  CernImageClass *klass = CERN_IMAGE_GET_CLASS(self);
  if (klass->get_width) {
    return klass->get_width(self);
  }
  return 0;
}

gfloat
cern_image_get_resolution(CernImage *self) {
  CernImageClass *klass = CERN_IMAGE_GET_CLASS(self);
  if (klass->get_resolution) {
    return klass->get_resolution(self);
  }
  return 0.0;
}

CernColorPalette *
cern_image_get_palette(CernImage *self) {
  CernImageClass *klass = CERN_IMAGE_GET_CLASS(self);
  if (klass->get_palette) {
    return klass->get_palette(self);
  }
  return NULL;
}

void
cern_image_set_palette(CernImage *self, CernColorPalette *palette) {
  CernImageClass *klass = CERN_IMAGE_GET_CLASS(self);
  if (klass->set_palette) {
    klass->set_palette(self, palette);
  }
}

gfloat
cern_image_get_physical_dimension(CernImage *self) {
  CernImageClass *klass = CERN_IMAGE_GET_CLASS(self);
  if (klass->get_physical_dimension) {
    return klass->get_physical_dimension(self);
  }
  return 0.0;
}

CernPixelFormat
cern_image_get_pixel_format(CernImage *self) {
  CernImageClass *klass = CERN_IMAGE_GET_CLASS(self);
  if (klass->get_pixel_format) {
    return klass->get_pixel_format(self);
  }
  return 0;
}

GList *
cern_image_get_property_id_list(CernImage *self) {
  CernImageClass *klass = CERN_IMAGE_GET_CLASS(self);
  if (klass->get_property_id_list) {
    return klass->get_property_id_list(self);
  }
  return NULL;
}

GList *
cern_image_get_property_items(CernImage *self) {
  CernImageClass *klass = CERN_IMAGE_GET_CLASS(self);
  if (klass->get_property_items) {
    return klass->get_property_items(self);
  }

  return NULL;
}

CernImageFormat *
cern_image_get_raw_format(CernImage *self) {
  CernImageClass *klass = CERN_IMAGE_GET_CLASS(self);
  if (klass->get_raw_format) {
    return klass->get_raw_format(self);
  }
  return NULL;
}

CernSizeF
cern_image_get_size(CernImage *self) {
  CernImageClass *klass = CERN_IMAGE_GET_CLASS(self);
  if (klass->get_size) {
    return klass->get_size(self);
  }

  return (CernSizeF) { 0 };
}

gfloat
cern_image_get_vertical_resolution(CernImage *self) {
  CernImageClass *klass = CERN_IMAGE_GET_CLASS(self);
  if (klass->get_vertical_resolution) {
    return klass->get_vertical_resolution(self);
  }
  return 0.0;
}

CernImage *
cern_image_clone(CernImage *self) {
  CernImageClass *klass = CERN_IMAGE_GET_CLASS(self);
  if (klass->clone) {
    return klass->clone(self);
  }
  return NULL;
}

CernRectangleF
cern_image_get_bounds(CernImage *self, CernGraphicsUnit unit) {
  CernImageClass *klass = CERN_IMAGE_GET_CLASS(self);
  if (klass->get_bounds) {
    return klass->get_bounds(self, unit);
  }
  return (CernRectangleF) { 0 };
}

CernEncoderParameters *
cern_image_get_encoder_parameter_list(CernImage *self, GUID const *encoder) {
  CernImageClass *klass = CERN_IMAGE_GET_CLASS(self);
  if (klass->get_encoder_parameter_list) {
    return klass->get_encoder_parameter_list(self, encoder);
  }
  return NULL;
}

guint32
cern_image_get_frame_count(CernImage *self, CernFrameDimension *dimension) {
  CernImageClass *klass = CERN_IMAGE_GET_CLASS(self);
  if (klass->get_frame_count) {
    return klass->get_frame_count(self, dimension);
  }
  return 0;
}

CernPropertyItem *
cern_image_get_property_item(CernImage *self, gint32 property_id) {
  CernImageClass *klass = CERN_IMAGE_GET_CLASS(self);
  if (klass->get_property_item) {
    return klass->get_property_item(self, property_id);
  }
  return NULL;
}

CernImage *
cern_image_get_thumbnail(CernImage *self, gint32 thumb_width, gint32 thumb_height) {
  CernImageClass *klass = CERN_IMAGE_GET_CLASS(self);
  if (klass->get_thumbnail) {
    return klass->get_thumbnail(self, thumb_width, thumb_height);
  }
  return NULL;
}

void
cern_image_remove_property_item(CernImage *self, gint32 property_id) {
  CernImageClass *klass = CERN_IMAGE_GET_CLASS(self);
  if (klass->remove_property_item) {
    klass->remove_property_item(self, property_id);
  }
}

void
cern_image_rotate_flip(CernImage *self, CernRotateFlipType flip_type) {
  CernImageClass *klass = CERN_IMAGE_GET_CLASS(self);
  if (klass->rotate_flip) {
    klass->rotate_flip(self, flip_type);
  }
}

void
cern_image_save_self(CernImage *self, CernImageFormat *format) {
  CernImageClass *klass = CERN_IMAGE_GET_CLASS(self);
  if (klass->save_self) {
    klass->save_self(self, format);
  }
}

void
cern_image_save(CernImage *self, gchar const *filename, CernImageFormat *format) {
  CernImageClass *klass = CERN_IMAGE_GET_CLASS(self);
  if (klass->save) {
    klass->save(self, filename, format);
  }
}

void
cern_image_save_to_stream(CernImage *self, GOutputStream *stream) {
  CernImageClass *klass = CERN_IMAGE_GET_CLASS(self);
  if (klass->save_to_stream) {
    klass->save_to_stream(self, stream);
  }
}

void
cern_image_set_property_item(CernImage *self, CernPropertyItem *item) {
  CernImageClass *klass = CERN_IMAGE_GET_CLASS(self);
  if (klass->set_property_item) {
    klass->set_property_item(self, item);
  }
}

void
cern_image_set_native(CernImage *self, gpointer ptr) {
  CernImagePrivate *priv;
  priv = cern_image_get_instance_private(self);
  priv->image = ptr;
}

gpointer
cern_image_get_native(CernImage *self) {
  CernImagePrivate *priv;
  priv = cern_image_get_instance_private(self);
  return priv->image;
}
