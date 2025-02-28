#include "cern/drawing/icon.h"
#include "cern/core/icloneable.h"
#include "cern/drawing/rectangle.h"
#include "cern/drawing/internal/native_gdi_object.h"

#include <unknwn.h>

#include "cern/drawing/size.h"
#include "cern/drawing/point.h"
#include "cern/drawing/rectangle.h"
#include "cern/drawing/graphics.h"

#include <Windows.h>
#include <gdiplus.h>
#include <assert.h>
#include <wingdi.h>
#include <shlwapi.h>

#if defined(_MSC_VER)
  #define PACKED_STRUCT(decl) __pragma(pack(push, 1)) decl __pragma(pack(pop))
#elif defined(__GNUC__) || defined(__clang__)
  #define PACKED_STRUCT(decl) decl __attribute__((packed))
#else
  #error "Unsupported compiler"
#endif

PACKED_STRUCT(struct {
  BYTE  bWidth;          // Width, in pixels, of the image
  BYTE  bHeight;         // Height, in pixels, of the image
  BYTE  bColorCount;     // Number of colors in image (0 if >=8bpp)
  BYTE  bReserved;       // Reserved ( must be 0)
  WORD  wPlanes;         // Color Planes
  WORD  wBitCount;       // Bits per pixel
  DWORD dwBytesInRes;    // How many bytes in this resource?
  DWORD dwImageOffset;   // Where in the file is this image?
}) typedef ICONDIRENTRY, *LPICONDIRENTRY;

PACKED_STRUCT(struct {
  WORD          idReserved;   // Reserved (must be 0)
  WORD          idType;       // Resource Type (1 for icons)
  WORD          idCount;      // How many images?
  ICONDIRENTRY  idEntries[1]; // An entry for each image (idCount of 'em)
}) typedef ICONDIR, *LPICONDIR;

#define PNG_SIGNATURE   (137 + ('P' << 8) + ('N' << 16) + ('G' << 24))
#define PNG_SIGNATURE2  (13 + (10 << 8) + (26 << 16) + (10 << 24))

static
gpointer
cern_icon_save_clip_rgn(HDC dc) {
  HRGN tmp_region = CreateRectRgn(0, 0, 0, 0);
  HRGN save_region = NULL;
  gint32 result;

  result = GetClipRgn(dc, tmp_region);
  if (result > 0) {
    save_region = tmp_region;
    tmp_region = NULL;
  }

  if (tmp_region != NULL) {
    DeleteObject(tmp_region);
  }

  return (gpointer) save_region;
}

static
void
cern_icon_restore_clip_rgn(HDC dc, HRGN region) {
  SelectClipRgn((HDC) dc, region);
  if (region != NULL) {
    DeleteObject(region);
  }
}

static
guint32
cern_icon_bit_depth = 0;

struct _CernIcon {
  GObject parent_instance;

  GBytes *data;
  gpointer handle;

  guint32 best_image_offset;
  guint32 best_image_bit_depth;
  guint32 best_bytes_in_res;
  gboolean is_best_image_png;
  CernSize icon_size;
  gboolean is_own_handle;
};

static
void
cern_icon_cloneable_iface(CernICloneableInterface *iface) {
  iface->clone
    = (CernICloneable *(*)(CernICloneable *)) cern_icon_clone;
}

G_DEFINE_FINAL_TYPE_WITH_CODE(CernIcon, cern_icon, G_TYPE_OBJECT,
  G_IMPLEMENT_INTERFACE(CERN_TYPE_ICLONEABLE, cern_icon_cloneable_iface))

static
void
cern_icon_init(CernIcon *self) {
  self->is_own_handle = TRUE;
}

static
void
cern_icon_finalize(GObject *obj) {
  g_return_if_fail(CERN_IS_ICON(obj));

  CernIcon *self = CERN_ICON(obj);

  if (self->data) {
    g_bytes_unref(self->data);
    self->data = NULL;
  }

  if (self->handle && self->is_own_handle) {
    DestroyIcon(self->handle);
    self->handle = NULL;
  }
}

static
void
cern_icon_class_init(CernIconClass *klass) {
  GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
  gobject_class->finalize = cern_icon_finalize;
}

