#include "cern/ui/common_properties.h"
#include "cern/drawing/rectangle.h"
#include "cern/drawing/size.h"
#include "cern/ui/anchor_styles.h"
#include "cern/ui/auto_size_mode.h"
#include "cern/ui/bounds_specified.h"
#include "cern/ui/content_alignment.h"
#include "cern/ui/dock_style.h"
#include "cern/ui/layout/iarranged_element.h"
#include "cern/ui/padding.h"
#include "cern/ui/property_store.h"
#include "cern/ui/bit32/vector.h"
#include "cern/ui/control.h"
#include <Windows.h>
#include <consoleapi2.h>
#include <glib.h>
#include <glibconfig.h>
#include <minwindef.h>
#include <winsvc.h>

typedef enum DockAnchorMode {
  DockAnchorMode_Anchor = 0,
  DockAnchorMode_Dock = 1
} DockAnchorMode;

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

CernAnchorStyles
cern_common_properties_x_translate_anchor_value(CernAnchorStyles value) {
  switch ((guint32) value) {
    case CernAnchorStyles_None: {
      return cern_common_properties_get_default_anchor();
    } break;
    case CernAnchorStyles_Top | CernAnchorStyles_Left: {
      return CernAnchorStyles_None;
    } break;
    default:
      return value;
  }

  return value;
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

  CernSize tmp_size = {
    .width = -7109,
    .height = -7107
  };

  CernSize check_size
    = cern_common_properties_get_maximum_size(element, &tmp_size);

  g_assert(!cern_size_equals(&check_size, value)
           && "PERF: Caller should guard against setting maximum size to original value.");

  CernPropertyStore *store
    = cern_iarranged_element_get_properties(element);

  cern_property_store_set_size(store, MaximumSizeProperty, *value);


  CernRectangle bounds
    = cern_iarranged_element_get_bounds(element);

  bounds.width = MIN(bounds.width, value->width);
  bounds.height = MIN(bounds.height, value->height);

  cern_iarranged_element_set_bounds(element, &bounds, CernBoundsSpecified_Size);

  cern_layout_transaction_do_layout(cern_iarranged_element_get_container(element), 
                                    element, cern_property_names_maximum_size());
  CernSize last_check
    = cern_common_properties_get_maximum_size(element, &tmp_size);

  g_assert(cern_size_equals(&last_check, value)
           && "Error detected setting MaximumSize.");
}

void
cern_common_properties_set_minimum_size(CernIArrangedElement *element, CernSize *value) {
  ensure_initialized();

  CernSize tmp_size = {
    .width = -7109,
    .height = -7107
  };

  g_assert(!cern_size_equals(value, &tmp_size)
           && "PERF: Caller should guard against setting minimum size to original value.");

  CernPropertyStore *store
    = cern_iarranged_element_get_properties(element);

  cern_property_store_set_size(store, MinimumSizeProperty, *value);

  CernLayoutTransaction *ltr
    = cern_layout_transaction_new(CERN_CONTROL(cern_iarranged_element_get_container(element)),
                                  element, cern_property_names_minimum_size());

  CernRectangle bounds
    = cern_iarranged_element_get_bounds(element);

  bounds.width = MIN(bounds.width, value->width);
  bounds.height = MIN(bounds.height, value->height);

  cern_iarranged_element_set_bounds(element, &bounds, CernBoundsSpecified_Size);

  g_clear_object(&ltr);

  CernSize last_check
    = cern_common_properties_get_minimum_size(element, &tmp_size);

  g_assert(cern_size_equals(cern_size_equals(value, &last_check)
                            && "Error detected setting minimum size."));
}

void
cern_common_properties_set_padding(CernIArrangedElement *element, CernPadding *value) {
  ensure_initialized();

  CernPadding tmp_padding = { 0 };
  cern_padding_set_all(&tmp_padding, -7105); 

  CernPadding current_padding
    = cern_common_properties_get_padding(element, &tmp_padding);

  g_assert(!cern_padding_equals(value, &current_padding)
           && "PERF: Caller should guard against setting padding to original value.");

  CernPadding normalized_padding
    = cern_layout_utils_clamp_negative_padding_to_zero(value);

  CernPropertyStore *store = cern_iarranged_element_get_properties(element);

  cern_property_store_set_padding(store, PaddingProperty, *value);

  CernPadding last_check = cern_common_properties_get_padding(element, &tmp_padding);

  g_assert(cern_padding_equals(&last_check, value) && "Error detected setting padding.");
}

void
cern_common_properties_update_specified_bounds(CernIArrangedElement *element,
                                               gint32 x, gint32 y, 
                                               gint32 width, gint32 height) {
  ensure_initialized();

  CernPropertyStore *store
    = cern_iarranged_element_get_properties(element);

  CernRectangle bounds = {
    .x = x,
    .y = y,
    .width = width,
    .height = height
  };

  cern_property_store_set_rectangle(store, SpecifiedBoundsProperty, bounds);
}

