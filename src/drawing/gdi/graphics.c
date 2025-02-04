#include "cern/drawing/graphics.h"
#include "cern/drawing/internal/native_gdi_object.h"
#include "cern/drawing/graphics_context.h"
#include "cern/drawing/2d/graphics_state.h"
#include "cern/drawing/internal/i_device_context.h"
#include "cern/drawing/internal/device_context.h"
#include "cern/drawing/image.h"
#include "cern/drawing/matrix.h"


#include <Windows.h>
#include <gdiplus.h>

#define NGDI_HANDLE(PTR)

static
gpointer
cern_graphics_halftone_palette = NULL;

static
gpointer
cern_graphics_get_handle_image(CernImage *image) {
  CernNativeGdiObject *object;
  object = CERN_NATIVE_GDI_OBJECT(image);
  return cern_native_gdi_object_get_native_handle(object);
}

static
GpRegion *
cern_graphics_get_handle_region(CernRegion *region) {
  CernNativeGdiObject *object;

  object = CERN_NATIVE_GDI_OBJECT(region);
  return cern_native_gdi_object_get_native_handle(object);
}

static
gpointer
cern_graphics_get_handle_matrix(CernMatrix *matrix) {
  CernNativeGdiObject *object;
  object = CERN_NATIVE_GDI_OBJECT(matrix);
  return cern_native_gdi_object_get_native_handle(object);
}

struct _CernGraphics {
  GObject parent_instance;

  gpointer handle;
  gpointer hdc;

  CernGraphicsContext *previous_context;
  CernImage *backing_image;

  GObject *printing_helper;
};

static
void
cern_graphics_device_context_interface_init(CernIDeviceContextInterface *iface) {
  iface->get_hdc = (HDC(*)(CernIDeviceContext *)) cern_graphics_get_hdc;
  iface->release_hdc = (void(*)(CernIDeviceContext *)) cern_graphics_release_hdc;
}

G_DEFINE_FINAL_TYPE_WITH_CODE(CernGraphics,
  cern_graphics,
  G_TYPE_OBJECT,
  G_IMPLEMENT_INTERFACE(CERN_TYPE_IDEVICE_CONTEXT, cern_graphics_device_context_interface_init));

void
static
cern_graphics_finalize(GObject *object) {

  CernGraphics *self = CERN_GRAPHICS(object);

  if (cern_graphics_get_hdc(self) != NULL) {
    cern_graphics_release_hdc(self);
  }

  GdipDeleteGraphics(self->handle);
  self->handle = NULL;

  G_OBJECT_CLASS(cern_graphics_parent_class)->finalize(object);
}

void
static
cern_graphics_dispose(GObject *object) {
  CernGraphics *self = CERN_GRAPHICS(object);

  if (self->backing_image != NULL) {
    g_clear_object(&self->backing_image);
  }

  G_OBJECT_CLASS(cern_graphics_parent_class)->dispose(object);
}

void
static
cern_graphics_class_init(CernGraphicsClass *klass) {
  GObjectClass *object_class = G_OBJECT_CLASS(klass);
  object_class->finalize = cern_graphics_finalize;
  object_class->dispose = cern_graphics_dispose;
}

void
static
cern_graphics_init(CernGraphics *self) { }

void
static
cern_graphics_pop_context(CernGraphics *self, guint32 current_context_state) {
  CernGraphicsContext *context;

  if (self->previous_context == NULL) {
    g_warning("Trying to restore a context when the stack is empty");
  }

  context = self->previous_context;

  while (context != NULL) {
    if (cern_graphics_context_get_state(context) == current_context_state)  {
      self->previous_context = cern_graphics_context_get_previous(context);
      g_clear_object(&context);
      return;
    }
    self->previous_context = cern_graphics_context_get_previous(context);
  }

  g_warning("Context state not found!");
}

static
void
cern_graphics_push_context(CernGraphics *self, CernGraphicsContext *context) {
  if (context == NULL || cern_graphics_context_get_state(context) == 0) {
    g_warning("CernGraphicsContext is NULL or not valid.");
    return;
  }

  if (self->previous_context != NULL) {
    cern_graphics_context_set_previous(context, self->previous_context);
    cern_graphics_context_set_next(cern_graphics_context_get_previous(self->previous_context), context);
  }

  self->previous_context = context;
}

static
CernGraphics *
cern_graphics_internal_from_handle(gpointer handle) {
  CernGraphics *self;

  self = g_object_new(CERN_TYPE_GRAPHICS, NULL);
  self->handle = handle;

  return self;
}

CernGraphics *
cern_graphics_new_from_hdc(gpointer hdc) {
  GpStatus status;
  GpGraphics *graphics;

  if (hdc == NULL) {
    g_critical("Must pass in a valid DC");
    return NULL;
  }

  status = GdipCreateFromHDC(hdc, &graphics);

  if (status != Ok) {
    g_critical("cern_graphics_new_from_hdc(...): GdipCreateFromHDC() failed: %d", status);
    return NULL;
  }

  return cern_graphics_internal_from_handle(graphics);
}

CernGraphics *
cern_graphics_new_from_hdc_and_hdevice(gpointer h_dc, gpointer h_device) {
  GpStatus status;
  GpGraphics *graphics;

  if (h_dc == NULL || h_device == NULL) {
    g_critical("Must pass in a valid DC and Device");
    return NULL;
  }

  status = GdipCreateFromHDC2(h_dc, h_device, &graphics);

  if (status != Ok) {
    g_critical("cern_graphics_new_from_hdc_and_hdevice(...): GdipCreateFromHDC2() failed: %d", status);
    return NULL;
  }

  return cern_graphics_internal_from_handle(graphics);
}

CernGraphics *
cern_graphics_new_from_hwnd(gpointer h_wnd) {
  GpStatus status;
  GpGraphics *graphics;

  status = GdipCreateFromHWND(h_wnd, &graphics);

  if (status != Ok) {
    g_critical("cern_graphics_new_from_hwnd(...): GdipCreateFromHWND() failed: %d", status);
    return NULL;
  }

  return cern_graphics_internal_from_handle(graphics);
}

CernGraphics *
cern_graphics_new_from_image(CernImage *image) {
  GpStatus status;
  GpGraphics *graphics;
  CernNativeGdiObject *native_image;

  if (cern_image_get_pixel_format(image) & CernPixelFormat_Indexed) {
    g_critical("cern_graphics_new_from_image(...): Indexed images are not supported");
    return NULL;
  }

  status
    = GdipGetImageGraphicsContext(cern_native_gdi_object_get_native_handle(native_image),
                                  &graphics);

  if (status != Ok) {
    g_critical("cern_graphics_new_from_image(...): GdipGetImageGraphicsContext() failed: %d", status);
    return NULL;
  }

  return cern_graphics_internal_from_handle(graphics);
}

/* functions */

gpointer
cern_graphics_get_hdc(CernGraphics *self) {
  HDC hdc;
  GpStatus status;

  status = GdipGetDC(self->handle, &hdc);

  if (status != Ok) {
    g_warning("cern_graphics_get_hdc(...): GdipGetDC() failed");
    return NULL;
  }

  return (gpointer) hdc;
}

void
cern_graphics_release_hdc(CernGraphics *self) {
  cern_graphics_rlease_hdc_value(self, self->hdc);
}

void
cern_graphics_rlease_hdc_value(CernGraphics *self, gpointer hdc) {
  GpStatus status;

  status = GdipReleaseDC(self->handle, hdc);

  if (status!= Ok) {
    g_warning("cern_graphics_release_hdc_value(...): GdipReleaseDC() failed");
    return;
  }

  self->hdc = NULL;
}

void
cern_graphics_flush(CernGraphics *self) {
  cern_graphics_flush_with_intention(self, CernFlushIntention_Flush);
}

void
cern_graphics_flush_with_intention(CernGraphics *self,
                                   CernFlushIntention intention) {
  GpStatus status;

  status = GdipFlush(self->handle, intention);

  if (status!= Ok) {
    g_warning("cern_graphics_flush_with_intention(...): GdipFlush() failed");
    return;
  }
}