static
gboolean
cern_bitmap_has_alpha(CernBitmapData *bitmap_data) {
  gboolean has_alpha = FALSE;
  gint32 height = cern_bitmap_data_get_height(bitmap_data);
  gint32 stride = cern_bitmap_data_get_stride(bitmap_data);
  guint8 *scan0 = cern_bitmap_data_get_scan0(bitmap_data);

  for (gint32 i = 0; i < height; i++) {
    for (gint32 j = 3; j < ABS(stride); j += 4) {
      guint8 *candidate = scan0 + (i * stride) + j;
      if (*candidate != 0) {
        has_alpha = TRUE;
        goto Found;
      }
    }
  }

Found:
  return has_alpha;
}

static
void
cern_icon_copy_bitmap_data(CernBitmapData *source_data, CernBitmapData *target_data) {
  gint32 offset_src = 0;
  gint32 offset_dest = 0;
  gint32 height = MIN(cern_bitmap_data_get_height(source_data),
                      cern_bitmap_data_get_height(target_data));
  gint32 stride = MIN(ABS(cern_bitmap_data_get_stride(source_data)),
                      ABS(cern_bitmap_data_get_stride(target_data)));

  for (gint32 i = 0; i < height; i++) {
    guint8 *src_ptr = cern_bitmap_data_get_scan0(source_data) + offset_src;
    guint8 *dest_ptr = cern_bitmap_data_get_scan0(target_data) + offset_dest;

    memmove(dest_ptr, src_ptr, ABS(stride));

    offset_src += cern_bitmap_data_get_stride(source_data);
    offset_dest += cern_bitmap_data_get_stride(target_data);
  }
}

static
void
cern_icon_draw_icon(CernIcon *self, gpointer dc, CernRectangle *image_rect,
                    CernRectangle *target_rect, gboolean stretch) {
  gpointer saaved_rgn;
  guint32 image_x = 0;
  guint32 image_y = 0;
  guint32 image_width;
  guint32 image_height;
  guint32 target_x = 0;
  guint32 target_y = 0;
  guint32 target_width = 0;
  guint32 target_height = 0;
  gint32 draw_width, draw_height;
  gint32 clip_width, clip_height;
  gint32 status;

  CernSize cursor_size = cern_icon_get_size(self);

  if (!cern_rectangle_is_empty(image_rect)) {
    image_x = cern_rectangle_get_x(image_rect);
    image_y = cern_rectangle_get_y(image_rect);
    image_width = cern_rectangle_get_width(image_rect);
    image_height = cern_rectangle_get_height(image_rect);
  } else {
    image_width = cern_size_get_width(&cursor_size);
    image_height = cern_size_get_height(&cursor_size);
  }

  if (!cern_rectangle_is_empty(target_rect)) {
    target_x = cern_rectangle_get_x(target_rect);
    target_y = cern_rectangle_get_y(target_rect);
    target_width = cern_rectangle_get_width(target_rect);
    target_height = cern_rectangle_get_height(target_rect);
  } else {
    target_width = cern_size_get_width(&cursor_size);
    target_height = cern_size_get_height(&cursor_size);
  }

  if (stretch) {
    draw_width
      = cern_size_get_width(&cursor_size) * target_width / image_width;

    draw_height
      = cern_size_get_height(&cursor_size) * target_height / image_height;

    clip_width = target_width;
    clip_height = target_height;
  } else {
    draw_width = cern_size_get_width(&cursor_size);
    draw_height = cern_size_get_height(&cursor_size);
    clip_width = MIN(target_width, image_width);
    clip_height = MIN(target_height, image_height);
  }

  saaved_rgn = cern_icon_save_clip_rgn(dc);

  status
    = IntersectClipRect(dc, target_x, target_y,
                        target_x + clip_width,
                        target_y + clip_height);

  status
    = DrawIconEx(dc, target_x - image_x, target_y - image_y,
                 (HICON) self->handle, draw_width, draw_height,
                 0, NULL, DI_NORMAL);

  cern_icon_restore_clip_rgn(dc, saaved_rgn);
}