void
cern_common_properties_update_specified_bounds_ex(CernIArrangedElement *element, 
                                                  gint32 x, gint32 y,
                                                  gint32 width, gint32 height,
                                                  CernBoundsSpecified specified) {
  ensure_initialized();

  CernPropertyStore *store
    = cern_iarranged_element_get_properties(element);

  CernRectangle original_bounds
    = cern_common_properties_get_specified_bounds(element);

  gboolean x_changed_but_not_specified
    = ((specified & CernBoundsSpecified_X) == CernBoundsSpecified_None)
      & (x != original_bounds.x);
  gboolean y_changed_but_not_specified
    = ((specified & CernBoundsSpecified_Y) == CernBoundsSpecified_None)
      & (y != original_bounds.y);
  gboolean w_changed_but_not_specified
    = ((specified & CernBoundsSpecified_Width) == CernBoundsSpecified_None)
      & (width != original_bounds.width);
  gboolean h_changed_but_not_specified
    = ((specified & CernBoundsSpecified_Height) == CernBoundsSpecified_None)
      & (height != original_bounds.height);

  if (x_changed_but_not_specified
      | y_changed_but_not_specified
      | w_changed_but_not_specified
      | h_changed_but_not_specified) {
    if (!x_changed_but_not_specified) {
      original_bounds.x = x;
    }

    if (!y_changed_but_not_specified) {
      original_bounds.y = y;
    }

    if (!w_changed_but_not_specified) {
      original_bounds.width = width;
    }

    if (!h_changed_but_not_specified) {
      original_bounds.height = height;
    }

    cern_property_store_set_rectangle(store, SpecifiedBoundsProperty, original_bounds);
  } else {
    if (cern_property_store_contains_object(store, SpecifiedBoundsProperty)) {
      cern_property_store_set_rectangle(store, SpecifiedBoundsProperty, cern_layout_utils_max_rectangle());
    }
  }
}

void
cern_common_properties_x_clear_preferred_size_cache(CernIArrangedElement *element) {
  CernPropertyStore *store
    = cern_iarranged_element_get_properties(element);

  cern_property_store_set_size(store, PreferredSizeCacheProperty, cern_layout_utils_invalid_size());

  CernSize test_size
    = cern_common_properties_x_set_preferred_size_cache(element);

  CernSize empty_size = { 0 };

  g_assert(cern_size_equals(&test_size, &empty_size)
           && "Error detected in xClearPreferredSizeCache.");
}

void
cern_common_properties_x_clear_all_preferred_size_caches(CernIArrangedElement *element) {
  cern_common_properties_x_clear_preferred_size_cache(element);

  GPtrArray *collection
    = cern_iarranged_element_get_children(element);
  
  for (gsize i = 0; i < collection->len; ++i) {
    cern_common_properties_x_clear_preferred_size_cache(CERN_IARRANGED_ELEMENT(collection->pdata[i]));
  }
}

CernSize
cern_common_properties_x_get_preferred_size_cache(CernIArrangedElement *element) {
  CernPropertyStore *store
    = cern_iarranged_element_get_properties(element);

  gboolean found = FALSE;

  CernSize size
    = cern_property_store_try_get_size(store, PreferredSizeCacheProperty, &found);

  if (found && (!cern_size_equals(&size, &(CernSize) { 0 }))) {
    return size;
  }

  return (CernSize) { 0 };
}

void
cern_common_properties_x_set_preferred_size_cache(CernIArrangedElement *element,
                                                  CernSize *value) {
  /* TODO: Check for not empty and not equals to current value */
  CernPropertyStore *store
    = cern_iarranged_element_get_properties(element);

  cern_property_store_set_size(store,
                               PreferredSizeCacheProperty,
                               (CernSize) { .width = value->width, .height = value->height });

  /* TODO: Check if it set or no */
}

CernAutoSizeMode
cern_common_properties_get_auto_size_mode(CernIArrangedElement *element) {
  CernBitVector32 state = cern_common_properties_get_layout_state(element);

  return
    cern_bit_vector32_section_get(&state, AutoSizeModeSection) == 0
      ? CernAutoSizeMode_GrowOnly 
      : CernAutoSizeMode_GrowAndShrink;
}

