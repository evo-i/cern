#include "cern/ui/cursor.h"
#include "cern/core/windows/handle.h"
#include "cern/drawing/icon.h"
#include "cern/drawing/2d/matrix.h"

#include <Windows.h>
#define COBJMACROS
#define WIDL_C_INLINE_WRAPPERS
#include <olectl.h>
#include <shlwapi.h>

static
CernSize
cern_cursor_size = { 0 };

struct _CernCursor {
  GObject parent_instance;

  GBytes *cursor_data;
  gpointer handle;
  gboolean handle_owner;
  gint32 resource_id;
  gpointer user_data;
};

static
gpointer
real_cern_cursor_get_handle(gpointer iface) {
  CernCursor *self = CERN_CURSOR(iface);
  return self->handle;
}

static
void
real_cern_cursor_set_handle(gpointer iface, gpointer value) {
  CernCursor *self = CERN_CURSOR(iface);
  self->handle = value;
}

static
void
cern_cursor_handle_interface_init(CernHandleInterface *iface) {
  iface->get = real_cern_cursor_get_handle;
  iface->set = real_cern_cursor_set_handle;
}

G_DEFINE_FINAL_TYPE_WITH_CODE(CernCursor, cern_cursor, G_TYPE_OBJECT,
  G_IMPLEMENT_INTERFACE(CERN_TYPE_HANDLE, cern_cursor_handle_interface_init));

static
void
cern_cursor_dispose(GObject *object) {
  CernCursor *self = CERN_CURSOR(object);

  if (self->cursor_data != NULL) {
    g_bytes_unref(self->cursor_data);
    self->cursor_data = NULL;
  }

  G_OBJECT_CLASS(cern_cursor_parent_class)->dispose(object);
}

static
void
cern_cursor_finalize(GObject *object) {
  CernCursor *self = CERN_CURSOR(object);

  if (self->handle_owner
      && self->handle != NULL) {
    DestroyCursor(self->handle);
  }

  cern_handle_set(CERN_HANDLE(self), NULL);
  G_OBJECT_CLASS(cern_cursor_parent_class)->finalize(object);
}

static
void
cern_cursor_class_init(CernCursorClass *klass) {
  GObjectClass *object_class = G_OBJECT_CLASS(klass);
  object_class->finalize = cern_cursor_finalize;
}

static
void
cern_cursor_init(CernCursor *self) {
  cern_handle_set(CERN_HANDLE(self), NULL);
  self->handle_owner = TRUE;
}

static
SIZE
cern_cursor_get_icon_size(CernCursor *self, gpointer handle) {
  SIZE size = {
    .cx = cern_cursor_size.width,
    .cy = cern_cursor_size.height
  };

  SIZE icon_size = { 0 };
  ICONINFO icon_info = { 0 };
  BITMAP bitmap = { 0 };

  if (!GetIconInfo(handle, &icon_info)) {
    return size;
  }

  if (icon_info.hbmColor != NULL) {
    GetObject(icon_info.hbmColor, sizeof(BITMAP), &bitmap);
    size.cx = bitmap.bmWidth;
    size.cy = bitmap.bmHeight;
    DeleteObject(icon_info.hbmColor);
  } else if (icon_info.hbmMask != NULL) {
    GetObject(icon_info.hbmMask, sizeof(BITMAP), &bitmap);
    size.cx = bitmap.bmWidth;
    size.cy = bitmap.bmHeight / 2;
    DeleteObject(icon_info.hbmMask);
  }

  return size;
}

static
gboolean
cern_cursor_load_picture(CernCursor *self, IStream *stream) {
  HRESULT hr;
  IPicture *picture;
  IPersistStream *persist_stream;

  g_return_val_if_fail(stream != NULL, FALSE);

  hr = OleCreatePictureIndirect(NULL, &IID_IPicture, TRUE, (void**) &picture);

  if (FAILED(hr)) {
    return FALSE;
  }

  hr = IPicture_QueryInterface(picture, &IID_IPersistStream, (void**) &persist_stream);

  if (FAILED(hr)) {
    IPicture_Release(picture);
    return FALSE;
  }

  hr = persist_stream->lpVtbl->Load(persist_stream, stream);

  if (FAILED(hr)) {
    IPicture_Release(picture);
    persist_stream->lpVtbl->Release(persist_stream);
    return FALSE;
  }

  hr = IPicture_get_Handle(picture, (OLE_HANDLE *) &self->handle);

  if (FAILED(hr)) {
    IPicture_Release(picture);
    persist_stream->lpVtbl->Release(persist_stream);
    return FALSE;
  }

  SHORT pic_type = 0;
  hr = IPicture_get_Type(picture, &pic_type);

  if (FAILED(hr) || pic_type != PICTYPE_ICON) {
    IPicture_Release(picture);
    persist_stream->lpVtbl->Release(persist_stream);
    return FALSE;
  }

  gpointer cursor_handle;

  hr = IPicture_get_Handle(picture, (OLE_HANDLE *) &cursor_handle);

  if (FAILED(hr)) {
    IPicture_Release(picture);
    persist_stream->lpVtbl->Release(persist_stream);
    return FALSE;
  }

  SIZE pic_size = { 0 };

  pic_size = cern_cursor_get_icon_size(self, cursor_handle);

  self->handle = CopyImage(cursor_handle, IMAGE_CURSOR, pic_size.cx, pic_size.cy, 0);
  self->handle_owner = TRUE;

  IPicture_Release(picture);
  persist_stream->lpVtbl->Release(persist_stream);
  return TRUE;
}