static
void
cern_icon_initialize(CernIcon **p_self, gint32 width, gint32 height) {
  CernIcon *self = (*p_self);
  gsize icon_dir_size;
  ICONDIR const *icon_dir;
  guint8 best_width = 0;
  guint8 best_height = 0;
  ICONDIRENTRY temp_entry = { 0 };

  if (self->handle != NULL
      || self->data == NULL
      || g_bytes_get_size(self->data) < sizeof(ICONDIR)) {
    g_critical("cern_icon_initialize(...): Failed to initialize CernIcon with invalid data.");
    g_clear_object(p_self);
    return;
  }

  width = width == 0 ? GetSystemMetrics(SM_CXICON) : width;
  height = height == 0 ? GetSystemMetrics(SM_CYICON) : height;

  if (cern_icon_bit_depth == 0) {
    HDC dc = GetDC(NULL);
    cern_icon_bit_depth = GetDeviceCaps(dc, BITSPIXEL);
    cern_icon_bit_depth *= GetDeviceCaps(dc, PLANES);
    ReleaseDC(NULL, dc);

    if (cern_icon_bit_depth == 8) {
      cern_icon_bit_depth = 4;
    }
  }

  icon_dir = g_bytes_get_data(self->data, NULL);

  if (icon_dir->idReserved != 0
      || icon_dir->idType != 1
      || icon_dir->idCount == 0) {
    g_critical("cern_icon_initialize(...): Failed to initialize CernIcon with invalid data.");
    g_clear_object(p_self);
    return;
  }

  if (sizeof(ICONDIRENTRY) * (icon_dir->idCount - 1) + sizeof(ICONDIR)
      > g_bytes_get_size(self->data)) {
    g_critical("cern_icon_initialize(...): Failed to initialize CernIcon Illegal number of ICONDIRENTRIES");
    g_clear_object(p_self);
    return;
  }

  ICONDIRENTRY const *icon_entries = &icon_dir->idEntries[0];

  for (gint32 i = 0; i < icon_dir->idCount; i++) {
    temp_entry = icon_entries[i];
    gint32 icon_bit_depth = temp_entry.bColorCount != 0
      ? (temp_entry.bColorCount < 0x10 ? 1 : 4)
      : temp_entry.wBitCount;

    if (icon_bit_depth == 0) {
      icon_bit_depth = 8;
    }

    gboolean update_best_fit = FALSE;

    if (self->best_bytes_in_res == 0) {
      update_best_fit = TRUE;
    } else {
      gint32 best_delta = ABS(best_width - width) + ABS(best_height - height);
      gint32 this_delta = ABS(temp_entry.bWidth - width) + ABS(temp_entry.bHeight - height);

      if ((this_delta < best_delta) ||
          (this_delta == best_delta &&
           ((icon_bit_depth <= cern_icon_bit_depth
              && icon_bit_depth > self->best_image_bit_depth) ||
            (self->best_image_bit_depth > cern_icon_bit_depth
              && icon_bit_depth < self->best_image_bit_depth)))) {
        update_best_fit = TRUE;
      }
    }

    if (update_best_fit) {
      self->best_image_offset = temp_entry.dwImageOffset;
      self->best_image_bit_depth = icon_bit_depth;
      self->best_bytes_in_res = temp_entry.dwBytesInRes;
      best_width = temp_entry.bWidth;
      best_height = temp_entry.bHeight;
    }
  }

  gsize icon_data_size = g_bytes_get_size(self->data);
  gsize end_offset = self->best_image_offset + self->best_bytes_in_res;

  if (self->best_image_offset < 0 || self->best_bytes_in_res < 0
      || end_offset > icon_data_size) {
    g_critical("cern_icon_initialize(...): Invalid offset/length for the Icon data");
    g_clear_object(p_self);
    return;
  }

  const guint8 *icon_data = g_bytes_get_data(self->data, NULL);
  self->handle = CreateIconFromResourceEx((PBYTE)(icon_data + self->best_image_offset),
                                          self->best_bytes_in_res, TRUE, 0x00030000, 0, 0, 0);

  if (self->handle == NULL) {
    g_critical("cern_icon_initialize(...): Failed to create icon from resource");
    g_clear_object(p_self);
    return;
  }

  self->icon_size.width = best_width;
  self->icon_size.height = best_height;
}

CernIcon *
cern_icon_new_from_handle(gpointer handle) {
  CernIcon *self;
  self = g_object_new(CERN_TYPE_ICON, NULL);
  self->handle = handle;
  return self;
}