gboolean
cern_common_properties_get_needs_dock_and_anchor_layout(CernIArrangedElement *element) {
  CernBitVector32 state = cern_common_properties_get_layout_state(element);

  gboolean result
    = cern_bit_vector32_section_get(&state, DockAndAnchorNeedsLayoutSection) != 0;

  g_assert((cern_common_properties_x_get_anchor(element)
            == cern_common_properties_get_default_anchor()
            && cern_common_properties_x_get_dock(element)
               == cern_common_properties_get_default_dock()
            && cern_common_properties_get_auto_size(element) 
               == cern_common_properties_get_default_auto_size()) != result
            && "Individual values of anchor/dock/auto_relocate/auto_size "
               "contradict cern_common_properties_get_needs_dock_and_anchor_layout(...).");

  return result;
}

gboolean
cern_common_properties_get_needs_anchor_layout(CernIArrangedElement *element) {
  CernBitVector32 state = cern_common_properties_get_layout_state(element);

  gboolean result 
    = (cern_bit_vector32_section_get(&state, DockAndAnchorNeedsLayoutSection) != 0)
       && (cern_bit_vector32_section_get(&state, DockModeSection) == DockAnchorMode_Anchor);

  g_assert((cern_common_properties_x_get_anchor(element) 
            != cern_common_properties_get_default_anchor()
            || (cern_common_properties_get_auto_size(element)
                != cern_common_properties_get_default_auto_size()
                && cern_common_properties_x_get_dock(element)
                   == CernDockStyle_None)) == result
           && "Individual values of anchor/dock/auto_relocate/auto_size "
              "contradict cern_common_properties_get_needs_anchor_layout(...).");

  return result;
}

gboolean
cern_common_properties_get_needs_dock_layout(CernIArrangedElement *element) {
  CernBitVector32 state = cern_common_properties_get_layout_state(element);

  gboolean result
    = cern_bit_vector32_section_get(&state, DockModeSection) == DockAnchorMode_Dock
        && cern_iarranged_element_participates_in_layout(element);

  g_assert((cern_common_properties_x_get_dock(element) != CernDockStyle_None
            && cern_iarranged_element_participates_in_layout(element)) == result
           && "Error detected in cern_common_properties_get_needs_dock_layout(...).");

  return result;
}

gboolean
cern_common_properties_get_self_auto_size_in_default_layout(CernIArrangedElement *element) {
  CernBitVector32 state
    = cern_common_properties_get_layout_state(element);
  return cern_bit_vector32_section_get(&state, SelfAutoSizingSection) == 1;
}

void
cern_common_properties_set_auto_size_mode(CernIArrangedElement *element,
                                          CernAutoSizeMode mode) {
  CernBitVector32 state = cern_common_properties_get_layout_state(element);
  cern_bit_vector32_section_set(&state, AutoSizeModeSection, CernAutoSizeMode_GrowAndShrink ? 1 : 0);
  cern_common_properties_set_layout_state(element, state);
}

