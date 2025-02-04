#ifndef CERN_DRAWING_GRAPHICS_H
#define CERN_DRAWING_GRAPHICS_H 1

#include <glib-object.h>

#include "point.h"
#include "point_f.h"
#include "color.h"
#include "region.h"
#include "pen.h"
#include "size_f.h"
#include "size.h"
#include "rectangle_f.h"
#include "rectangle.h"

#include "2d/matrix_order.h"
#include "2d/combine_mode.h"
#include "2d/graphics_path.h"
#include "2d/smoothing_mode.h"
#include "2d/flush_intention.h"
#include "2d/compositing_mode.h"
#include "2d/coordinate_space.h"
#include "2d/pixel_offset_mode.h"
#include "2d/interpolation_mode.h"
#include "2d/compositing_quality.h"
#include "2d/graphics_container.h"

#include "text/text_hendering_hint.h"

#include "graphics_unit.h"
#include "copy_pixel_operation.h"


G_BEGIN_DECLS

typedef struct _CernImage CernImage;
typedef struct _CernMatrix CernMatrix;
typedef struct _CernFont CernFont;
typedef struct _CernIcon CernIcon;
typedef struct _CernStringFormat CernStringFormat;
typedef struct _CernGraphicsState CernGraphicsState;

#define CERN_TYPE_GRAPHICS cern_graphics_get_type ()

G_DECLARE_FINAL_TYPE(CernGraphics, cern_graphics, CERN, GRAPHICS, GObject)

CernGraphics *
cern_graphics_new_from_hdc(gpointer hdc);

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
cern_graphics_rlease_hdc_value(CernGraphics *self, gpointer hdc);

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

CernPoint
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

guint32
cern_graphics_get_text_contrast(CernGraphics *self);

void
cern_graphics_set_text_contrast(CernGraphics *self, guint32 contrast);

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
cern_graphics_draw_line_points(CernGraphics *self, CernPen *pen,
                               CernPointF *pt1, CernPointF *pt2);

void
cern_graphics_draw_lines(CernGraphics *self, CernPen *pen,
                         CernPointF *points, gsize points_count);

void
cern_graphics_draw_line_i(CernGraphics *self, CernPen *pen,
                          gint32 x1, gint32 y1, gint32 x2, gint32 y2);

void
cern_graphics_draw_line_points_i(CernGraphics *self, CernPen *pen,
                                 CernPoint *pt1, CernPoint *pt2);

void
cern_graphics_draw_lines_i(CernGraphics *self, CernPen *pen,
                           CernPoint *points, gsize points_count);

void
cern_graphics_draw_arc(CernGraphics *self, CernPen *pen,
                       gfloat x, gfloat y, gfloat width, gfloat height,
                       gfloat start_angle, gfloat sweep_angle);

void
cern_graphics_draw_arc_rect(CernGraphics *self, CernPen *pen,
                            CernRectangleF *rect,
                            gfloat start_angle, gfloat sweep_angle);

void
cern_graphics_draw_arc_i(CernGraphics *self, CernPen *pen,
                         gint32 x, gint32 y, gint32 width, gint32 height,
                         gint32 start_angle, gint32 sweep_angle);