CernCompositingMode
cern_graphics_get_compositing_mode(CernGraphics *self) {
  GpStatus status;
  CernCompositingMode mode;

  status = GdipGetCompositingMode(self->handle, (CompositingMode *) &mode);

  if (status != Ok) {
    g_warning("cern_graphics_get_compositing_mode(...): GdipGetCompositingMode() failed: %d", status);
    return CernCompositingMode_SourceOver;
  }

  return mode;
}


void
cern_graphics_set_compositing_mode(CernGraphics *self,
                                   CernCompositingMode mode) {
  GpStatus status;

  status = GdipSetCompositingMode(self->handle, (CompositingMode) mode);

  if (status!= Ok) {
    g_warning("cern_graphics_set_compositing_mode(...): GdipSetCompositingMode() failed: %d", status);
    return;
  }
}

CernPoint
cern_graphics_get_rendering_origin(CernGraphics *self) {
  GpStatus status;
  GpPoint point;

  status = GdipGetRenderingOrigin(self->handle, &point.X, &point.Y);

  if (status != Ok) {
    g_warning("cern_graphics_get_rendering_origin(...): GdipGetRenderingOrigin() failed: %d", status);
    return cern_point_create(-1, -1);
  }

  return cern_point_create(point.X, point.Y);
}

void
cern_graphics_set_render_origin(CernGraphics *self, CernPoint *origin) {
  GpStatus status;

  status = GdipSetRenderingOrigin(self->handle, origin->x, origin->y);

  if (status != Ok) {
    g_warning("cern_graphics_set_render_origin(...): GdipSetRenderingOrigin() failed: %d", status);
    return;
  }
}

CernCompositingQuality
cern_graphics_get_compositing_quality(CernGraphics *self) {
  GpStatus status;
  CernCompositingQuality quality;

  status
    = GdipGetCompositingQuality(self->handle, (CompositingQuality *) &quality);

  if (status != Ok) {
    g_warning("cern_graphics_get_compositing_quality(...): GdipGetCompositingQuality() failed: %d", status);
    return CernCompositingQuality_Invalid;
  }

  return quality;
}

void
cern_graphics_set_compositing_quality(CernGraphics *self,
                                      CernCompositingQuality quality) {
  GpStatus status;

  status
    = GdipSetCompositingQuality(self->handle, (CompositingQuality) quality);

  if (status != Ok) {
    g_warning("cern_graphics_set_compositing_quality(...): GdipSetCompositingQuality() failed: %d", status);
    return;
  }
}

CernTextRenderingHint
cern_graphics_get_text_rendering_hint(CernGraphics *self) {
  GpStatus status;
  CernTextRenderingHint hint;

  status = GdipGetTextRenderingHint(self->handle, (TextRenderingHint *) &hint);

  if (status != Ok) {
    g_warning("cern_graphics_get_text_rendering_hint(...): GdipGetTextRenderingHint() failed: %d", status);
    return CernTextRenderingHint_SystemDefault;
  }

  return hint;
}

void
cern_graphics_set_text_rendering_hint(CernGraphics *self,
                                      CernTextRenderingHint hint) {
  GpStatus status;

  status = GdipSetTextRenderingHint(self->handle, (TextRenderingHint) hint);

  if (status != Ok) {
    g_warning("cern_graphics_set_text_rendering_hint(...): GdipSetTextRenderingHint() failed: %d", status);
    return;
  }
}

guint32
cern_graphics_get_text_contrast(CernGraphics *self) {
  GpStatus status;
  guint32 contrast;

  status = GdipGetTextContrast(self->handle, &contrast);

  if (status != Ok) {
    g_warning("cern_graphics_get_text_contrast(...): GdipGetTextContrast() failed: %d", status);
    return 0;
  }

  return contrast;
}

void
cern_graphics_set_text_contrast(CernGraphics *self, guint32 contrast) {
  GpStatus status;

  status = GdipSetTextContrast(self->handle, contrast);

  if (status != Ok) {
    g_warning("cern_graphics_set_text_contrast(...): GdipSetTextContrast() failed: %d", status);
    return;
  }
}

CernSmoothingMode
cern_graphics_get_smoothing_mode(CernGraphics *self) {
  GpStatus status;
  CernSmoothingMode mode;

  status = GdipGetSmoothingMode(self->handle, (SmoothingMode *) &mode);

  if (status != Ok) {
    g_warning("cern_graphics_get_smoothing_mode(...): GdipGetSmoothingMode() failed: %d", status);
    return CernSmoothingMode_Invalid;
  }

  return mode;
}

void
cern_graphics_set_smoothing_mode(CernGraphics *self, CernSmoothingMode mode) {
  GpStatus status;

  status = GdipSetSmoothingMode(self->handle, (SmoothingMode) mode);

  if (status != Ok) {
    g_warning("cern_graphics_set_smoothing_mode(...): GdipSetSmoothingMode() failed: %d", status);
    return;
  }
}

CernPixelOffsetMode
cern_graphics_get_pixel_offset_mode(CernGraphics *self) {
  GpStatus status;
  CernPixelOffsetMode mode;

  status = GdipGetPixelOffsetMode(self->handle, (PixelOffsetMode *) &mode);

  if (status != Ok) {
    g_warning("cern_graphics_get_pixel_offset_mode(...): GdipGetPixelOffsetMode() failed: %d", status);
    return CernPixelOffsetMode_Invalid;
  }

  return mode;
}

void
cern_graphics_set_pixel_offset_mode(CernGraphics *self,
                                    CernPixelOffsetMode mode) {
  GpStatus status;

  status = GdipSetPixelOffsetMode(self->handle, (PixelOffsetMode) mode);

  if (status != Ok) {
    g_warning("cern_graphics_set_pixel_offset_mode(...): GdipSetPixelOffsetMode() failed: %d", status);
    return;
  }
}

GObject *
cern_graphics_get_printing_helper(CernGraphics *self) {
  return self->printing_helper;
}

void
cern_graphics_set_printing_helper(CernGraphics *self, GObject *helper) {
  self->printing_helper = g_object_ref(helper);
}

CernInterpolationMode
cern_graphics_get_interpolation_mode(CernGraphics *self) {
  GpStatus status;
  CernInterpolationMode mode;

  status = GdipGetInterpolationMode(self->handle, (InterpolationMode *) &mode);

  if (status != Ok) {
    g_warning("cern_graphics_get_interpolation_mode(...): GdipGetInterpolationMode() failed: %d", status);
    return CernInterpolationMode_Invalid;
  }

  return mode;
}

void
cern_graphics_set_interpolation_mode(CernGraphics *self, CernInterpolationMode mode) {
  GpStatus status;

  status = GdipSetInterpolationMode(self->handle, (InterpolationMode) mode);

  if (status != Ok) {
    g_warning("cern_graphics_set_interpolation_mode(...): GdipSetInterpolationMode() failed: %d", status);
    return;
  }
}

CernMatrix *
cern_graphics_get_transform(CernGraphics *self);

void
cern_graphics_set_transform(CernGraphics *self, CernMatrix *matrix);

CernGraphicsUnit
cern_graphics_get_page_unit(CernGraphics *self) {
  GpStatus status;
  CernGraphicsUnit unit;

  status = GdipGetPageUnit(self->handle, (Unit *) &unit);

  if (status != Ok) {
    g_warning("cern_graphics_get_page_unit(...): GdipGetPageUnit() failed: %d", status);
    return CernGraphicsUnit_World;
  }

  return unit;
}

void
cern_graphics_set_page_unit(CernGraphics *self, CernGraphicsUnit unit) {
  GpStatus status;

  status = GdipSetPageUnit(self->handle, (Unit) unit);

  if (status != Ok) {
    g_warning("cern_graphics_set_page_unit(...): GdipSetPageUnit() failed: %d", status);
    return;
  }
}