CernIcon *
cern_icon_new_from_path(gchar const *path) {
  return cern_icon_new_from_path_with_wh(path, 0, 0);
}

CernIcon *
cern_icon_new_from_path_with_size(gchar const *path, CernSize *size) {
  return cern_icon_new_from_path_with_wh(path, size->width, size->height);
}

CernIcon *
cern_icon_new_from_path_with_wh(gchar const *path,
                                guint32 width,
                                guint32 height) {
  CernIcon *self;
  GError *error;
  GFile *file;
  GBytes *bytes;
  GFileInputStream *input_stream;

  error = NULL;
  self = NULL;
  file = g_file_new_for_path(path);
  bytes = g_file_load_bytes(file, NULL, NULL, &error);


  if (bytes != NULL) {
    self = cern_icon_new_from_handle(NULL);
    self->data = g_bytes_ref(bytes);
    cern_icon_initialize(&self, width, height);
  } else {
    g_critical("cern_icon_new_from_path_with_wh(): Failed to initialize icon.");
  }

  if (self != NULL
      && self->data != NULL) {
    g_bytes_unref(self->data);
  }

  g_object_unref(file);
  return self;
}

CernIcon *
cern_icon_new_from_icon_with_size(CernIcon *original, CernSize *size) {
  return
    cern_icon_new_from_icon_and_wh(original, size->width, size->height);
}

CernIcon *
cern_icon_new_from_icon_and_wh(CernIcon *original,
                               guint32 width,
                               guint32 height) {
  GBytes *bytes;
  CernIcon *self;
  gpointer handle;
  CernSize size;

  bytes = NULL;
  self = NULL;

  if (original == NULL) {
    g_critical("cern_icon_new_from_icon_and_wh(): Unable to create icon from NULL.");
    return self;
  }

  self = cern_icon_new_from_handle(NULL);
  self->data = original->data;

  if (bytes == NULL) {
    self->icon_size = original->icon_size;
    handle
      = CopyImage(original->handle, IMAGE_ICON,
                  self->icon_size.width, self->icon_size.height, 0);
  } else {
    cern_icon_initialize(&self, self->icon_size.width, self->icon_size.height);
    g_bytes_ref(self->data);
  }

  return self;
}

CernIcon *
cern_icon_extract_associated_icon(gchar const *path) {
  return cern_icon_extract_associated_icon_with_index(path, 0);
}

CernIcon *
cern_icon_extract_associated_icon_with_index(gchar const *path, gint32 index) {
  CernIcon *self;
  GError *error = NULL;
  GFile *file;
  gpointer handle;

  if (path == NULL) {
    g_critical("cern_icon_extract_associated_icon_with_index(): Invalid argument: filePath is null.");
    return NULL;
  }

  file = g_file_new_for_path(path);

  if (!g_file_query_exists(file, NULL)) {
    g_critical("cern_icon_extract_associated_icon_with_index(): File not exists.");
    return NULL;
  }

  g_object_unref(file);

  gunichar2 *wide_path = g_utf8_to_utf16(path, -1, NULL, NULL, NULL);

  handle = ExtractAssociatedIconW(NULL, wide_path, (WORD *) &index);

  g_free(wide_path);

  if (handle == NULL) {
    g_critical("cern_icon_extract_associated_icon_with_index(): ExtractAssociatedIconW() Failed: %d", GetLastError());
    return NULL;
  }

  return
    cern_icon_new_from_handle(handle);
}

gpointer
cern_icon_get_handle(CernIcon *self) {
  g_return_val_if_fail(CERN_IS_ICON(self), NULL);
  return self->handle;
}

guint32
cern_icon_get_width(CernIcon *self) {
  g_return_val_if_fail(CERN_IS_ICON(self), 0);
  return
    cern_icon_get_size(self)
      .width;
}

guint32
cern_icon_get_height(CernIcon *self) {
  g_return_val_if_fail(CERN_IS_ICON(self), 0);
  return
    cern_icon_get_size(self)
      .height;
}

