#include "cern/drawing/graphics.h"
#include "cern/drawing/internal/native_gdi_object.h"
#include "cern/drawing/graphics_context.h"
#include "cern/drawing/2d/graphics_state.h"
#include "cern/drawing/internal/i_device_context.h"


#include <Windows.h>
#include <gdiplus.h>

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
cern_graphics_region_get_handle(CernRegion *region) {
  CernNativeGdiObject *object;

  object = CERN_NATIVE_GDI_OBJECT(region);
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
  // Implementation of the interface initialization
}

G_DEFINE_FINAL_TYPE_WITH_CODE(CernGraphics,
  cern_graphics,
  G_TYPE_OBJECT,
  G_IMPLEMENT_INTERFACE(CERN_TYPE_IDEVICE_CONTEXT, cern_graphics_device_context_interface_init));

void
static
cern_graphics_class_init(CernGraphicsClass *klass) { }

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

CernGraphics *
cern_graphics_new_from_hdc(gpointer hdc) {

}

CernGraphics *
cern_graphics_new_from_hdc_and_hdevice(gpointer h_dc, gpointer h_device);

CernGraphics *
cern_graphics_new_from_hwnd(gpointer h_wnd);

CernGraphics *
cern_graphics_new_from_image(CernImage *image);

/* functions */

gpointer
cern_graphics_get_hdc(CernGraphics *self);

void
cern_graphics_release_hdc(CernGraphics *self);

void
cern_graphics_flush(CernGraphics *self);

void
cern_graphics_flush_with_intention(CernGraphics *self,
                                   CernFlushIntention intention);

CernCompositingMode
cern_graphics_get_compositing_mode(CernGraphics *self);

void
cern_graphics_set_compositing_mode(CernGraphics *self,
                                   CernCompositingMode mode);

CernPoint *
cern_graphics_get_rendering_origin(CernGraphics *self);

void
cern_graphics_set_render_origin(CernGraphics *self, CernPoint *origin);

CernCompositingQuality
cern_graphics_get_compositing_quality(CernGraphics *self);

void
cern_graphics_set_compositing_quality(CernGraphics *self,
                                      CernCompositingQuality quality);

CernTextRenderingHint
cern_graphics_get_text_rendering_hint(CernGraphics *self);

void
cern_graphics_set_text_rendering_hint(CernGraphics *self,
                                      CernTextRenderingHint hint);

gint32
cern_graphics_get_text_contrast(CernGraphics *self);

void
cern_graphics_set_text_contrast(CernGraphics *self, gint32 contrast);

CernSmoothingMode
cern_graphics_get_smoothing_mode(CernGraphics *self);

void
cern_graphics_set_smoothing_mode(CernGraphics *self, CernSmoothingMode mode);

CernPixelOffsetMode
cern_graphics_get_pixel_offset_mode(CernGraphics *self);

void
cern_graphics_set_pixel_offset_mode(CernGraphics *self, CernPixelOffsetMode mode);

GObject *
cern_graphics_get_printing_helper(CernGraphics *self);

void
cern_graphics_set_printing_helper(CernGraphics *self, GObject *helper);

CernInterpolationMode
cern_graphics_get_interpolation_mode(CernGraphics *self);

void
cern_graphics_set_interpolation_mode(CernGraphics *self, CernInterpolationMode mode);

CernMatrix *
cern_graphics_get_transform(CernGraphics *self);

void
cern_graphics_set_transform(CernGraphics *self, CernMatrix *matrix);

CernGraphicsUnit
cern_graphics_get_page_unit(CernGraphics *self);

void
cern_graphics_set_page_unit(CernGraphics *self, CernGraphicsUnit unit);

gfloat
cern_graphics_get_page_scale(CernGraphics *self);

void
cern_graphics_set_page_scale(CernGraphics *self, gfloat scale);

gfloat
cern_graphics_get_dpy_x(CernGraphics *self);

gfloat
cern_graphics_get_dpy_y(CernGraphics *self);

void
cern_graphics_copy_from_screen(CernGraphics *self,
                               CernPoint *source_point,
                               CernPoint *dest_point,
                               CernSize *region_size);

void
cern_graphics_copy_from_screen_coords(CernGraphics *self,
                                      gint source_x,
                                      gint source_y,
                                      gint dest_x,
                                      gint dest_y,
                                      CernSize *block_size);
void
cern_graphics_copy_from_screen_with_operation(CernGraphics *self,
                                              CernPoint *source_point,
                                              CernPoint *dest_point,
                                              CernSize *block_size,
                                              CernCopyPixelOperation operation);

void
cern_graphics_copy_from_screen_coords_with_operation(CernGraphics *self,
                                                     gint source_x,
                                                     gint source_y,
                                                     gint dest_x,
                                                     gint dest_y,
                                                     CernSize *block_size,
                                                     CernCopyPixelOperation operation);

void
cern_graphics_reset_transform(CernGraphics *self);

void
cern_graphics_multiply_transform(CernGraphics *self, CernMatrix *matrix);

void
cern_graphics_multiply_transform_with_order(CernGraphics *self,
                                           CernMatrix *matrix,
                                           CernMatrixOrder order);

void
cern_graphics_translate_transform(CernGraphics *self, gfloat dx, gfloat dy);

void
cern_graphics_translate_transform_with_order(CernGraphics *self,
                                             gfloat dx,
                                             gfloat dy,
                                             CernMatrixOrder order);

void
cern_graphics_scale_transform(CernGraphics *self, gfloat sx, gfloat sy);

void
cern_graphics_scale_transform_with_order(CernGraphics *self,
                                         gfloat sx,
                                         gfloat sy,
                                         CernMatrixOrder order);

void
cern_graphics_rotate_transform(CernGraphics *self, gfloat angle);

void
cern_graphics_rotate_transform_with_order(CernGraphics *self,
                                          gfloat angle,
                                          CernMatrixOrder order);

void
cern_graphics_transform_points_f(CernGraphics *self,
                                 CernCoordinateSpace dest_space,
                                 CernCoordinateSpace src_space,
                                 CernPointF *points,
                                 gsize points_count);

void
cern_graphics_transform_points(CernGraphics *self,
                               CernCoordinateSpace dest_space,
                               CernCoordinateSpace src_space,
                               CernPoint *points,
                               gsize points_count);

CernColor *
cern_graphics_get_nearest_color(CernGraphics *self, CernColor *color);

void
cern_graphics_draw_line(CernGraphics *self, CernPen *pen,
                        gfloat x1, gfloat y1, gfloat x2, gfloat y2);

void
cern_graphics_draw_line_points_f(CernGraphics *self, CernPen *pen,
                                 CernPointF *pt1, CernPointF *pt2);

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

  native_region = cern_graphics_region_get_handle(region);

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

  reg = cern_graphics_region_get_handle(region);

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