gfloat
cern_graphics_get_page_scale(CernGraphics *self) {
  GpStatus status;
  gfloat scale;

  status = GdipGetPageScale(self->handle, &scale);

  if (status != Ok) {
    g_warning("cern_graphics_get_page_scale(...): GdipGetPageScale() failed: %d", status);
    return 0.0;
  }

  return scale;
}

void
cern_graphics_set_page_scale(CernGraphics *self, gfloat scale) {
  GpStatus status;

  status = GdipSetPageScale(self->handle, scale);

  if (status != Ok) {
    g_warning("cern_graphics_set_page_scale(...): GdipSetPageScale() failed: %d", status);
    return;
  }
}

gfloat
cern_graphics_get_dpy_x(CernGraphics *self) {
  GpStatus status;
  gfloat x;

  status = GdipGetDpiX(self->handle, &x);

  if (status != Ok) {
    g_warning("cern_graphics_get_dpy_x(...): GdipGetDpiX() failed: %d", status);
    return 0.0;
  }

  return x;
}

gfloat
cern_graphics_get_dpy_y(CernGraphics *self) {
  GpStatus status;
  gfloat y;

  status = GdipGetDpiY(self->handle, &y);

  if (status != Ok) {
    g_warning("cern_graphics_get_dpy_y(...): GdipGetDpiY() failed: %d", status);
    return 0.0;
  }

  return y;
}

void
cern_graphics_copy_from_screen(CernGraphics *self,
                               CernPoint *source_point,
                               CernPoint *dest_point,
                               CernSize *region_size) {
  gint32 sx, sy, dx, dy;
  sx = source_point->x;
  sy = source_point->y;
  dx = dest_point->x;
  dy = dest_point->y;

  cern_graphics_copy_from_screen_coords(self, sx, sy, dx, dy, region_size);
}

void
cern_graphics_copy_from_screen_coords(CernGraphics *self,
                                      gint32 sx,
                                      gint32 sy,
                                      gint32 dx,
                                      gint32 dy,
                                      CernSize *block_size) {
  cern_graphics_copy_from_screen_coords_with_operation(self, sx, sy, dx, dy, block_size, CernCopyPixelOperation_SourceCopy);
}
void
cern_graphics_copy_from_screen_with_operation(CernGraphics *self,
                                              CernPoint *source_point,
                                              CernPoint *dest_point,
                                              CernSize *block_size,
                                              CernCopyPixelOperation operation) {
  gint32 sx, sy, dx, dy;
  sx = source_point->x;
  sy = source_point->y;
  dx = dest_point->x;
  dy = dest_point->y;

  cern_graphics_copy_from_screen_coords_with_operation(self, sx, sy, dx, dy, block_size, operation);
}

void
cern_graphics_copy_from_screen_coords_with_operation(CernGraphics *self,
                                                     gint32 source_x,
                                                     gint32 source_y,
                                                     gint32 dest_x,
                                                     gint32 dest_y,
                                                     CernSize *block_size,
                                                     CernCopyPixelOperation operation) {
  gint32 status;
  DWORD error;
  gint32 dest_width, dest_height;
  CernIDeviceContext *current_device_context;
  CernDeviceContext *device_context;
  HDC screen_dc, target_dc;

  switch (operation) {
    case CernCopyPixelOperation_Blackness:
    case CernCopyPixelOperation_NotSourceErase:
    case CernCopyPixelOperation_NotSourceCopy:
    case CernCopyPixelOperation_SourceErase:
    case CernCopyPixelOperation_DestinationInvert:
    case CernCopyPixelOperation_PatInvert:
    case CernCopyPixelOperation_SourceInvert:
    case CernCopyPixelOperation_SourceAnd:
    case CernCopyPixelOperation_MergePaint:
    case CernCopyPixelOperation_MergeCopy:
    case CernCopyPixelOperation_SourceCopy:
    case CernCopyPixelOperation_SourcePaint:
    case CernCopyPixelOperation_PatCopy:
    case CernCopyPixelOperation_PatPaint:
    case CernCopyPixelOperation_Whiteness:
    case CernCopyPixelOperation_CaptureBlt:
    case CernCopyPixelOperation_NoMirrorBitmap:
    break;
    default:
      g_critical("cern_graphics_copy_from_screen_coords_with_operation(...): Invalid copy pixel operation: %d", operation);
      break;
  }

  dest_width = block_size->width;
  dest_height = block_size->height;

  current_device_context = CERN_IDEVICE_CONTEXT(self);
  device_context = cern_device_context_new(NULL);

  // FIXME: Implement this function using CernDeviceContext...

  // Example implementation using CernDeviceContext:
  screen_dc = cern_i_device_context_get_hdc(current_device_context);
  target_dc = cern_device_context_get_hdc(device_context);

  status
    = BitBlt(screen_dc, dest_x, dest_y, dest_width, dest_height,
             target_dc, source_x, source_y, operation);

  g_clear_object(&device_context);

  if (status == 0) {
    error = GetLastError();
    g_warning("cern_graphics_copy_from_screen_coords_with_operation(...): BitBlt() failed: %lu", error);
  }
}

void
cern_graphics_reset_transform(CernGraphics *self) {
  GpStatus status;

  status = GdipResetWorldTransform(self->handle);

  if (status != Ok) {
    g_warning("cern_graphics_reset_transform(...): GdipResetTransform() failed: %d", status);
    return;
  }
}

void
cern_graphics_multiply_transform(CernGraphics *self, CernMatrix *matrix) {
  cern_graphics_multiply_transform_with_order(self, matrix, CernMatrixOrder_Prepend);
}

void
cern_graphics_multiply_transform_with_order(CernGraphics *self,
                                            CernMatrix *matrix,
                                            CernMatrixOrder order) {
  GpStatus status;

  if (matrix == NULL) {
    g_warning("cern_graphics_multiply_transform_with_order(...): Invalid matrix");
    return;
  }

  status
    = GdipMultiplyWorldTransform(self->handle, cern_graphics_get_handle_matrix(matrix), (GpMatrixOrder) order);

  if (status != Ok) {
    g_warning("cern_graphics_multiply_transform_with_order(...): GdipMultiplyWorldTransform() failed: %d", status);
    return;
  }
}

void
cern_graphics_translate_transform(CernGraphics *self, gfloat dx, gfloat dy) {
  cern_graphics_translate_transform_with_order(self, dx, dy, CernMatrixOrder_Prepend);
}

void
cern_graphics_translate_transform_with_order(CernGraphics *self,
                                             gfloat dx,
                                             gfloat dy,
                                             CernMatrixOrder order) {
  GpStatus status;

  status
    = GdipTranslateWorldTransform(self->handle, dx, dy, (GpMatrixOrder) order);

  if (status != Ok) {
    g_warning("cern_graphics_translate_transform_with_order(...): GdipTranslateWorldTransform() failed: %d", status);
    return;
  }
}

void
cern_graphics_scale_transform(CernGraphics *self, gfloat sx, gfloat sy) {
  cern_graphics_scale_transform_with_order(self, sx, sy, CernMatrixOrder_Prepend);
}

void
cern_graphics_scale_transform_with_order(CernGraphics *self,
                                         gfloat sx,
                                         gfloat sy,
                                         CernMatrixOrder order) {
  GpStatus status;

  status
    = GdipScaleWorldTransform(self->handle, sx, sy, (GpMatrixOrder) order);

  if (status!= Ok) {
    g_warning("cern_graphics_scale_transform_with_order(...): GdipScaleWorldTransform() failed: %d", status);
    return;
  }
}

void
cern_graphics_rotate_transform(CernGraphics *self, gfloat angle) {
  cern_graphics_rotate_transform_with_order(self, angle, CernMatrixOrder_Prepend);
}

