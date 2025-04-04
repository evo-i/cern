#ifndef CERN_UI_TEXT_RENDERER_H
#define CERN_UI_TEXT_RENDERER_H 1

#include <glib-object.h>
#include "cern/drawing/point.h"
#include "cern/drawing/rectangle.h"
#include "cern/drawing/font.h"
#include "cern/ui/text_format_flags.h"
#include "cern/drawing/internal/i_device_context.h"
#include "cern/drawing/color.h"

G_BEGIN_DECLS

void
cern_text_renderer_draw_text(CernIDeviceContext *dc,
                             gchar const *text,
                             CernFont *font,
                             CernPoint point,
                             CernColor color);


void
cern_text_renderer_draw_text_with_back_color(CernIDeviceContext *dc,
                                             gchar const *text,
                                             CernFont *font,
                                             CernPoint point,
                                             CernColor fore_color,
                                             CernColor back_color);

void
cern_text_renderer_draw_text_with_format_flags(CernIDeviceContext *dc,
                                               gchar const *text,
                                               CernFont *font,
                                               CernPoint point,
                                               CernColor fore_color,
                                               CernTextFormatFlags flags);

void
cern_text_renderer_draw_text_with_back_color_and_format_flags(CernIDeviceContext *dc,
                                                              gchar const *text,
                                                              CernFont *font,
                                                              CernPoint point,
                                                              CernColor fore_color,
                                                              CernColor back_color,
                                                              CernTextFormatFlags flags);

void
cern_text_renderer_draw_text_with_bounds(CernIDeviceContext *dc,
                                         gchar const *text,
                                         CernFont *font,
                                         CernRectangle bounds,
                                         CernColor fore_color);

void
cern_text_renderer_draw_text_with_bounds_and_back_color(CernIDeviceContext *dc,
                                                        gchar const *text,
                                                        CernFont *font,
                                                        CernRectangle bounds,
                                                        CernColor fore_color,
                                                        CernColor back_color);

void
cern_text_renderer_draw_text_with_bounds_and_text_format(CernIDeviceContext *dc,
                                                         gchar const *text,
                                                         CernFont *font,
                                                         CernRectangle bounds,
                                                         CernColor fore_color,
                                                         CernTextFormatFlags flags);

void
cern_text_renderer_draw_text_with_bounds_back_color_and_text_format(CernIDeviceContext *dc,
                                                                    gchar const *text,
                                                                    CernFont *font,
                                                                    CernRectangle bounds,
                                                                    CernColor fore_color,
                                                                    CernColor back_color,
                                                                    CernTextFormatFlags flags);

CernSize
cern_text_renderer_measure_text(gchar const *text,
                                CernFont *font);

CernSize
cern_text_renderer_measure_text_with_proposed_size(gchar const *text,
                                                   CernFont *font,
                                                   CernSize proposed_size);

CernSize
cern_text_renderer_measure_text_with_proposed_size_and_text_format(gchar const *text,
                                                                   CernFont *font,
                                                                   CernSize proposed_size,
                                                                   CernTextFormatFlags flags);

CernSize
cern_text_renderer_measure_text_dc(CernIDeviceContext *dc,
                                   gchar const *text,
                                   CernFont *font);

CernSize
cern_text_renderer_measure_text_dc_with_proposed_size(CernIDeviceContext *dc,
                                                      gchar const *text,
                                                      CernFont *font,
                                                      CernSize proposed_size);

CernSize
cern_text_renderer_measure_text_dc_with_proposed_size_and_text_format(CernIDeviceContext *dc,
                                                                      gchar const *text,
                                                                      CernFont *font,
                                                                      CernSize proposed_size,
                                                                      CernTextFormatFlags flags);
CernColor
cern_text_renderer_get_disabled_text_color(CernColor back_color);

G_END_DECLS

#endif /* CERN_UI_TEXT_RENDERER_H */