CernCursor *
cern_cursor_new(gpointer handle) {
  CernCursor *self;
  g_return_val_if_fail(handle != NULL, NULL);

  self = g_object_new(CERN_TYPE_CURSOR, NULL);

  self->handle = handle;
  self->handle_owner = FALSE;
  return self;
}

CernCursor *
cern_cursor_new_from_file_name(gchar const *file_name) {
  CernCursor *self;
  GFile *file;
  GError *error;
  GInputStream *stream;

  g_return_val_if_fail(file_name != NULL, NULL);

  file = g_file_new_for_path(file_name);
  error = NULL;

  stream = G_INPUT_STREAM(g_file_read(file, NULL, &error));

  self =
    cern_cursor_new_from_stream(G_INPUT_STREAM(g_file_read(file, NULL, &error)));

  return self;
}

CernCursor *
cern_cursor_new_from_stream(GInputStream *stream) {
  CernCursor *self = NULL;
  GError *error = NULL;
  GBytes *bytes;
  gsize stream_length = 0;

  g_seekable_seek(G_SEEKABLE(stream), 0, G_SEEK_END, NULL, &error);
  stream_length = g_seekable_tell(G_SEEKABLE(stream));
  g_seekable_seek(G_SEEKABLE(stream), 0, G_SEEK_SET, NULL, &error);
  bytes = g_input_stream_read_bytes(stream, stream_length, NULL, &error);
  g_object_unref(stream);

  if (!error) {
    self = cern_cursor_new(NULL);
    self->cursor_data = bytes;
    BYTE const *data = g_bytes_get_data(bytes, &stream_length);
    IStream *istream
      = SHCreateMemStream(data, stream_length);
    cern_cursor_load_picture(self, istream);
    istream->lpVtbl->Release(istream);
  } else {
    g_error_free(error);
    g_bytes_unref(bytes);
  }

  return self;
}

CernRectangle
cern_cursor_rectangle_get_clip(void) {
  RECT rect = { 0 };
  GetClipCursor(&rect);

  return (CernRectangle) {
    .x = rect.left,
    .y = rect.top,
    .width = rect.right - rect.left,
    .height = rect.bottom - rect.top
  };
}

void
cern_cursor_rectangle_set_clip(CernRectangle *value) {
  RECT rect = {
    .left = value->x,
    .top = value->y,
    .right = value->x + value->width,
    .bottom = value->y + value->height
  };
  ClipCursor(&rect);
}

CernCursor *
cern_cursor_get_current(void) {
  return cern_cursor_new(GetCursor());
}

gpointer
cern_cursor_get_handle(CernCursor *self) {
  g_return_val_if_fail(CERN_IS_CURSOR(self), NULL);
  return cern_handle_get(CERN_HANDLE(self));
}

CernPoint
cern_cursor_get_hot_spot(CernCursor *self) {
  CernPoint hot_spot = { 0 };
  CernHandle *icon_handle = NULL;
  gpointer icon_handle_native = NULL;
  ICONINFO icon_info = { 0 };
  CernIcon *current_icon = NULL;

  current_icon = cern_icon_new_from_h_icon(cern_cursor_get_handle(self));

  if (current_icon != NULL) {
    icon_handle = CERN_HANDLE(current_icon);
    icon_handle_native = cern_handle_get(icon_handle);
    GetIconInfo(icon_handle_native, &icon_info);
    hot_spot.x = icon_info.xHotspot;
    hot_spot.y = icon_info.yHotspot;

    g_object_unref(current_icon);

    if (icon_info.hbmMask != NULL) {
      DeleteObject(icon_info.hbmMask);
    }

    if (icon_info.hbmColor != NULL) {
      DeleteObject(icon_info.hbmColor);
    }
  } else {
    g_warning("%s(...): Failed to get icon info", __func__);
  }

  return hot_spot;
}

CernPoint
cern_cursor_get_position(void) {
  POINT point = { 0 };
  GetCursorPos(&point);
  return (CernPoint) { .x = point.x, .y = point.y };
}

void
cern_cursor_set_position(CernPoint value) {
  SetCursorPos(value.x, value.y);
}