void
cern_graphics_rotate_transform_with_order(CernGraphics *self,
                                          gfloat angle,
                                          CernMatrixOrder order) {
  GpStatus status;

  status
    = GdipRotateWorldTransform(self->handle, angle, (GpMatrixOrder) order);

  if (status!= Ok) {
    g_warning("cern_graphics_rotate_transform_with_order(...): GdipRotateWorldTransform() failed: %d", status);
    return;
  }
}

void
cern_graphics_transform_points_f(CernGraphics *self,
                                 CernCoordinateSpace dest_space,
                                 CernCoordinateSpace src_space,
                                 CernPointF *points,
                                 gsize points_count) {
  GpStatus status;
  GpPointF *gp_points;

  if (points == NULL) {
    g_warning("cern_graphics_transform_points_f(...): Invalid points array");
    return;
  }

  gp_points = g_new(GpPointF, points_count);

  for (gsize i = 0; i < points_count; i++) {
    gp_points[i].X = points[i].x;
    gp_points[i].Y = points[i].y;
  }

  status
    = GdipTransformPoints(self->handle, (GpCoordinateSpace) dest_space,
                          (GpCoordinateSpace) src_space, gp_points, points_count);

  if (status!= Ok) {
    g_warning("cern_graphics_transform_points_f(...): GdipTransformPoints() failed: %d", status);
  } else {
    for (gsize i = 0; i < points_count; i++) {
      points[i].x = gp_points[i].X;
      points[i].y = gp_points[i].Y;
    }
  }

  g_free(gp_points);
}

void
cern_graphics_transform_points(CernGraphics *self,
                               CernCoordinateSpace dest_space,
                               CernCoordinateSpace src_space,
                               CernPoint *points,
                               gsize points_count) {
  GpStatus status;

  GpPoint *gp_points;

  if (points == NULL) {
    g_warning("cern_graphics_transform_points(...): Invalid points array");
    return;
  }

  gp_points = g_new(GpPoint, points_count);

  for (gsize i = 0; i < points_count; i++) {
    gp_points[i].X = points[i].x;
    gp_points[i].Y = points[i].y;
  }

  status
    = GdipTransformPointsI(self->handle, (GpCoordinateSpace) dest_space,
                           (GpCoordinateSpace) src_space, gp_points, points_count);

  if (status!= Ok) {
    g_warning("cern_graphics_transform_points(...): GdipTransformPointsI() failed: %d", status);
  } else {
    for (gsize i = 0; i < points_count; i++) {
      points[i].x = gp_points[i].X;
      points[i].y = gp_points[i].Y;
    }
  }

  g_free(gp_points);
}

CernColor *
cern_graphics_get_nearest_color(CernGraphics *self, CernColor *color);

void
cern_graphics_draw_line(CernGraphics *self, CernPen *pen,
                        gfloat x1, gfloat y1, gfloat x2, gfloat y2) {
  GpStatus status;
  gpointer pen_ptr;
  CernNativeGdiObject *pen_object;

  if (pen == NULL) {
    g_warning("cern_graphics_draw_line(...): Invalid pen");
    return;
  }


  pen_object = CERN_NATIVE_GDI_OBJECT(pen);
  pen_ptr = cern_native_gdi_object_get_native_handle(pen_object);

  status = GdipDrawLine(self->handle, pen_ptr, x1, y1, x2, y2);

  if (status != Ok) {
    g_warning("cern_graphics_draw_line(...): GdipDrawLine() failed: %d", status);
    return;
  }
}

void
cern_graphics_draw_line_points_f(CernGraphics *self, CernPen *pen,
                                 CernPointF *pt1, CernPointF *pt2) {

}

void
cern_graphics_draw_lines_f(CernGraphics *self, CernPen *pen,
                           CernPointF *points, gsize points_count);

void
cern_graphics_draw_line_i(CernGraphics *self, CernPen *pen,
                          gint32 x1, gint32 y1, gint32 x2, gint32 y2);

void
cern_graphics_draw_line_points(CernGraphics *self, CernPen *pen,
                               CernPoint *pt1, CernPoint *pt2);

void
cern_graphics_draw_lines(CernGraphics *self, CernPen *pen,
                         CernPoint *points, gsize points_count);

void
cern_graphics_draw_arc(CernGraphics *self, CernPen *pen,
                       gfloat x, gfloat y, gfloat width, gfloat height,
                       gfloat start_angle, gfloat sweep_angle);

void
cern_graphics_draw_arc_rect_f(CernGraphics *self, CernPen *pen,
                              CernRectangleF *rect,
                              gfloat start_angle, gfloat sweep_angle);

void
cern_graphics_draw_arc_i(CernGraphics *self, CernPen *pen,
                         gint32 x, gint32 y, gint32 width, gint32 height,
                         gint32 start_angle, gint32 sweep_angle);

void
cern_graphics_draw_arc_rect(CernGraphics *self, CernPen *pen,
                            CernRectangle *rect,
                            gfloat start_angle, gfloat sweep_angle);

void
cern_graphics_draw_bezier(CernGraphics *self, CernPen *pen,
                          gfloat x1, gfloat y1, gfloat x2, gfloat y2,
                          gfloat x3, gfloat y3, gfloat x4, gfloat y4);

void
cern_graphics_draw_bezier_points_f(CernGraphics *self, CernPen *pen,
                                   CernPointF *pt1, CernPointF *pt2,
                                   CernPointF *pt3, CernPointF *pt4);

void
cern_graphics_draw_beziers_f(CernGraphics *self, CernPen *pen,
                             CernPointF *points, gsize points_count);

void
cern_graphics_draw_bezier_with_points(CernGraphics *self, CernPen *pen,
                                      CernPoint *pt1, CernPoint *pt2,
                                      CernPoint *pt3, CernPoint *pt4);

void
cern_graphics_draw_beziers(CernGraphics *self, CernPen *pen,
                           CernPoint *points, gsize points_count);

void
cern_graphics_draw_rectangle(CernGraphics *self, CernPen *pen,
                            CernRectangle *rect);

void
cern_graphics_draw_rectangle_f(CernGraphics *self, CernPen *pen,
                               gfloat x, gfloat y, gfloat width, gfloat height);

void
cern_graphics_draw_rectangle_i(CernGraphics *self, CernPen *pen,
                               gint32 x, gint32 y, gint32 width, gint32 height);

void
cern_graphics_draw_rectangles_f(CernGraphics *self, CernPen *pen,
                                CernRectangleF *rects, gsize rects_count);

void
cern_graphics_draw_rectangles(CernGraphics *self, CernPen *pen,
                              CernRectangle *rects, gsize rects_count);

void
cern_graphics_draw_ellipse(CernGraphics *self, CernPen *pen,
                           CernRectangleF *rect);

void
cern_graphics_draw_ellipse_f(CernGraphics *self, CernPen *pen,
                             gfloat x, gfloat y, gfloat width, gfloat height);

void
cern_graphics_draw_ellipse_rect_f(CernGraphics *self, CernPen *pen,
                                  CernRectangleF *rect);

void
cern_graphics_draw_ellipse_rect(CernGraphics *self, CernPen *pen,
                                CernRectangle *rect);

void
cern_graphics_draw_ellipse_i(CernGraphics *self, CernPen *pen,
                             gint32 x, gint32 y, gint32 width, gint32 height);

void
cern_graphics_draw_pie(CernGraphics *self, CernPen *pen,
                       CernRectangleF *rect, gfloat start_angle,
                       gfloat sweep_angle);

void
cern_graphics_draw_pie_f(CernGraphics *self, CernPen *pen,
                         gfloat x, gfloat y, gfloat width, gfloat height,
                         gfloat start_angle, gfloat sweep_angle);

void
cern_graphics_draw_pie_rect(CernGraphics *self, CernPen *pen,
                            CernRectangle *rect, gfloat start_angle,
                            gfloat sweep_angle);
void
cern_graphics_draw_pie_i(CernGraphics *self, CernPen *pen,
                         gint32 x, gint32 y, gint32 width, gint32 height,
                         gint32 start_angle, gint32 sweep_angle);