CernSize
cern_icon_get_size(CernIcon *self) {
  CernSize size = { 0 };
  ICONINFO icon_info = { 0 };
  BITMAP bitmap = { 0 };

  g_return_val_if_fail(CERN_IS_ICON(self), (CernSize) { 0 });

  GetIconInfo(self->handle, &icon_info);

  if (icon_info.hbmColor != NULL) {
    GetObject(icon_info.hbmColor, sizeof(BITMAP), &bitmap);
    DeleteObject(icon_info.hbmColor);
    size = (CernSize) { bitmap.bmWidth, bitmap.bmHeight };
  } else if(icon_info.hbmMask!= NULL) {
    GetObject(icon_info.hbmMask, sizeof(BITMAP), &bitmap);
    DeleteObject(icon_info.hbmMask);
    size = (CernSize) { bitmap.bmWidth, bitmap.bmHeight };
  } else {
    if (icon_info.hbmMask != NULL) {
      DeleteObject(icon_info.hbmMask);
    }
  }

  return size;
}

gboolean
cern_icon_has_png_signature(CernIcon *self) {
  g_return_val_if_fail(CERN_IS_ICON(self), FALSE);

  if (!self->is_best_image_png) {
    if (self->data != NULL
         && g_bytes_get_size(self->data) >= self->best_image_offset + 8) {
      guint32 const *icon_signature1
        = g_bytes_get_region(self->data, sizeof(guint32), self->best_image_offset, 1);

      guint32 const *icon_signature2
        = g_bytes_get_region(self->data, sizeof(guint32), self->best_image_offset + 4, 1);

      self->is_best_image_png
        = (*icon_signature1) == PNG_SIGNATURE
            && (*icon_signature2) == PNG_SIGNATURE2;
    } else {
      self->is_best_image_png = FALSE;
    }

    return self->is_best_image_png;
  }
}

CernIcon *
cern_icon_clone(CernIcon *self) {
  g_return_val_if_fail(CERN_IS_ICLONEABLE(self), NULL);
  return NULL;
}

CernBitmap *
cern_icon_png_frame(CernIcon *self) {
  g_return_val_if_fail(CERN_IS_ICLONEABLE(self), NULL);
  GBytes *png_bytes;
  GError *error = NULL;
  CernBitmap *bitmap;
  GInputStream *input_stream;

  if (!cern_icon_has_png_signature(self)) {
    g_critical("cern_icon_png_frame(): Icon does not have PNG signature.");
    return NULL;
  }

  input_stream = g_memory_input_stream_new_from_bytes(g_bytes_ref(self->data));

  bitmap = cern_bitmap_new_from_stream(input_stream);

  if (bitmap == NULL) {
    g_critical("cern_icon_png_frame(): Failed to create CernBitmap from PNG stream.");
    g_object_unref(input_stream);
    return NULL;
  }

  g_bytes_unref(self->data);
  g_object_unref(input_stream);

  return bitmap;
}

