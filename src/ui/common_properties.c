#include "cern/ui/common_properties.h"
#include "cern/drawing/rectangle.h"
#include "cern/ui/anchor_styles.h"
#include "cern/ui/bounds_specified.h"
#include "cern/ui/content_alignment.h"
#include "cern/ui/dock_style.h"
#include "cern/ui/layout/iarranged_element.h"
#include "cern/ui/padding.h"
#include "cern/ui/property_store.h"
#include "cern/ui/bit32/vector.h"
#include <glib.h>
#include <glibconfig.h>
#include <minwindef.h>

static
gint32
LayoutStateProperty = 0;

static
gint32
SpecifiedBoundsProperty = 0;

static
gint32
PreferredSizeCacheProperty = 0;

static
gint32
PaddingProperty = 0;

static
gint32
MarginProperty = 0;

static
gint32
MinimumSizeProperty = 0;

static
gint32
MaximumSizeProperty = 0;

static
gint32
LayoutBoundsProperty = 0;

static
CernBitVector32Section
DockAndAnchorNeedsLayoutSection = { 0 };

static
CernBitVector32Section
DockAndAnchorSection = { 0 };

static
CernBitVector32Section
DockModeSection = { 0 };

static
CernBitVector32Section
AutoSizeSection = { 0 };

static
CernBitVector32Section
BoxStretchInternalSection = { 0 };

static
CernBitVector32Section
AnchorNeverShrinksSection = { 0 };

static
CernBitVector32Section
FlowBreakSection = { 0 };

static
CernBitVector32Section
SelfAutoSizingSection = { 0 };

static
CernBitVector32Section
AutoSizeModeSection = { 0 };

static
GOnce init_once_props = G_ONCE_INIT;

static
gpointer
once_prop_init(gpointer data) {
  LayoutStateProperty = cern_property_store_create_key();
  SpecifiedBoundsProperty = cern_property_store_create_key();
  PreferredSizeCacheProperty = cern_property_store_create_key();
  PaddingProperty = cern_property_store_create_key();
  MarginProperty = cern_property_store_create_key();
  MinimumSizeProperty = cern_property_store_create_key();
  MaximumSizeProperty = cern_property_store_create_key();
  LayoutBoundsProperty = cern_property_store_create_key();

  DockAndAnchorNeedsLayoutSection = cern_bit_vector32_create_section(0x7F);
  DockAndAnchorSection = cern_bit_vector32_create_section(0x0F);
  DockModeSection = cern_bit_vector32_create_section_ex(0x01, DockAndAnchorSection);
  AutoSizeModeSection = cern_bit_vector32_create_section_ex(0x01, DockModeSection);
  BoxStretchInternalSection = cern_bit_vector32_create_section_ex(0x03, AutoSizeModeSection);
  AnchorNeverShrinksSection = cern_bit_vector32_create_section_ex(0x01, BoxStretchInternalSection);
  FlowBreakSection = cern_bit_vector32_create_section_ex(0x01, AnchorNeverShrinksSection);
  SelfAutoSizingSection = cern_bit_vector32_create_section_ex(0x01, FlowBreakSection);
  AutoSizeModeSection = cern_bit_vector32_create_section_ex(0x01, SelfAutoSizingSection);

  return NULL;
}

static
void
ensure_initialized(void) {
  g_once(&init_once_props, once_prop_init, NULL);
}

CernContentAlignment
cern_common_properties_get_default_alignment(void) {
  ensure_initialized();
  return CernContentAlignment_TopLeft;
}

CernAnchorStyles
cern_common_properties_get_default_anchor(void) {
  ensure_initialized();
  return CernAnchorStyles_Top | CernAnchorStyles_Left;
}

gboolean
cern_common_properties_get_default_auto_size(void) {
  ensure_initialized();
  return FALSE;
}

CernDockStyle
cern_common_properties_get_default_dock(void) {
  ensure_initialized();
  return CernDockStyle_None;
}

CernPadding
cern_common_properties_get_default_margin(void) {
  ensure_initialized();
  return (CernPadding) { 3, 3, 3, 3 };
}

CernSize
cern_common_properties_get_default_minimum_size(void) {
  ensure_initialized();
  return (CernSize) { 0, 0 };
}

CernSize
cern_common_properties_get_default_maximum_size(void) {
  ensure_initialized();
  return (CernSize) { 0, 0 };
}

void
cern_common_properties_clear_maximum_size(CernIArrangedElement *element) {
  ensure_initialized();
  CernPropertyStore *store
    = cern_iarranged_element_get_properties(element);

  if (cern_property_store_contains_object(store, MaximumSizeProperty)) {
    cern_property_store_remove_object(store, MaximumSizeProperty);
  }
}

gboolean
cern_common_properties_get_auto_size(CernIArrangedElement *element) {
  ensure_initialized();

  CernBitVector32 layout_state
    = cern_common_properties_get_layout_state(element);

  gint32 value
    = cern_bit_vector32_section_get(&layout_state, AutoSizeModeSection);

  return value != 0;
}