void
cern_graphics_draw_polygon(CernGraphics *self, CernPen *pen,
                           CernPointF *points, gsize points_count);

void
cern_graphics_draw_polygon_i(CernGraphics *self, CernPen *pen,
                             CernPoint *points, gsize points_count);

void
cern_graphics_draw_path(CernGraphics *self, CernPen *pen,
                        CernGraphicsPath *path);

void
cern_graphics_draw_curve(CernGraphics *self, CernPen *pen,
                         CernPointF *points, gsize points_count);

void
cern_graphics_draw_curve_tension(CernGraphics *self, CernPen *pen,
                                 CernPointF *points, gsize points_count,
                                 gfloat tension);

void
cern_graphics_draw_curve_segments(CernGraphics *self, CernPen *pen,
                                  CernPointF *points, gsize offset,
                                  gsize number_of_segments);

void
cern_graphics_draw_curve_segments_tension(CernGraphics *self, CernPen *pen,
                                          CernPointF *points, gsize offset,
                                          gsize number_of_segments, gfloat tension);

void
cern_graphics_draw_curve_i(CernGraphics *self, CernPen *pen,
                           CernPoint *points, gsize points_count);

void
cern_graphics_draw_curve_tension_i(CernGraphics *self, CernPen *pen,
                                   CernPoint *points, gsize points_count,
                                   gfloat tension);

void
cern_graphics_draw_curve_segments_tension_i(CernGraphics *self, CernPen *pen,
                                            CernPoint *points, gsize offset,
                                            gsize number_of_segments, gfloat tension);

void
cern_graphics_draw_closed_curve(CernGraphics *self, CernPen *pen,
                                CernPointF *points, gsize points_count);

void
cern_graphics_draw_closed_curve_tension(CernGraphics *self, CernPen *pen,
                                        CernPointF *points, gsize points_count,
                                        gfloat tension, CernFillMode fill_mode);

void
cern_graphics_draw_closed_curve_i(CernGraphics *self, CernPen *pen,
                                  CernPoint *points, gsize points_count);

void
cern_graphics_draw_closed_curve_tension_i(CernGraphics *self, CernPen *pen,
                                          CernPoint *points, gsize points_count,
                                          gfloat tension, CernFillMode fill_mode);

void
cern_graphics_clear(CernGraphics *self, CernColor *color);

void
cern_graphics_fill_rectangle(CernGraphics *self, CernBrush *brush,
                             CernRectangleF *rect);

void
cern_graphics_fill_rectangle_f(CernGraphics *self, CernBrush *brush,
                               gfloat x, gfloat y, gfloat width, gfloat height);

void
cern_graphics_fill_rectangle_i(CernGraphics *self, CernBrush *brush,
                               CernRectangle *rect);

void
cern_graphics_fill_rectangle_i_xy(CernGraphics *self, CernBrush *brush,
                                  gint x, gint y, gint width, gint height);

void
cern_graphics_fill_rectangles(CernGraphics *self, CernBrush *brush,
                              CernRectangleF *rects, gsize rects_count);

void
cern_graphics_fill_rectangles_i(CernGraphics *self, CernBrush *brush,
                                CernRectangle *rects, gsize rects_count);

void
cern_graphics_fill_polygon(CernGraphics *self, CernBrush *brush,
                           CernPointF *points, gsize points_count);

void
cern_graphics_fill_polygon_i(CernGraphics *self, CernBrush *brush,
                             CernPoint *points, gsize points_count);

void
cern_graphics_fill_polygon_f(CernGraphics *self, CernBrush *brush,
                             CernPointF *points, gsize points_count);

void
cern_graphics_fill_polygon_mode(CernGraphics *self, CernBrush *brush,
                                CernPointF *points, gsize points_count,
                                CernFillMode fill_mode);

void
cern_graphics_fill_polygon_i_mode(CernGraphics *self, CernBrush *brush,
                                  CernPoint *points, gsize points_count,
                                  CernFillMode fill_mode);

void
cern_graphics_fill_polygon_mode_array(CernGraphics *self, CernBrush *brush,
                                      CernPoint *points, gsize points_count,
                                      CernFillMode fill_mode);

void
cern_graphics_fill_ellipse(CernGraphics *self, CernBrush *brush,
                           CernRectangleF *rect);

void
cern_graphics_fill_ellipse_f(CernGraphics *self, CernBrush *brush,
                             gfloat x, gfloat y, gfloat width, gfloat height);

void
cern_graphics_fill_ellipse_i_rect(CernGraphics *self, CernBrush *brush,
                                  CernRectangle *rect);

void
cern_graphics_fill_ellipse_i(CernGraphics *self, CernBrush *brush,
                             gint32 x, gint32 y, gint32 width, gint32 height);

void
cern_graphics_fill_pie(CernGraphics *self, CernBrush *brush,
                       CernRectangleF *rect, gfloat startAngle, gfloat sweepAngle);

void
cern_graphics_fill_pie_f(CernGraphics *self, CernBrush *brush,
                         gfloat x, gfloat y, gfloat width, gfloat height,
                         gfloat startAngle, gfloat sweepAngle);


void
cern_graphics_fill_pie_i(CernGraphics *self, CernBrush *brush,
                         gint32 x, gint32 y, gint32 width, gint32 height,
                         gint32 startAngle, gint32 sweepAngle);

void
cern_graphics_fill_pie_i_rect(CernGraphics *self, CernBrush *brush,
                              CernRectangle *rect, gint32 startAngle,
                              gint32 sweepAngle);

void
cern_graphics_fill_path(CernGraphics *self, CernBrush *brush,
                        CernGraphicsPath *path);

void
cern_graphics_fill_closed_curve(CernGraphics *self, CernBrush *brush,
                                CernPointF *points, gsize points_count);

void
cern_graphics_fill_closed_curve_mode(CernGraphics *self, CernBrush *brush,
                                     CernPointF *points, gsize points_count,
                                     CernFillMode fill_mode);


void
cern_graphics_fill_closed_curve_tension(CernGraphics *self, CernBrush *brush,
                                        CernPointF *points, gsize points_count,
                                        CernFillMode fill_mode, gfloat tension);

void
cern_graphics_fill_closed_curve_i(CernGraphics *self, CernBrush *brush,
                                  CernPoint *points, gsize points_count);

void
cern_graphics_fill_closed_curve_i_mode(CernGraphics *self, CernBrush *brush,
                                       CernPoint *points, gsize points_count,
                                       CernFillMode fill_mode);

void
cern_graphics_fill_closed_curve_i_tension(CernGraphics *self, CernBrush *brush,
                                          CernPoint *points, gsize points_count,
                                          CernFillMode fill_mode, gfloat tension);

void
cern_graphics_fill_region(CernGraphics *self, CernBrush *brush,
                          CernRegion *region);

void
cern_graphics_draw_string(CernGraphics *self, const gchar *text,
                          CernFont *font, CernBrush *brush,
                          gfloat x, gfloat y);

void
cern_graphics_draw_string_point(CernGraphics *self, const gchar *text,
                                CernFont *font, CernBrush *brush,
                                CernPointF *point);

void
cern_graphics_draw_string_format(CernGraphics *self, const gchar *text,
                                 CernFont *font, CernBrush *brush,
                                 gfloat x, gfloat y,
                                 CernStringFormat *format);

void
cern_graphics_draw_string_point_format(CernGraphics *self, const gchar *text,
                                       CernFont *font, CernBrush *brush,
                                       CernPointF *point,
                                       CernStringFormat *format);

void
cern_graphics_draw_string_rect(CernGraphics *self, const gchar *text,
                               CernFont *font, CernBrush *brush,
                               CernRectangleF *layout_rectangle);

void
cern_graphics_draw_string_rect_format(CernGraphics *self, const gchar *text,
                                      CernFont *font, CernBrush *brush,
                                      CernRectangleF *layout_rectangle,
                                      CernStringFormat *format);
