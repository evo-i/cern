#ifndef CERN_UI_DPI_HELPER_H
#define CERN_UI_DPI_HELPER_H 1

#include <glib-object.h>
#include "cern/drawing/bitmap.h"
#include "cern/drawing/size.h"

G_BEGIN_DECLS

typedef struct _CernButton CernButton;

gboolean
cern_dpi_helper_get_is_awareness_value_set(void);

void
cern_dpi_helper_initialize_helper_windows(void);

void
cern_dpi_helper_initialize_helper_quirks();

gboolean
cern_dpi_helper_get_enable_dpi_changed_high_dpi_improvements(void);

gboolean
cern_dpi_helper_get_enable_tool_strip_high_dpi_improvements(void);

gboolean
cern_dpi_hepler_get_enable_tool_strip_per_monitor_v2_high_dpi_improvements(void);

gboolean
cern_dpi_helper_get_enable_dpi_changed_message_handling(void);

gboolean
cern_dpi_helper_get_enable_checked_list_box_high_dpi_improvements(void);

gboolean
cern_dpi_helper_get_enable_single_pass_scaling_of_dpi_windows(void);

gboolean
cern_dpi_helper_get_enable_thread_exception_dialog_high_dpi_improvements(void);

gboolean
cern_dpi_helper_get_enable_data_grid_view_control_hihg_dpi_improvements(void);

gboolean
cern_dpi_helper_get_enable_anchor_layout_high_dpi_improvements(void);

gboolean
cern_dpi_helper_get_enable_month_calendar_high_dpi_improvements(void);

gdouble
cern_dpi_helper_get_device_dpi(void);

gboolean
cern_dpi_helper_get_is_scaling_required(void);

gint32
cern_dpi_helper_logical_to_device_units(gint32 value, gint32 device_pixels);

gdouble
cern_dpi_helper_logical_to_device_units_double(gdouble value, gint32 device_pixels);

gint32
cern_dpi_helper_logical_to_device_units_x(gint32 value);

gint32
cern_dpi_helper_logical_to_device_units_y(gint32 value);

CernSize
cern_dpi_helper_logical_to_device_units_size(CernSize logical_size, gint32 device_dpi);

CernBitmap *
cern_dpi_helper_create_resized_bitmap(CernBitmap *logical_image, CernSize target_size_image);

void
cern_dpi_helper_scale_bitmap_logical_to_device(CernBitmap **logical_image, gint32 device_dpi);

gint32
cern_dpi_helper_convert_to_given_dpi_pixel(gint32 value, gdouble pixel_factor);

void
cern_dpi_helper_scale_button_image_logical_to_device(CernButton *btn);

G_END_DECLS

#endif /* CERN_UI_DPI_HELPER_H */