CernBitmap *
cern_icon_bitmap_frame(CernIcon *self) {
  CernSize lock_size;
  CernPoint lock_point;
  CernRectangle lock_rectangle;

  CernBitmap *bitmap = NULL;
  CernBitmapData *bitmap_data, *lock_bitmap_data;

  g_return_val_if_fail(CERN_IS_ICON(self), NULL);

  if (self->data != NULL || self->best_image_bit_depth == 32) {
    assert(self->best_image_offset >= 0
      && (self->best_image_offset + self->best_bytes_in_res)
            >= g_bytes_get_size(self->data));

    lock_size = cern_icon_get_size(self);
    lock_point = cern_point_create(0, 0);
    lock_rectangle
      = cern_rectangle_create_with_point_and_size(&lock_point, &lock_size);

    bitmap
      = cern_bitmap_new(cern_icon_get_width(self), cern_icon_get_height(self));

    lock_bitmap_data = cern_bitmap_data_new();

    bitmap_data
      = cern_bitmap_lock_bits(bitmap, &lock_rectangle,
                              CernImageLockMode_WriteOnly,
                              CernPixelFormat_Format32bppArgb, lock_bitmap_data);

    guint32 *pixel_ptr = (guint32 *) cern_bitmap_data_get_scan0(bitmap_data);
    gsize new_offset = self->best_image_offset + sizeof(BITMAPINFOHEADER);
    guint8 const *data_ptr = g_bytes_get_data(self->data, NULL);

    gsize line_length = cern_icon_get_width(self) * 4;
    gsize width = cern_icon_get_width(self);
    gsize height = cern_icon_get_height(self);

    for (gsize i = (height - 1) * 4; i >= 0; i -= 4) {
      memcpy(pixel_ptr, data_ptr + new_offset + i * width, line_length);
      pixel_ptr += width;
    }

    cern_bitmap_unlock_bits(bitmap, bitmap_data);

    g_clear_object(&lock_bitmap_data);
  } else if (self->best_image_bit_depth == 0 || self->best_image_bit_depth == 32) {
    ICONINFO icon_info = { 0 };
    BITMAP bmp = { 0 };
    GetIconInfo((HICON) self->handle, &icon_info);

    if (icon_info.hbmColor!= NULL) {
      GetObject(icon_info.hbmColor, sizeof(BITMAP), &bmp);
      if (bmp.bmBitsPixel != 32) {
        CernBitmap *tmp_bitmap;
        CernBitmapData *bitmap_data;
        CernBitmapData *bitmap_data_pass;
        CernBitmapData *target_bitmap_data;
        CernRectangle lock_rectangle;
        CernPixelFormat px_format;

        lock_rectangle
          = (CernRectangle) {0, 0, bmp.bmWidth, bmp.bmHeight};

        bitmap_data_pass = cern_bitmap_data_new();
        px_format = cern_image_get_pixel_format(CERN_IMAGE(tmp_bitmap));

        tmp_bitmap = cern_bitmap_new_from_h_bitmap(icon_info.hbmColor);
        bitmap_data
          = cern_bitmap_lock_bits(tmp_bitmap, &lock_rectangle,
                                  CernImageLockMode_ReadOnly,
                                  px_format, bitmap_data_pass);

        if (cern_bitmap_has_alpha(bitmap_data)) {
          guint32 w = cern_bitmap_data_get_width(bitmap_data);
          guint32 h = cern_bitmap_data_get_height(bitmap_data);
          bitmap
            = cern_bitmap_new_with_format(w, h, CernPixelFormat_Format32bppArgb);

          target_bitmap_data
            = cern_bitmap_lock_bits(bitmap, &lock_rectangle,
                                    CernImageLockMode_WriteOnly,
                                    CernPixelFormat_Format32bppArgb,
                                    bitmap_data_pass);
          cern_icon_copy_bitmap_data(bitmap_data, target_bitmap_data);
        }

        if (tmp_bitmap != NULL) {
          if (bitmap_data != NULL) {
            cern_bitmap_unlock_bits(tmp_bitmap, bitmap_data);
          }
          g_object_unref(tmp_bitmap);
        }

        if (bitmap != NULL) {
          if (target_bitmap_data != NULL) {
            cern_bitmap_unlock_bits(bitmap, target_bitmap_data);
          }
        }

        if (bitmap_data_pass != NULL) {
          g_object_unref(bitmap_data_pass);
        }
      }
    }
    if (icon_info.hbmColor != NULL) {
      DeleteObject(icon_info.hbmColor);
    }
    if (icon_info.hbmMask != NULL) {
      DeleteObject(icon_info.hbmMask);
    }
  }

  if (bitmap == NULL) {
    CernSize size = cern_icon_get_size(self);
    CernBitmap *temp_bitmap = cern_bitmap_new_from_h_icon(self->handle);

    CernRectangle draw_rect
      = cern_rectangle_create(0, 0, size.width, size.height);
    bitmap = cern_bitmap_new(size.width, size.height);

    CernGraphics *graphics
      = cern_graphics_new_from_image(CERN_IMAGE(bitmap));

    cern_graphics_draw_image_rect_i(graphics, CERN_IMAGE(temp_bitmap), &draw_rect);



    g_object_unref(temp_bitmap);
    g_object_unref(graphics);
  }

  return bitmap;
}

CernBitmap *
cern_icon_to_bitmap(CernIcon *self) {
  g_return_val_if_fail(CERN_IS_ICON(self), NULL);

  if (cern_icon_has_png_signature(self)) {
    return cern_icon_png_frame(self);
  }
  return cern_icon_bitmap_frame(self);
}

CernIcon *
cern_icon_new_from_h_icon(gpointer h_icon) {
  g_return_val_if_fail(h_icon != NULL, NULL);

}

void
cern_icon_unref(CernIcon *self) {
  g_return_if_fail(CERN_IS_ICON(self));
}