CernSizeF *
cern_graphics_measure_string(CernGraphics *self, const gchar *text,
                             CernFont *font, CernSizeF *layout_area,
                             CernStringFormat *format,
                             gint32 *characters_fitted, gint32 *lines_filled);

CernSizeF *
cern_graphics_measure_string_point_format(CernGraphics *self, const gchar *text,
                                          CernFont *font, CernPointF *origin,
                                          CernStringFormat *format);

CernSizeF *
cern_graphics_measure_string_layout(CernGraphics *self, const gchar *text,
                                   CernFont *font, CernSizeF *layout_area);

CernSizeF *
cern_graphics_measure_string_format(CernGraphics *self, const gchar *text,
                                    CernFont *font, CernSizeF *layout_area,
                                    CernStringFormat *format);
CernSizeF *
cern_graphics_measure_string_basic(CernGraphics *self, const gchar *text,
                                   CernFont *font);

CernSizeF *
cern_graphics_measure_string_width(CernGraphics *self, const gchar *text,
                                   CernFont *font, gint32 width);

CernSizeF *
cern_graphics_measure_string_width_format(CernGraphics *self, const gchar *text,
                                          CernFont *font, gint32 width,
                                          CernStringFormat *format);

void
cern_graphics_measure_character_ranges(CernGraphics *self, const gchar *text,
                                       CernFont *font,
                                       CernRectangleF *layout_rectangle,
                                       CernStringFormat *format,
                                       CernRegion ** out_regions,
                                       gsize *out_count_of_regions);

void
cern_graphics_draw_icon(CernGraphics *self, CernIcon *icon, gint32 x, gint32 y);

void
cern_graphics_draw_icon_rect(CernGraphics *self, CernIcon *icon,
                             CernRectangle *target_rect);

void
cern_graphics_draw_icon_unstretched(CernGraphics *self, CernIcon *icon,
                                    CernRectangle *target_rect);

void
cern_graphics_draw_image(CernGraphics *self, CernImage *image, CernPointF *point) {
  g_critical("%s Not implemented", __func__);
}

void
cern_graphics_draw_image_xy(CernGraphics *self, CernImage *image,
                            gfloat x, gfloat y) {
  g_critical("%s Not implemented", __func__);
}

void
cern_graphics_draw_image_rect(CernGraphics *self, CernImage *image,
                              CernRectangleF *rect) {
  g_critical("%s Not implemented", __func__);
}

void
cern_graphics_draw_image_point(CernGraphics *self, CernImage *image,
                               CernPoint *point) {
  g_critical("%s Not implemented", __func__);
}

void
cern_graphics_draw_image_rect_i(CernGraphics *self, CernImage *image,
                                CernRectangle *rect) {
  g_critical("%s Not implemented", __func__);
}

void
cern_graphics_draw_image_unscaled(CernGraphics *self, CernImage *image,
                                  CernPoint *point) {
  g_critical("%s Not implemented", __func__);
}

void
cern_graphics_draw_image_unscaled_rect(CernGraphics *self, CernImage *image,
                                       CernRectangle *rect) {
  g_critical("%s Not implemented", __func__);
}

void
cern_graphics_draw_image_unscaled_and_clipped(CernGraphics *self,
                                              CernImage *image,
                                              CernRectangle *rect) {
  g_critical("%s Not implemented", __func__);
}

void
cern_graphics_draw_image_points_f(CernGraphics *self, CernImage *image,
                                  CernPointF *dest_points, gsize points_count) {
  g_critical("%s Not implemented", __func__);
}

void
cern_graphics_draw_image_points(CernGraphics *self, CernImage *image,
                                CernPoint *dest_points, gsize points_count) {
   g_critical("%s Not implemented", __func__);
}

void
cern_graphics_draw_image_rect_xy_source_rect_src_unit(CernGraphics *self,
                                                      CernImage *image,
                                                      gfloat x, gfloat y,
                                                      CernRectangleF *src_rect,
                                                      CernGraphicsUnit src_unit) {
  g_critical("%s Not implemented", __func__);
}

void
cern_graphics_draw_image_rect_xy_source_rect_src_unit_i(CernGraphics *self,
                                                        CernImage *image,
                                                        gint32 x, gint32 y,
                                                        CernRectangle *src_rect,
                                                        CernGraphicsUnit src_unit) {
  g_critical("%s Not implemented", __func__);
}

void
cern_graphics_draw_image_rect_src(CernGraphics *self, CernImage *image,
                                  CernRectangleF *dest_rect,
                                  CernRectangleF *src_rect,
                                  CernGraphicsUnit src_unit) {
  GpStatus status;

  if (dest_rect == NULL || src_rect == NULL) {
    g_critical("cern_graphics_draw_image_rect_src(...): NULL rectangle provided");
    return;
  }

  // FIXME: Need to pass ImageAttributes to the GdipDrawImageRectRect call

  status
    = GdipDrawImageRectRect(self->handle,
                            cern_graphics_get_handle_image(image),
                            dest_rect->x, dest_rect->y,
                            dest_rect->width, dest_rect->height,
                            src_rect->x, src_rect->y,
                            src_rect->width, src_rect->height,
                            (GpUnit) src_unit, NULL, NULL, NULL);


  if (status != Ok) {
    g_critical("cern_graphics_draw_image_rect_src(...): GdipDrawImageRectRect() Failed: %d", status);
  }
}

/* TODO: void cern_graphics_enumerate_metafile */

void
cern_graphics_set_clip_graphics(CernGraphics *self, CernGraphics *other,
                                CernCombineMode combine_mode) {
  GpStatus status;

  if (other == NULL) {
    g_critical("cern_graphics_set_clip_graphics(...): NULL graphics provided");
    return;
  }

  status
    = GdipSetClipGraphics(self->handle, other->handle,
                          (CombineMode) combine_mode);

  if (status != Ok) {
    g_critical("cern_graphics_set_clip_graphics(...): GdipSetClipGraphics() Failed: %d", status);
  }
}

void
cern_graphics_set_clip_rect_i(CernGraphics *self, CernRectangle *rect,
                              CernCombineMode combine_mode) {
  GpStatus status;

  if (rect == NULL) {
    g_critical("cern_graphics_set_clip_rect_i(...): NULL rectangle provided");
    return;
  }

  status
    = GdipSetClipRectI(self->handle, rect->x, rect->y,
                       rect->width, rect->height, (CombineMode) combine_mode);

  if (status != Ok) {
    g_critical("cern_graphics_set_clip_rect_i(...): GdipSetClipRectI() Failed: %d", status);
  }
}

void
cern_graphics_set_clip_rect_f(CernGraphics *self, CernRectangleF *rect,
                              CernCombineMode combine_mode) {
  GpStatus status;

  if (rect == NULL) {
    g_critical("cern_graphics_set_clip_rect_f(...): NULL rectangle provided");
    return;
  }

  status
    = GdipSetClipRect(self->handle, rect->x, rect->y,
                      rect->width, rect->height, (CombineMode) combine_mode);


  if (status != Ok) {
    g_critical("cern_graphics_set_clip_rect_f(...): GdipSetClipRect() Failed: %d", status);
  }
}

void
cern_graphics_set_clip_region(CernGraphics *self, CernRegion *region,
                              CernCombineMode combine_mode) {
  GpStatus status;
  GpRegion *native_region;

  if (region == NULL) {
    g_critical("cern_graphics_set_clip_region(...): NULL region provided");
    return;
  }

  native_region = cern_graphics_get_handle_region(region);

  status = GdipSetClipRegion(self->handle, native_region, (CombineMode) combine_mode);

  if (status != Ok) {
    g_critical("cern_graphics_set_clip_region(...): GdipSetClipRegion() Failed: %d", status);
  }
}