CernSize
cern_cursor_get_size(CernCursor *self) {
  if (cern_cursor_size.width == 0
      && cern_cursor_size.height == 0) {
    cern_cursor_size.width = GetSystemMetrics(SM_CXCURSOR);
    cern_cursor_size.height = GetSystemMetrics(SM_CYCURSOR);
  }

  return cern_cursor_size;
}

gpointer
cern_cursor_set_tag(CernCursor *self, gpointer value) {
  gpointer old_value;

  old_value = self->user_data;
  self->user_data = value;

  return old_value;
}

gpointer
cern_cursor_get_tag(CernCursor *self) {
  return self->user_data;
}

gpointer
cern_cursor_copy_handle(CernCursor *self) {
  CernSize size;

  size = cern_cursor_get_size(self);

  return
    CopyImage(cern_cursor_get_handle(self), IMAGE_CURSOR,
              size.width, size.height, 0);
}

# define ROP (gint32)(0xCC0020)

static
void
cern_cursor_draw_image_core(CernCursor *self,
                            CernGraphics *graphics,
                            CernRectangle *image_rect,
                            CernRectangle *target_rect, gboolean stretch) {
  CernMatrix *matrix;
  gpointer hdc;

  graphics = g_object_ref(graphics);

  matrix = cern_graphics_get_transform(graphics);

  target_rect->x += (gint32) cern_matrix_get_offset_x(matrix);
  target_rect->y += (gint32) cern_matrix_get_offset_y(matrix);

  hdc = cern_graphics_get_hdc(graphics);

  {
    gint32 image_x = 0;
    gint32 image_y = 0;
    gint32 image_width = 0;
    gint32 image_height = 0;
    gint32 target_x = 0;
    gint32 target_y = 0;
    gint32 target_width = 0;
    gint32 target_height = 0;
    CernSize cursor_size = cern_cursor_get_size(self);
    gint32 draw_width = 0, draw_height = 0;
    gint32 clip_width = 0, clip_height = 0;

    if (!cern_rectangle_is_empty(image_rect)) {
      image_x = image_rect->x;
      image_y = image_rect->y;
      image_width = image_rect->width;
      image_height = image_rect->height;
    } else {
      image_width = cursor_size.width;
      image_height = cursor_size.height;
    }

    if (!cern_rectangle_is_empty(target_rect)) {
      target_x = target_rect->x;
      target_y = target_rect->y;
      target_width = target_rect->width;
      target_height = target_rect->height;
    } else {
      target_width = cursor_size.width;
      target_height = cursor_size.height;
    }

    if (stretch) {
      if (image_width == target_width && image_height == target_height
          && image_x == 0 && image_y == 0 && ROP == SRCCOPY
          && image_width == cursor_size.width
          && image_height == cursor_size.height) {
        (void) DrawIcon(hdc, target_x, target_y, self->handle);
        cern_graphics_rlease_hdc_value(graphics, hdc);
        g_object_unref(graphics);
        return;
      }

      draw_width = cursor_size.width * target_width / image_width;
      draw_height = cursor_size.height * target_height / image_height;
      clip_width = target_width;
      clip_height = target_height;
    } else {
      if (image_x == 0 && image_y == 0 && ROP == SRCCOPY
          && cursor_size.width <= target_width
          && cursor_size.height <= target_height
          && cursor_size.width == image_width
          && cursor_size.height == image_height) {
        (void) DrawIcon(hdc, target_x, target_y, self->handle);
        cern_graphics_rlease_hdc_value(graphics, hdc);
        g_object_unref(graphics);
        return;
      }

      draw_width = cursor_size.width;
      draw_height = cursor_size.height;
      clip_width = target_width < image_width ? target_width : image_width;
      clip_height = target_height < image_height ? target_height : image_height;
    }

    if (ROP == SRCCOPY) {
      IntersectClipRect(hdc, target_x, target_y,
                        target_x + clip_width, target_y + clip_height);
      DrawIconEx(hdc, target_x - image_x, target_y - image_y, self->handle,
                 draw_width,  draw_height, 0, NULL, DI_NORMAL);
      g_object_unref(graphics);
      cern_graphics_rlease_hdc_value(graphics, hdc);
      return;
    }
  }
}

# undef ROP

void
cern_cursor_draw(CernCursor *self, CernGraphics *graphics,
                 CernRectangle *target_rect) {
  CernRectangle image_rect = cern_rectangle_empty();
  cern_cursor_draw_image_core(self, graphics, &image_rect, target_rect, FALSE);
}

void
cern_cursor_draw_stretched(CernCursor *self, CernGraphics *graphics,
                           CernRectangle *target_rect) {
  CernRectangle image_rect = cern_rectangle_empty();
  cern_cursor_draw_image_core(self, graphics, &image_rect, target_rect, TRUE);
}

void
cern_cursor_hide(void) {
  ShowCursor(FALSE);
}

void
cern_cursor_show(void) {
  ShowCursor(TRUE);
}

gboolean
cern_cursor_equals(CernCursor *self, CernCursor *other) {
  return self->handle == other->handle;
}