CernPadding
cern_common_properties_get_margin(CernIArrangedElement *element) {
  ensure_initialized();
  
  CernPropertyStore *store
    = cern_iarranged_element_get_properties(element);

  gboolean found;

  CernPadding padding
    = cern_property_store_try_get_padding(store, MarginProperty, &found);

  if (found) {
    return padding;
  }

  return cern_common_properties_get_default_margin();
}

CernSize
cern_common_properties_get_maximum_size(CernIArrangedElement *element, 
                                        CernSize *default_maximum_size) {
  ensure_initialized();

  CernPropertyStore *store
    = cern_iarranged_element_get_properties(element);

  gboolean found = FALSE;

  CernSize size
    = cern_property_store_try_get_size(store, MaximumSizeProperty, &found);

  if (found) {
    return size;
  }

   return (CernSize) {
    .width = default_maximum_size->width,
    .height = default_maximum_size->height
  };
}

CernSize
cern_common_properties_get_minimum_size(CernIArrangedElement *element,
                                        CernSize *default_minimum_size) {
  ensure_initialized();

  CernPropertyStore *store
    = cern_iarranged_element_get_properties(element);

  gboolean found = FALSE;

  CernSize size
    = cern_property_store_try_get_size(store, MinimumSizeProperty, &found);

  if (found) {
    return size;
  }

  return (CernSize) {
    .width = default_minimum_size->width,
    .height = default_minimum_size->height
  };
}

CernPadding
cern_common_properties_get_padding(CernIArrangedElement *element,
                                   CernPadding *default_padding) {
  ensure_initialized();

  CernPropertyStore *store
    = cern_iarranged_element_get_properties(element);

  gboolean found = FALSE;

  CernPadding padding
    = cern_property_store_try_get_padding(store, PaddingProperty, &found);

  if (found) {
    return padding;
  }

  return (CernPadding) {
    .left = default_padding->left,
    .top = default_padding->top,
    .right = default_padding->right,
    .bottom = default_padding->bottom
  };
}

CernRectangle
cern_common_properties_get_specified_bounds(CernIArrangedElement *element) {
  ensure_initialized();

  CernPropertyStore *store
    = cern_iarranged_element_get_properties(element);

  gboolean found = FALSE;

  CernRectangle rect
    = cern_property_store_try_get_rectangle(store, SpecifiedBoundsProperty, &found);

  if (found) {
    return  rect;
  }

  return cern_iarranged_element_get_bounds(element);
}

void
cern_common_properties_reset_padding(CernIArrangedElement *element) {
  ensure_initialized();

  CernPropertyStore *store
    = cern_iarranged_element_get_properties(element);

  gboolean found;

  cern_property_store_try_get_padding(store, PaddingProperty, &found);

  if (found) {
    cern_property_store_remove_object(store, PaddingProperty);
  }
}

void
cern_common_properties_set_auto_size(CernIArrangedElement *element, gboolean value) {
  ensure_initialized();

  if (value == cern_common_properties_get_auto_size(element)) {
    return;
  }

  CernBitVector32 state
    = cern_common_properties_get_layout_state(element);

  cern_bit_vector32_section_set(&state, AutoSizeModeSection, value ? 1 : 0);
  cern_common_properties_set_layout_state(element, state);

  if (value == FALSE) {
    CernRectangle rect = cern_common_properties_get_specified_bounds(element);
    cern_iarranged_element_set_bounds(element, &rect, CernBoundsSpecified_None);
  }

  g_assert(cern_common_properties_get_auto_size(element) == value);
}

void
cern_common_properties_set_margin(CernIArrangedElement *element, CernPadding *value) {
  ensure_initialized();

  CernPadding current_padding = cern_common_properties_get_margin(element);

  g_assert(!cern_padding_equals(&current_padding, value));

  CernPropertyStore *store = cern_iarranged_element_get_properties(element);

  cern_property_store_set_padding(store, PaddingProperty, *value);

  cern_layout_transaction_do_layout(cern_iarranged_element_get_container(element), element, cern_property_names_margin());
}

void
cern_common_properties_set_maximum_size(CernIArrangedElement *element, CernSize *value) {
  ensure_initialized();


  CernPropertyStore *store
    = cern_iarranged_element_get_properties(element);

  cern_property_store_set_size(store, MaximumSizeProperty, *value);


}

void
cern_common_properties_set_minimum_size(CernIArrangedElement *element, CernSize *value) {
  ensure_initialized();
}

void
cern_common_properties_set_padding(CernIArrangedElement *element, CernPadding *value) {
  ensure_initialized();
}

void
cern_common_properties_update_specified_bounds(CernIArrangedElement *element,
                                               gint32 x, gint32 y, 
                                               gint32 width, gint32 height) {
  ensure_initialized();
}

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