void
cern_graphics_set_clip_path(CernGraphics *self, CernGraphicsPath *path,
                            CernCombineMode combine_mode) {
  GpStatus status;
  GpPath *native_path;
  CernNativeGdiObject *object;

  if (path == NULL) {
    g_critical("cern_graphics_set_clip_path(...): NULL path provided");
    return;
  }

  object = CERN_NATIVE_GDI_OBJECT(path);
  native_path = cern_native_gdi_object_get_native_handle(object);

  status = GdipSetClipPath(self->handle, native_path, (CombineMode) combine_mode);

  if (status != Ok) {
    g_critical("cern_graphics_set_clip_path(...): GdipSetClipPath() Failed: %d", status);
  }
}


void
cern_graphics_intersect_clip_region(CernGraphics *self, CernRegion *region) {
  GpStatus status;
  GpRegion *reg;

  if (region == NULL) {
    g_critical("cern_graphics_intersect_clip_region(...): NULL region provided");
    return;
  }

  reg = cern_graphics_get_handle_region(region);

  status
    = GdipSetClipRegion(self->handle, reg, CombineModeIntersect);

  if (status != Ok) {
    g_critical("cern_graphics_intersect_clip_region(...): GdipSetClipRegion() Failed: %d", status);
  }
}

void
cern_graphics_intersect_clip_rect(CernGraphics *self, CernRectangle *rect) {
  GpStatus status;
  gint x, y, width, height;

  if (rect == NULL) {
    g_critical("cern_graphics_intersect_clip_rect(...): NULL rectangle provided");
    return;
  }

  x = cern_rectangle_get_x(rect);
  y = cern_rectangle_get_y(rect);
  width = cern_rectangle_get_width(rect);
  height = cern_rectangle_get_height(rect);

  status
    = GdipSetClipRectI(self->handle, x, y, width, height, CombineModeIntersect);

  if (status != Ok) {
    g_critical("cern_graphics_intersect_clip_rect(...): GdipSetClipRect() Failed: %d", status);
  }
}

void
cern_graphics_intersect_clip_rect_f(CernGraphics *self, CernRectangleF *rect) {
  GpStatus status;
  gfloat x, y, width, height;

  if (rect == NULL) {
    g_critical("cern_graphics_intersect_clip_rect_f(...): NULL rectangle provided");
    return;
  }

  x = cern_rectangle_f_get_x(rect);
  y = cern_rectangle_f_get_y(rect);
  width = cern_rectangle_f_get_width(rect);
  height = cern_rectangle_f_get_height(rect);

  status
    = GdipSetClipRect(self->handle, x, y, width, height, CombineModeIntersect);

  if (status != Ok) {
    g_critical("cern_graphics_intersect_clip_rect_f(...): GdipSetClipRect() Failed: %d", status);
  }
}

void
cern_graphics_exclude_clip_rect_f(CernGraphics *self, CernRectangleF *rect) {
  GpStatus status;
  GpRegion *native;
  CernNativeGdiObject *object;
  gfloat x, y, width, height;

  if (rect == NULL) {
    g_critical("cern_graphics_exclude_clip_rect_f(...): NULL rectangle provided");
    return;
  }

  x = cern_rectangle_f_get_x(rect);
  y = cern_rectangle_f_get_y(rect);
  width = cern_rectangle_f_get_width(rect);
  height = cern_rectangle_f_get_height(rect);

  status
    = GdipSetClipRect(self->handle, x, y, width, height, CombineModeExclude);


  if (status != Ok) {
    g_critical("cern_graphics_exclude_clip_rect_f(...): GdipSetClipRect() Failed: %d", status);
  }
}

void
cern_graphics_exclude_clip_rect(CernGraphics *self, CernRectangle *rect) {
  GpStatus status;
  gint32 x, y, width, height;

  if (rect == NULL) {
    g_critical("cern_graphics_exclude_clip_rect(...): NULL rectangle provided");
    return;
  }

  x = cern_rectangle_get_x(rect);
  y = cern_rectangle_get_y(rect);
  width = cern_rectangle_get_width(rect);
  height = cern_rectangle_get_height(rect);

  status
    = GdipSetClipRectI(self->handle, x, y, width, height, CombineModeExclude);


  if (status != Ok) {
    g_critical("cern_graphics_exclude_clip_rect(...): GdipSetClipRect() Failed: %d", status);
  }
}

void
cern_graphics_exclude_clip_region(CernGraphics *self, CernRegion *region) {
  GpStatus status;
  GpRegion *native;
  CernNativeGdiObject *object;

  if (region == NULL) {
    g_critical("cern_graphics_exclude_clip_region(...): NULL region provided");
    return;
  }

  object = CERN_NATIVE_GDI_OBJECT(region);
  native = cern_native_gdi_object_get_native_handle(object);

  status
    = GdipSetClipRegion(self->handle, native, CernCombineMode_Exclude);

  if (status != Ok) {
    g_critical("cern_graphics_exclude_clip_region(...): GdipSetClipRegion() Failed: %d", status);
  }
}

void
cern_graphics_reset_clip(CernGraphics *self) {
  GpStatus status;

  status = GdipResetClip(self->handle);

  if (status != Ok) {
    g_critical("cern_graphics_reset_clip(...): GdipResetClip() Failed: %d", status);
  }
}

void
cern_graphics_translate_clip(CernGraphics *self, gfloat dx, gfloat dy) {
  GpStatus status;

  status = GdipTranslateClip(self->handle, dx, dy);

  if (status != Ok) {
    g_critical("cern_graphics_translate_clip(...): GdipTranslateClip() Failed: %d", status);
  }
}

void
cern_graphics_translate_clip_i(CernGraphics *self, gint32 dx, gint32 dy) {
  GpStatus status;

  status = GdipTranslateClipI(self->handle, dx, dy);

  if (status != Ok) {
    g_critical("cern_graphics_translate_clip_i(...): GdipTranslateClipI() Failed: %d", status);
  }
}


GObject *
cern_graphics_get_context_info(CernGraphics *self) {
  g_critical("cern_graphics_get_context_info() is not implemented");
  return NULL;
}


CernRegion *
cern_graphics_get_clip(CernGraphics *self) {
  GpRegion *region;
  GpStatus status;
  CernRegion *result;
  CernNativeGdiObject *object;

  status = GdipGetClip(self->handle, &region);

  if (status != Ok) {
    g_warning("GdipGetClip() failed: %d", status);
    return NULL;
  }

  result = cern_region_new();
  object = CERN_NATIVE_GDI_OBJECT(result);

  cern_native_gdi_object_set_native_handle(object, region);

  return result;
}


void
cern_graphics_set_clip(CernGraphics *self, CernRegion *region) {
  cern_graphics_set_clip_region(self, region, CernCombineMode_Replace);
}

CernRectangle
cern_graphics_get_clip_bounds(CernGraphics *self) {
  CernRectangle rect;
  GpRectF bounds;
  GpStatus status;

  status = GdipGetClipBounds(self->handle, &bounds);

  if (status != Ok) {
    g_warning("GdipGetClipBounds() failed: %d", status);
    return (CernRectangle) { 0 };
  }

  rect
    = cern_rectangle_create(bounds.X, bounds.Y, bounds.Width, bounds.Height);

  return rect;
}

gboolean
cern_graphics_is_clip_empty(CernGraphics *self) {
  GpStatus status;
  BOOL is_empty;

  status = GdipIsClipEmpty(self->handle, &is_empty);

  if (status != Ok) {
    g_warning("GdipIsClipEmpty() failed: %d", status);
    return FALSE;
  }

  return is_empty != 0;
}

CernRectangleF *
cern_graphics_get_visible_clip_bounds(CernGraphics *self) {
  g_critical("cern_graphics_get_visible_clip_bounds() is not implemented.");
  return NULL;
}

gboolean
cern_graphics_is_visible_clip_empty(CernGraphics *self) {
  GpStatus status;
  BOOL is_empty;

  status = GdipIsVisibleClipEmpty(self->handle, &is_empty);

  if (status != Ok) {
    g_warning("GdipIsVisibleClipEmpty() failed: %d", status);
    return FALSE;
  }

  return is_empty != 0;
}

