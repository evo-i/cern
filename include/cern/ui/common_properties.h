#ifndef CERN_UI_COMMON_PROPERTIES_H
#define CERN_UI_COMMON_PROPERTIES_H 1

#include <glib-object.h>

#include "padding.h"
#include "auto_size_mode.h"
#include "content_alignment.h"
#include "anchor_styles.h"
#include "layout/iarranged_element.h"
#include "cern/drawing/rectangle.h"
#include "dock_style.h"
#include "bit32/vector.h"

G_BEGIN_DECLS

CernContentAlignment
cern_common_properties_get_default_alignment(void);

CernAnchorStyles
cern_common_properties_get_default_anchor(void);

gboolean
cern_common_properties_get_default_auto_size(void);

CernDockStyle
cern_common_properties_get_default_dock(void);

CernPadding
cern_common_properties_get_default_margin(void);

CernSize
cern_common_properties_get_default_minimum_size(void);

CernSize
cern_common_properties_get_default_maximum_size(void);

void
cern_common_properties_clear_maximum_size(CernIArrangedElement *element);

gboolean
cern_common_properties_get_auto_size(CernIArrangedElement *element);

CernPadding
cern_common_properties_get_margin(CernIArrangedElement *element);

CernSize
cern_common_properties_get_maximum_size(CernIArrangedElement *element,
                                        CernSize *default_maximum_size);

CernSize
cern_common_properties_get_minimum_size(CernIArrangedElement *element,
                                        CernSize *default_minimum_size);

CernPadding
cern_common_properties_get_padding(CernIArrangedElement *element,
                                   CernPadding *default_padding);

CernRectangle
cern_common_properties_get_specified_bounds(CernIArrangedElement *element);

void
cern_common_properties_reset_padding(CernIArrangedElement *element);

void
cern_common_properties_set_auto_size(CernIArrangedElement *element, gboolean value);

void
cern_common_properties_set_margin(CernIArrangedElement *element, CernPadding *value);

void
cern_common_properties_set_maximum_size(CernIArrangedElement *element, CernSize *value);

void
cern_common_properties_set_minimum_size(CernIArrangedElement *element, CernSize *value);

void
cern_common_properties_set_padding(CernIArrangedElement *element, CernPadding *value);

void
cern_common_properties_update_specified_bounds(CernIArrangedElement *element,
                                               gint32 x, gint32 y, 
                                               gint32 width, gint32 height);

void
cern_common_properties_update_specified_bounds_ex(CernIArrangedElement *element, 
                                                  gint32 x, gint32 y,
                                                  gint32 width, gint32 height,
                                                  CernBoundsSpecified specified);

void
cern_common_properties_x_clear_preferred_size_cache(CernIArrangedElement *element);

void
cern_common_properties_x_clear_all_preferred_size_caches(CernIArrangedElement *element);

CernSize
cern_common_properties_x_get_preferred_size_cache(CernIArrangedElement *element);

void
cern_common_properties_x_set_preferred_size_cache(CernIArrangedElement *element,
                                                  CernSize *value);

CernAutoSizeMode
cern_common_properties_get_auto_size_mode(CernIArrangedElement *element);

gboolean
cern_common_properties_get_needs_dock_and_anchor_layout(CernIArrangedElement *element);

gboolean
cern_common_properties_get_needs_anchor_layout(CernIArrangedElement *element);

gboolean
cern_common_properties_get_needs_dock_layout(CernIArrangedElement *element);

gboolean
cern_common_properties_get_self_auto_size_in_default_layout(CernIArrangedElement *element);

void
cern_common_properties_set_auto_size_mode(CernIArrangedElement *element,
                                          CernAutoSizeMode mode);

gboolean
cern_common_properties_should_self_size(CernIArrangedElement *element);

void
cern_common_properties_set_should_self_size_in_default_layout(CernIArrangedElement *element,
                                                              gboolean value);

CernAnchorStyles
cern_common_properties_x_get_anchor(CernIArrangedElement *element);

gboolean
cern_common_properties_x_get_auto_size_and_anchored(CernIArrangedElement *element);

CernDockStyle
cern_common_properties_x_get_dock(CernIArrangedElement *element);

void
cern_common_properties_x_set_anchor(CernIArrangedElement *element, CernAnchorStyles value);

void
cern_common_properties_x_set_dock(CernIArrangedElement *element, CernDockStyle value);

gboolean
cern_common_properties_get_flow_break(CernIArrangedElement *element);

void
cern_common_properites_set_flow_break(CernIArrangedElement *element, gboolean value);

CernSize
cern_common_properties_get_layout_bounds(CernIArrangedElement *element);

void
cern_common_properties_set_layout_bounds(CernIArrangedElement *element, CernSize *value);

gboolean
cern_common_properties_get_has_layout_bounds(CernIArrangedElement *element);

CernBitVector32
cern_common_properties_get_layout_state(CernIArrangedElement *element);

void
cern_common_properties_set_layout_state(CernIArrangedElement *element, CernBitVector32 state);

G_END_DECLS

#endif /* CERN_UI_COMMON_PROPERTIES_H */