void
cern_graphics_draw_arc_rect_i(CernGraphics *self, CernPen *pen,
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
cern_graphics_draw_beziers(CernGraphics *self, CernPen *pen,
                           CernPointF *points, gsize points_count);

void
cern_graphics_draw_bezier_with_points_i(CernGraphics *self, CernPen *pen,
                                        CernPoint *pt1, CernPoint *pt2,
                                        CernPoint *pt3, CernPoint *pt4);

void
cern_graphics_draw_beziers_i(CernGraphics *self, CernPen *pen,
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

/**
 * cern_graphics_fill_rectangles_i:
 * @self: A pointer to the CernGraphics object.
 * @brush: A pointer to the CernBrush used to fill the rectangles.
 * @rects: A pointer to an array of CernRectangle structures representing the rectangles to be filled.
 * @rects_count: The number of rectangles in the @rects array.
 *
 * Fills multiple rectangles specified by the @rects array using the given @brush.
 * The rectangles are defined in integer coordinates.
 */
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
cern_graphics_fill_polygon_mode_i(CernGraphics *self, CernBrush *brush,
                                  CernPoint *points, gsize points_count,
                                  CernFillMode fill_mode);

void
cern_graphics_fill_polygon_mode_array(CernGraphics *self, CernBrush *brush,
                                      CernPoint *points, gsize points_count,
                                      CernFillMode fill_mode);

void
cern_graphics_fill_ellipse_rect(CernGraphics *self, CernBrush *brush,
                           CernRectangleF *rect);

void
cern_graphics_fill_ellipse(CernGraphics *self, CernBrush *brush,
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
CernSizeF
cern_graphics_measure_string(CernGraphics *self, const gchar *text,
                             CernFont *font, CernSizeF *layout_area,
                             CernStringFormat *format,
                             gint32 *characters_fitted, gint32 *lines_filled);

CernSizeF
cern_graphics_measure_string_point_format(CernGraphics *self, const gchar *text,
                                          CernFont *font, CernPointF *origin,
                                          CernStringFormat *format);

CernSizeF
cern_graphics_measure_string_layout(CernGraphics *self, const gchar *text,
                                   CernFont *font, CernSizeF *layout_area);

CernSizeF
cern_graphics_measure_string_format(CernGraphics *self, const gchar *text,
                                    CernFont *font, CernSizeF *layout_area,
                                    CernStringFormat *format);
CernSizeF
cern_graphics_measure_string_basic(CernGraphics *self, const gchar *text,
                                   CernFont *font);

CernSizeF
cern_graphics_measure_string_width(CernGraphics *self, const gchar *text,
                                   CernFont *font, gint32 width);

CernSizeF
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
cern_graphics_draw_image(CernGraphics *self, CernImage *image, CernPointF *point);

void
cern_graphics_draw_image_xy(CernGraphics *self, CernImage *image,
                            gfloat x, gfloat y);

void
cern_graphics_draw_image_rect(CernGraphics *self, CernImage *image,
                              CernRectangleF *rect);

void
cern_graphics_draw_image_point(CernGraphics *self, CernImage *image,
                               CernPoint *point);

void
cern_graphics_draw_image_rect_i(CernGraphics *self, CernImage *image,
                                CernRectangle *rect);

void
cern_graphics_draw_image_unscaled(CernGraphics *self, CernImage *image,
                                  CernPoint *point);

void
cern_graphics_draw_image_unscaled_rect(CernGraphics *self, CernImage *image,
                                       CernRectangle *rect);

void
cern_graphics_draw_image_unscaled_and_clipped(CernGraphics *self,
                                              CernImage *image,
                                              CernRectangle *rect);

void
cern_graphics_draw_image_points_f(CernGraphics *self, CernImage *image,
                                  CernPointF *dest_points, gsize points_count);

void
cern_graphics_draw_image_points(CernGraphics *self, CernImage *image,
                                CernPoint *dest_points, gsize points_count);

void
cern_graphics_draw_image_rect_xy_source_rect_src_unit(CernGraphics *self,
                                                      CernImage *image,
                                                      gfloat x, gfloat y,
                                                      CernRectangleF *src_rect,
                                                      CernGraphicsUnit src_unit);

void
cern_graphics_draw_image_rect_xy_source_rect_src_unit_i(CernGraphics *self,
                                                        CernImage *image,
                                                        gint32 x, gint32 y,
                                                        CernRectangle *src_rect,
                                                        CernGraphicsUnit src_unit);

void
cern_graphics_draw_image_rect_src(CernGraphics *self, CernImage *image,
                                  CernRectangleF *dest_rect,
                                  CernRectangleF *src_rect,
                                  CernGraphicsUnit src_unit);

/* TODO: void cern_graphics_enumerate_metafile */

void
cern_graphics_set_clip_graphics(CernGraphics *self, CernGraphics *other,
                                CernCombineMode combine_mode);

void
cern_graphics_set_clip_rect_i(CernGraphics *self, CernRectangle *rect,
                              CernCombineMode combine_mode);

void
cern_graphics_set_clip_rect_f(CernGraphics *self, CernRectangleF *rect,
                              CernCombineMode combine_mode);

void
cern_graphics_set_clip_region(CernGraphics *self, CernRegion *region,
                              CernCombineMode combine_mode);

void
cern_graphics_set_clip_path(CernGraphics *self, CernGraphicsPath *path,
                            CernCombineMode combine_mode);

void
cern_graphics_intersect_clip_region(CernGraphics *self, CernRegion *region);

void
cern_graphics_intersect_clip_rect(CernGraphics *self, CernRectangle *rect);

void
cern_graphics_intersect_clip_rect_f(CernGraphics *self, CernRectangleF *rect);

void
cern_graphics_exclude_clip_rect(CernGraphics *self, CernRectangle *rect);

void
cern_graphics_exclude_clip_rect_f(CernGraphics *self, CernRectangleF *rect);

void
cern_graphics_exclude_clip_region(CernGraphics *self, CernRegion *region);

void
cern_graphics_reset_clip(CernGraphics *self);

void
cern_graphics_translate_clip(CernGraphics *self, gfloat dx, gfloat dy);

void
cern_graphics_translate_clip_i(CernGraphics *self, gint32 dx, gint32 dy);

GObject *
cern_graphics_get_context_info(CernGraphics *self);

CernRegion *
cern_graphics_get_clip(CernGraphics *self);

void
cern_graphics_set_clip(CernGraphics *self, CernRegion *region);

CernRectangle
cern_graphics_get_clip_bounds(CernGraphics *self);

gboolean
cern_graphics_is_clip_empty(CernGraphics *self);

CernRectangleF *
cern_graphics_get_visible_clip_bounds(CernGraphics *self);

gboolean
cern_graphics_is_visible_clip_empty(CernGraphics *self);

gboolean
cern_graphics_is_visible_point(CernGraphics *self, CernPoint *point);

gboolean
cern_graphics_is_visible_point_f(CernGraphics *self, CernPointF *point);

gboolean
cern_graphics_is_visible_rect(CernGraphics *self, CernRectangle *rect);

gboolean
cern_graphics_is_visible_rect_f(CernGraphics *self, CernRectangleF *rect);

CernGraphicsState *
cern_graphics_save(CernGraphics *self);

void
cern_graphics_restore(CernGraphics *self, CernGraphicsState *state);

CernGraphicsContainer *
cern_graphics_begin_container_f(CernGraphics *self, CernRectangleF *dest_rect,
                                CernRectangleF *src_rect, CernGraphicsUnit unit);

CernGraphicsContainer *
cern_graphics_begin_container_i(CernGraphics *self, CernRectangle *dest_rect,
                                CernRectangle *src_rect, CernGraphicsUnit unit);

CernGraphicsContainer *
cern_graphics_begin_container(CernGraphics *self);

void
cern_graphics_end_container(CernGraphics *self, CernGraphicsContainer *container);

void
cern_graphics_add_metafile_comment(CernGraphics *self, GByteArray *array);

gpointer
cern_graphics_get_halftone_palette(void);

G_END_DECLS

#endif /* CERN_DRAWING_GRAPHICS_H */