gboolean
cern_graphics_is_visible_point(CernGraphics *self, CernPoint *point) {
  GpStatus status;
  BOOL is_visible;
  gint x, y;

  x = cern_point_get_x(point);
  y = cern_point_get_y(point);

  status = GdipIsVisiblePointI(self->handle, x, y, &is_visible);

  if (status != Ok) {
    g_warning("GdipIsVisiblePointI() failed: %d", status);
    return FALSE;
  }

  return is_visible != 0;
}

gboolean
cern_graphics_is_visible_point_f(CernGraphics *self, CernPointF *point) {
  GpStatus status;
  BOOL is_visible;
  gfloat x, y;

  x = cern_point_f_get_x(point);
  y = cern_point_f_get_y(point);

  status = GdipIsVisiblePoint(self->handle, x, y, &is_visible);

  if (status != Ok) {
    g_warning("GdipIsVisiblePoint() failed: %d", status);
    return FALSE;
  }

  return is_visible != 0;
}

gboolean
cern_graphics_is_visible_rect(CernGraphics *self, CernRectangle *rect) {
  CernRectangleF temp;
  gboolean result;
  temp = cern_rectangle_f_create();
  cern_rectangle_f_set_x(&temp, cern_rectangle_get_x(rect));
  cern_rectangle_f_set_y(&temp, cern_rectangle_get_y(rect));
  cern_rectangle_f_set_width(&temp, cern_rectangle_get_width(rect));
  cern_rectangle_f_set_height(&temp, cern_rectangle_get_height(rect));
  result = cern_graphics_is_visible_rect_f(self, &temp);
  return result;
}

gboolean
cern_graphics_is_visible_rect_f(CernGraphics *self, CernRectangleF *rect) {
  GpStatus status;
  BOOL is_visible;
  gfloat x, y, width, height;

  x = cern_rectangle_f_get_x(rect);
  y = cern_rectangle_f_get_y(rect);
  width = cern_rectangle_f_get_width(rect);
  height = cern_rectangle_f_get_height(rect);

  status = GdipIsVisibleRect(self->handle, x, y, width, height, &is_visible);

  if (status != Ok) {
    g_warning("GdipIsVisibleRect() failed: %d", status);
    return FALSE;
  }

  return is_visible != 0;
}

CernGraphicsState *
cern_graphics_save(CernGraphics *self) {
  CernGraphicsContext *context;
  GpStatus status;
  GraphicsState real_state;

  context = cern_graphics_context_new(self);
  real_state = cern_graphics_context_get_state(context);

  status = GdipSaveGraphics(self->handle, &real_state);

  if (status != Ok) {
    g_critical("cern_graphics_save(...): GdipSaveGraphics() Failed: %d", status);
    return NULL;
  }

  context = cern_graphics_context_new(self);
  cern_graphics_context_set_state(context, real_state);
  cern_graphics_push_context(self, context);

  return cern_graphics_state_new(real_state);
}

void
cern_graphics_restore(CernGraphics *self, CernGraphicsState *state) {
  GpStatus status;

  status
    = GdipRestoreGraphics(self->handle, cern_graphics_state_get_value(state));

  if (status != Ok) {
    g_critical("cern_graphics_restore(...): GdipRestoreGraphics() Failed: %d", status);
    return;
  }

  cern_graphics_pop_context(self, cern_graphics_state_get_value(state));
}

CernGraphicsContainer *
cern_graphics_begin_container_f(CernGraphics *self, CernRectangleF *dest_rect,
                                CernRectangleF *src_rect, CernGraphicsUnit unit) {
  GpStatus status;
  GraphicsContainer state;
  CernGraphicsContext *context;
  GpRectF src, dest;

  src.X = cern_rectangle_f_get_x(src_rect);
  src.Y = cern_rectangle_f_get_y(src_rect);
  src.Width = cern_rectangle_f_get_width(src_rect);
  src.Height = cern_rectangle_f_get_height(src_rect);

  dest.X = cern_rectangle_f_get_x(dest_rect);
  dest.Y = cern_rectangle_f_get_y(dest_rect);
  dest.Width = cern_rectangle_f_get_width(dest_rect);
  dest.Height = cern_rectangle_f_get_height(dest_rect);

  status
    = GdipBeginContainer(self->handle, &dest, &src, (GpUnit) unit, &state);

  if (status != Ok) {
    g_critical("cern_graphics_begin_container_i(...): GdipBeginContainerI() failed: %d", status);
    return NULL;
  }

  context = cern_graphics_context_new(self);
  cern_graphics_context_set_state(context, state);
  cern_graphics_push_context(self, context);
  return cern_graphics_container_new(state);
}

CernGraphicsContainer *
cern_graphics_begin_container_i(CernGraphics *self, CernRectangle *dest_rect,
                                CernRectangle *src_rect, CernGraphicsUnit unit) {
  GpStatus status;
  GraphicsContainer state;
  CernGraphicsContext *context;
  GpRect src, dest;

  src.X = cern_rectangle_get_x(src_rect);
  src.Y = cern_rectangle_get_y(src_rect);
  src.Width = cern_rectangle_get_width(src_rect);
  src.Height = cern_rectangle_get_height(src_rect);

  dest.X = cern_rectangle_get_x(dest_rect);
  dest.Y = cern_rectangle_get_y(dest_rect);
  dest.Width = cern_rectangle_get_width(dest_rect);
  dest.Height = cern_rectangle_get_height(dest_rect);

  status
    = GdipBeginContainerI(self->handle, &dest, &src, (GpUnit) unit, &state);

  if (status != Ok) {
    g_critical("cern_graphics_begin_container_i(...): GdipBeginContainerI() failed: %d", status);
    return NULL;
  }

  context = cern_graphics_context_new(self);
  cern_graphics_context_set_state(context, state);
  cern_graphics_push_context(self, context);
  return cern_graphics_container_new(state);
}

CernGraphicsContainer *
cern_graphics_begin_container(CernGraphics *self) {
  CernGraphicsContext *context;
  GraphicsContainer state;
  GpStatus status;

  status = GdipBeginContainer2(self->handle, &state);

  if (status != Ok) {
    g_clear_object(&context);
    g_critical("cern_graphics_begin_container(): GdipBeginContainer2() Failed. %d", state);
    return NULL;
  }

  cern_graphics_context_set_state(context, state);
  cern_graphics_push_context(self, context);

  return cern_graphics_container_new(state);
}

void
cern_graphics_end_container(CernGraphics *self, CernGraphicsContainer *container) {
  GpStatus status;
  guint32 container_id;

  g_warn_if_fail(container != NULL);
  g_return_if_fail(container != NULL);

  status
    = GdipEndContainer(self->handle,
                       cern_graphics_container_get_value(container));
  container_id = cern_graphics_container_get_value(container);

  if (status != Ok) {
    g_warning("cern_graphics_end_container(...): GdipEndContainer() Failed...");
    return;
  }

  cern_graphics_pop_context(self, container_id);
}

void
cern_graphics_add_metafile_comment(CernGraphics *self, GByteArray *array) {
  GpStatus status = Ok;
  if (array == NULL) {
    g_critical("Array cannot be NULL");
    return;
  }

  if (array->len <= 0) {
    g_critical("Length of array must be grather than 0");
    return;
  }

  if (array->data == NULL) {
    g_critical("The data of array cannot be NULL.");
  }

  status = GdipComment(self->handle, array->len, array->data);

  if (status != Ok) {
    g_warning("Unable to set comment for metafile. GdipComment(): Failed.");
  }
}

gpointer
cern_graphics_get_halftone_palette(void) {
  static GMutex halftone_palette_mutex;

  g_mutex_lock(&halftone_palette_mutex);
  if (cern_graphics_halftone_palette == NULL) {
    cern_graphics_halftone_palette
      = GdipCreateHalftonePalette();
  }
  g_mutex_unlock(&halftone_palette_mutex);

  return cern_graphics_halftone_palette;
}