gboolean
cern_common_properties_should_self_size(CernIArrangedElement *element) {
  if (cern_common_properties_get_auto_size(element)) {
    if (CERN_IS_CONTROL(cern_iarranged_element_get_container(element))) {
      CernControl *ctl = CERN_CONTROL(cern_iarranged_element_get_container(element));
      if(CERN_IS_DEFAULT_LAYOUT(cern_control_get_layout_engine(ctl)) {
        return cern_common_properties_get_self_auto_size_in_default_layout(element);
      }
    }

    return FALSE;
  }

  return TRUE;
}

void
cern_common_properties_set_should_self_size_in_default_layout(CernIArrangedElement *element,
                                                              gboolean value) {
  g_assert(value != cern_common_properties_get_self_auto_size_in_default_layout(element)
           && "PERF: Caller should guard against setting auto size to original value.");
  CernBitVector32 state = cern_common_properties_get_layout_state(element);
  cern_bit_vector32_section_set(&state, SelfAutoSizingSection, value? 1: 0);
  cern_common_properties_set_layout_state(element, state);

  g_assert(cern_common_properties_get_self_auto_size_in_default_layout(element) == value
           && "Error detected setting auto size.");
}

CernAnchorStyles
cern_common_properties_x_get_anchor(CernIArrangedElement *element) {
  CernBitVector32 state = cern_common_properties_get_layout_state(element);
  CernAnchorStyles value
    = (CernAnchorStyles) cern_bit_vector32_section_get(&state, DockAndAnchorSection);
  DockAnchorMode mode
    = (DockAnchorMode) cern_bit_vector32_section_get(&state, DockModeSection);

  value
    = mode == DockAnchorMode_Anchor
      ? cern_common_properties_x_translate_anchor_value(value)
      : cern_common_properties_get_default_anchor();

  g_assert(mode == DockAnchorMode_Anchor || value == cern_common_properties_get_default_anchor()
           && "cern_common_properties_x_get_anchor(...) needs to return "
              "cern_common_properties_get_default_anchor() when docked.");

  return value;
}

gboolean
cern_common_properties_x_get_auto_size_and_anchored(CernIArrangedElement *element) {
  CernBitVector32 state = cern_common_properties_get_layout_state(element);

  if (cern_bit_vector32_section_get(&state, SelfAutoSizingSection) != 0) {
    return FALSE;
  }

  gboolean result
    = cern_bit_vector32_section_get(&state, AutoSizeModeSection) != 0
      && (cern_bit_vector32_section_get(&state, DockModeSection) == DockAnchorMode_Anchor);

  g_assert(result == (cern_common_properties_get_auto_size(element) && cern_common_properties_x_get_dock(element) == CernDockStyle_None)
           && "Error detected in cern_common_properties_x_get_auto_size_and_anchored(...).");

  return result;
}

CernDockStyle
cern_common_properties_x_get_dock(CernIArrangedElement *element) {
  CernBitVector32 state = cern_common_properties_get_layout_state(element);

  CernDockStyle value = (CernDockStyle) cern_bit_vector32_section_get(&state, DockAndAnchorSection);
  DockAnchorMode mode = (DockAnchorMode) cern_bit_vector32_section_get(&state, DockModeSection);

  value = mode == DockAnchorMode_Dock ? value : cern_common_properties_get_default_dock();

  /* TODO: Asser invalid values. */

  return value;
}

void
cern_common_properties_x_set_anchor(CernIArrangedElement *element, CernAnchorStyles value) {
  g_assert(value != cern_common_properties_x_get_anchor(element)
           && "PERF: Caller should guard against setting anchor to original value.");

  CernBitVector32 state = cern_common_properties_get_layout_state(element);

  cern_bit_vector32_section_set(&state, DockAndAnchorSection, 
                                (gint32) cern_common_properties_x_translate_anchor_value(value));
  cern_bit_vector32_section_set(&state, DockModeSection, (gint32) DockAnchorMode_Anchor);
  cern_common_properties_set_layout_state(element, state);

  /* TODO: Assetrt values... */
}

void
cern_common_properties_x_set_dock(CernIArrangedElement *element, CernDockStyle value) {
  CernBitVector32 state = cern_common_properties_get_layout_state(element);

  cern_bit_vector32_section_set(&state, DockAndAnchorSection, (gint32) value);
  cern_bit_vector32_section_set(&state, DockModeSection, 
                                (gint32) (value == CernDockStyle_None
                                            ? DockAnchorMode_Anchor
                                            : DockAnchorMode_Dock));

  cern_common_properties_set_layout_state(element, state);

  /* TODO: Assert values... */
}

gboolean
cern_common_properties_get_flow_break(CernIArrangedElement *element) {
  CernBitVector32 state = cern_common_properties_get_layout_state(element);
  return cern_bit_vector32_section_get(&state, FlowBreakSection) == 1;
}

void
cern_common_properties_set_flow_break(CernIArrangedElement *element, gboolean value) {
  CernBitVector32 state = cern_common_properties_get_layout_state(element);

  cern_bit_vector32_section_set(&state, FlowBreakSection, value ? 1 : 0);

  cern_common_properties_set_layout_state(element, state);

  cern_layout_transaction_do_layout(cern_iarranged_element_get_container(element),
                                    element, cern_property_names_flow_break());
}

CernSize
cern_common_properties_get_layout_bounds(CernIArrangedElement *element) {
  CernPropertyStore *store = cern_iarranged_element_get_properties(element);

  gboolean found = FALSE;
  
  CernSize size = cern_property_store_try_get_size(store, LayoutBoundsProperty, &found);

  if (found) {
    return size;
  }

  return (CernSize) {
    .width = 0,
    .height = 0
  };
}

void
cern_common_properties_set_layout_bounds(CernIArrangedElement *element, CernSize *value) {
  CernPropertyStore *store = cern_iarranged_element_get_properties(element);

  cern_property_store_set_size(store, LayoutBoundsProperty,
                               (CernSize) { .width = value->width, height = value->height });
}

gboolean
cern_common_properties_get_has_layout_bounds(CernIArrangedElement *element) {
  CernPropertyStore *store = cern_iarranged_element_get_properties(element);

  gboolean found = FALSE;

  /* calm down the compiler not used return value */
  (void) cern_property_store_try_get_size(store, LayoutBoundsProperty, &found);

  return found;
}

CernBitVector32
cern_common_properties_get_layout_state(CernIArrangedElement *element) {
  CernPropertyStore *store = cern_iarranged_element_get_properties(element);
  return (CernBitVector32) {
    .data = cern_property_store_get_integer(store, LayoutStateProperty);
  };
}

void
cern_common_properties_set_layout_state(CernIArrangedElement *element, CernBitVector32 state) {
  CernPropertyStore *store = cern_iarranged_element_get_properties(element);
  cern_property_store_set_integer(store, LayoutStateProperty, state.data);
}

