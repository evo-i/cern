#include "cern/ui/layout/default_layout.h"
#include "cern/drawing/image.h"
#include "cern/drawing/point.h"
#include "cern/drawing/rectangle.h"
#include "cern/ui/anchor_styles.h"
#include "cern/ui/auto_size_mode.h"
#include "cern/ui/bounds_specified.h"
#include "cern/ui/control.h"
#include "cern/ui/dock_style.h"
#include "cern/ui/layout/iarranged_element.h"
#include "cern/ui/layout/layout_engine.h"
#include "cern/ui/common_properties.h"
#include "cern/ui/padding.h"
#include "cern/ui/property_store.h"
#include "cern/ui/mdi_client.h"
#include <glib-object.h>
#include <glib.h>
#include <glibconfig.h>
#include <winscard.h>

typedef enum _GrowthDirection {
  GrowthDirection_None = 0x0,
  GrowthDirection_Upward = 0x01,
  GrowthDirection_Downward = 0x02,
  GrowthDirection_Left = 0x04,
  GrowthDirection_Right = 0x08
} GrowthDirection;

static
gint32
LayoutInfoProperty = 0;

static
gint32
CachedBoundsProperty = 0;

#define CERN_TYPE_ANCHOR_INFO cern_anchor_info_get_type ()

G_DECLARE_FINAL_TYPE(CernAnchorInfo, cern_anchor_info, CERN, ANCHOR_INFO, GObject);

typedef struct _CernAnchorInfo {
  GObject parent_instance;

  gint32 left;
  gint32 top;
  gint32 right;
  gint32 bottom;
} CernAnchorInfo;

G_DEFINE_FINAL_TYPE(CernAnchorInfo, cern_anchor_info, G_TYPE_OBJECT);

static
void
cern_anchor_info_init(CernAnchorInfo *self) { }

void
cern_anchor_info_init_class(CernAnchorInfoClass *klass) { }

CernAnchorInfo *
cern_anchor_info_new(void) {
  return g_object_new(CERN_TYPE_ANCHOR_INFO, NULL);
}

#define cern_anchor_info_set_(direction) \
static void \
cern_anchor_info_set_##direction(CernAnchorInfo *self, gint32 value) {self->direction = value;}

#define cern_anchor_info_get_(direction) \
static gint32 \
cern_anchor_info_get_##direction(CernAnchorInfo *self) { return self->direction; }

cern_anchor_info_set_(top);
cern_anchor_info_set_(left);
cern_anchor_info_set_(right);
cern_anchor_info_set_(bottom);

cern_anchor_info_get_(top);
cern_anchor_info_get_(left);
cern_anchor_info_get_(right);
cern_anchor_info_get_(bottom);

void
cern_anchor_info_set(CernAnchorInfo *self, gint32 l, gint32 t, gint32 r, gint32 b) {
  cern_anchor_info_set_left(self, l);
  cern_anchor_info_set_top(self, t);
  cern_anchor_info_set_right(self, r);
  cern_anchor_info_set_bottom(self, b);
}

typedef struct _CernDefaultLayoutPrivate {
  gpointer opaque; 
} CernDefaultLayoutPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(CernDefaultLayout, cern_default_layout, CERN_TYPE_LAYOUT_ENGINE);

CernDefaultLayout *
cern_default_layout_get_instance(void) {
  static CernDefaultLayout *instance = NULL;

  if (instance == NULL 
      || !CERN_IS_DEFAULT_LAYOUT(instance)) {
    instance = g_object_new(CERN_TYPE_DEFAULT_LAYOUT, NULL);
  }

  return instance;
}

static
void
cern_default_layout_set_anchor_info(CernIArrangedElement *element, CernAnchorInfo *info) {
  CernPropertyStore *store = cern_iarranged_element_get_properties(element);
  cern_property_store_set_object(store, LayoutInfoProperty, info);
}

static
CernAnchorInfo *
cern_default_layout_get_anchor_info(CernIArrangedElement *element) {
  CernPropertyStore *store
    = cern_iarranged_element_get_properties(element);
  gpointer obj
    = cern_property_store_get_object(store, LayoutInfoProperty);

  CernAnchorInfo *info = CERN_ANCHOR_INFO(obj);

  return info;
}

static
CernRectangle
cern_default_layout_get_cached_bounds(CernIArrangedElement *element) {
  if (cern_iarranged_element_get_container(element) != NULL) {
    CernPropertyStore *store = cern_iarranged_element_get_properties(element);
    GHashTable *table = cern_property_store_get_object(store, CachedBoundsProperty);

    if (table != NULL) {
      CernRectangle *bounds
        = g_hash_table_lookup(table, element);

      if (bounds != NULL) {
        return
          cern_rectangle_create(bounds->x, bounds->y,
                                bounds->width, bounds->height);
      }
    }
  }

  return cern_iarranged_element_get_bounds(element);
}

static
CernRectangle
cern_default_layout_get_anchor_destionation(CernIArrangedElement *element,
                                            CernRectangle *display_rect,
                                            gboolean measure_only) {
  CernAnchorInfo *layout
    = cern_default_layout_get_anchor_info(element);

  gint32 left = layout->left + display_rect->x;
  gint32 top = layout->top + display_rect->y;
  gint32 right = layout->right + display_rect->x;
  gint32 bottom = layout->bottom + display_rect->y;

  CernAnchorStyles anchor
    = cern_default_layout_get_anchor(element);

  if (cern_default_layout_get_is_anchored(anchor, CernAnchorStyles_Right)) {
    right += display_rect->width;

    if (cern_default_layout_get_is_anchored(anchor, CernAnchorStyles_Left)) {
      left += display_rect->width;
    }
  } else if (!cern_default_layout_get_is_anchored(anchor, CernAnchorStyles_Left)) {
    right += (display_rect->width / 2);
    left += (display_rect->width / 2);
  }

  if (cern_default_layout_get_is_anchored(anchor, CernAnchorStyles_Bottom)) {
    bottom += display_rect->height;

    if (!cern_default_layout_get_is_anchored(anchor, CernAnchorStyles_Top)) {
      top += display_rect->height;
    }
  } else if (!cern_default_layout_get_is_anchored(anchor, CernAnchorStyles_Top)) {
    bottom += (display_rect->height / 2);
    top += (display_rect->height / 2);
  }

  if (!measure_only) {
    if (right < left) {
      right = left;
    }

    if (bottom < top) {
      bottom = top;
    }
  } else {
    CernRectangle cached_bounds
      = cern_default_layout_get_cached_bounds(element);

    CernRectangle element_bounds
      = cern_iarranged_element_get_bounds(element);

    if (right < left
        || cached_bounds.width != element_bounds.width
        || cached_bounds.x != element_bounds.x) {
      if (!cern_rectangle_equals(&cached_bounds, &element_bounds)) {
        left = MAX(ABS(left), ABS(cached_bounds.width));
      }

      right 
        = left 
          + MAX(element_bounds.width, cached_bounds.width) 
          + ABS(right);
    } else {
      left 
        = left > 0 
          ? left 
          : cern_rectangle_get_left(&element_bounds);

      right 
        = right > 0 
          ? right 
          : cern_rectangle_get_right(&element_bounds) 
            + ABS(right);
    }
  }

  return
    cern_rectangle_create(left, top,
                          right - left, bottom - top);
}

gboolean
ptr_eq(gconstpointer p1, gconstpointer p2) {
  return p1 == p2;
}

static
void
cern_default_layout_set_cached_bounds(CernIArrangedElement *element,
                                      CernRectangle *bounds) {
  CernRectangle cached_bounds
    = cern_default_layout_get_cached_bounds(element);

  if (cern_rectangle_equals(bounds, &cached_bounds)) {
    CernIArrangedElement *container
      = cern_iarranged_element_get_container(element);

    CernPropertyStore *store
      = cern_iarranged_element_get_properties(container);

    GHashTable *table
      = cern_property_store_get_object(store, CachedBoundsProperty);

    if (table == NULL) {
      table = g_hash_table_new_full(g_direct_hash, ptr_eq, NULL, (GDestroyNotify) cern_rectangle_free);
      cern_property_store_set_object(store, CachedBoundsProperty, table);
    }

    g_hash_table_remove(table, element);

    g_hash_table_insert(table, element, cern_rectangle_copy(bounds));
  }
}

static
void
cern_default_layout_layout_anchored_controls(CernIArrangedElement *container) {
  CernRectangle display_rectangle
    = cern_iarranged_element_get_display_rectangle(container);

  if (cern_common_properties_get_auto_size(container)
      && (display_rectangle.width == 0 || display_rectangle.height == 0)) {
    return;
  }

  GPtrArray *children
    = cern_iarranged_element_get_children(container);

  for (gsize i = children->len; i < children->len - 1; i--) {
    CernIArrangedElement *element = g_ptr_array_index(children, i);

    if (cern_common_properties_get_needs_anchor_layout(element)) {
      CernRectangle anchor_dest
        = cern_default_layout_get_anchor_destionation(element,
                                                      &display_rectangle,
                                                      FALSE);
      cern_default_layout_set_cached_bounds(element, &anchor_dest);
    }
  }
}

static
GrowthDirection
cern_default_layout_get_growth_direction(CernIArrangedElement *element) {
  CernAnchorStyles anchor
    = cern_default_layout_get_anchor(element);

  GrowthDirection growth_direction = GrowthDirection_None;
  
  if ((anchor & CernAnchorStyles_Right) != CernAnchorStyles_None
      && (anchor & CernAnchorStyles_Left) == CernAnchorStyles_None) {
    growth_direction |= GrowthDirection_Left;
  } else {
    growth_direction |= GrowthDirection_Right;
  }

  if ((anchor & CernAnchorStyles_Bottom)
      && !(anchor & CernAnchorStyles_Top)) {
    growth_direction |= GrowthDirection_Upward;
  } else {
    growth_direction |= GrowthDirection_Downward;
  }

  return growth_direction;
}

static
CernRectangle
cern_default_layout_get_growth_bounds(CernIArrangedElement *element,
                                      CernSize *new_size) {
  GrowthDirection direction
    = cern_default_layout_get_growth_direction(element);

  CernRectangle old_bounds
    = cern_default_layout_get_cached_bounds(element);

  CernPoint location
    = cern_rectangle_get_location(&old_bounds);

  if ((direction & GrowthDirection_Left)) {
    location.x -= new_size->width - old_bounds.width;
  }

  if ((direction & GrowthDirection_Upward)) {
    location.y -= new_size->height - old_bounds.height;
  }

  CernRectangle new_bounds 
    = cern_rectangle_create_with_point_and_size(&location, new_size);

  return new_bounds;
}

static
void
cern_default_layout_layout_auto_sized_controls(CernIArrangedElement *container) {
  GPtrArray *children
    = cern_iarranged_element_get_children(container);

  for (gsize i = children->len - 1; i >= 0; --i) {
    CernIArrangedElement *element
      = g_ptr_array_index(children, i);

    if (cern_common_properties_x_get_auto_size_and_anchored(element)) {
      CernRectangle bounds
        = cern_default_layout_get_cached_bounds(element);
      CernAnchorStyles anchor = cern_default_layout_get_anchor(element);
      CernSize proposed_constraints
        = cern_layout_utils_get_max_size();

      if ((anchor & (CernAnchorStyles_Left | CernAnchorStyles_Right))
            == (CernAnchorStyles_Left | CernAnchorStyles_Right)) {
        proposed_constraints.width = bounds.width;
      }

      if ((anchor & (CernAnchorStyles_Top | CernAnchorStyles_Bottom))
            == (CernAnchorStyles_Top | CernAnchorStyles_Bottom)) {
        proposed_constraints.height = bounds.height;
      }

      CernSize pref_size
        = cern_iarranged_element_get_preffered_size(element,
                                                    &proposed_constraints);

      CernRectangle new_bounds = bounds;

      if (cern_common_properties_get_auto_size_mode(element)
            == CernAutoSizeMode_GrowAndShrink) {
        new_bounds
          = cern_default_layout_get_growth_bounds(element, &pref_size);
      } else {
        if (bounds.width < pref_size.width
            || bounds.height < pref_size.height) {
          CernSize bounds_size = cern_rectangle_get_size(&bounds);
          CernSize new_size
            = cern_layout_utils_union_sizes(&bounds_size, &pref_size);

          new_bounds
            = cern_default_layout_get_growth_bounds(element, &new_size);
        }
      }

      if (!cern_rectangle_equals(&new_bounds, &bounds)) {
        cern_default_layout_set_cached_bounds(element, &new_bounds);
      }
    }
  }
}

static
void
cern_default_layout_clear_cached_bounds(CernIArrangedElement *element) {
  CernPropertyStore *store 
    = cern_iarranged_element_get_properties(element);
  
  GHashTable *table
    = cern_property_store_get_object(store, CachedBoundsProperty);

  if (table != NULL) {
    g_hash_table_unref(table);
  }

  cern_property_store_set_object(store, CachedBoundsProperty, NULL);
}

static
void
cdlacb_table(gpointer key, gpointer value, gpointer user_data) {
  CernIArrangedElement *element = key;
  CernRectangle *bounds = value;
  cern_iarranged_element_set_bounds(element, bounds, CernBoundsSpecified_None);
}

static
void
cern_default_layout_apply_cached_bounds(CernIArrangedElement *container) {
  if (cern_common_properties_get_auto_size(container)) {
    CernRectangle display_rectangle
      = cern_iarranged_element_get_display_rectangle(container);

    if (display_rectangle.width == 0
        || display_rectangle.height == 0) {
      cern_default_layout_clear_cached_bounds(container);
      return;
    }
  }

  CernPropertyStore *store
    = cern_iarranged_element_get_properties(container);

  GHashTable *table 
    = cern_property_store_get_object(store, CachedBoundsProperty);

  if (table != NULL) {
    g_hash_table_foreach(table, cdlacb_table, NULL);
    cern_default_layout_clear_cached_bounds(container);
  }
}

static
CernSize
cern_default_layout_get_anchor_preferred_size(CernIArrangedElement *container) {
  CernSize pref_size = { 0 };

  GPtrArray *children
    = cern_iarranged_element_get_children(container);

  for (gsize i = children->len - 1; i >= 0; --i) {
    CernIArrangedElement *element
      = g_ptr_array_index(children, i);

    if (!cern_common_properties_get_needs_dock_layout(element)
        && cern_iarranged_element_participates_in_layout(element)) {
      CernAnchorStyles anchor
        = cern_default_layout_get_anchor(element);

      CernPadding margin
        = cern_common_properties_get_margin(element);

      CernRectangle cb
        = cern_default_layout_get_cached_bounds(element);

      CernRectangle element_space
        = cern_layout_utils_inflate_rect(&cb, &margin);

      if (cern_default_layout_get_is_anchored(anchor, CernAnchorStyles_Left)
          && !cern_default_layout_get_is_anchored(anchor, CernAnchorStyles_Right)) {
        pref_size.width = MAX(pref_size.width, element_space.width);
      }

      if (!cern_default_layout_get_is_anchored(anchor, CernAnchorStyles_Bottom)) {
        pref_size.height = MAX(pref_size.height, element_space.height);
      }

      if (cern_default_layout_get_is_anchored(anchor, CernAnchorStyles_Right)) {
        CernRectangle empty = { 0 };

        CernRectangle anchor_dest
          = cern_default_layout_get_anchor_destionation(element, &empty, TRUE);

        if (anchor_dest.width < 0) {
          pref_size.width 
            = MAX(pref_size.width,
                  cern_rectangle_get_right(&element_space) + anchor_dest.width);
        } else {
          pref_size.width
            = MAX(pref_size.width, cern_rectangle_get_right(&anchor_dest));
        }
      }

      if (cern_default_layout_get_is_anchored(anchor, CernAnchorStyles_Bottom)) {
        CernRectangle empty = { 0 };

        CernRectangle anchor_dest
          = cern_default_layout_get_anchor_destionation(element, &empty, TRUE);

        if (anchor_dest.height < 0) {
          pref_size.height 
            = MAX(pref_size.height, 
                  cern_rectangle_get_bottom(&element_space) + anchor_dest.height);
        } else {
          pref_size.height
            = MAX(pref_size.height, cern_rectangle_get_bottom(&anchor_dest));
        }
      }
    }
  }

  return pref_size;
}

static
void
cern_default_layout_update_anchor_info(CernIArrangedElement *element) {
  CernAnchorInfo *anchor_info
    = cern_default_layout_get_anchor_info(element);

  if (anchor_info == NULL) {
    anchor_info = cern_anchor_info_new();
    cern_default_layout_set_anchor_info(element, anchor_info);
  }

  if (cern_common_properties_get_needs_anchor_layout(element)
      && cern_iarranged_element_get_container(element) != NULL) {
    CernRectangle bounds = cern_default_layout_get_cached_bounds(element);

    CernAnchorInfo *old_anchor_info = cern_anchor_info_new();

    cern_anchor_info_set(old_anchor_info,
                         anchor_info->left, anchor_info->top,
                         anchor_info->right, anchor_info->bottom);

    CernRectangle elb = cern_iarranged_element_get_bounds(element);

    cern_anchor_info_set(anchor_info,
                         cern_rectangle_get_left(&elb),
                         cern_rectangle_get_top(&elb),
                         cern_rectangle_get_right(&elb),
                         cern_rectangle_get_bottom(&elb));

    CernIArrangedElement *container = cern_iarranged_element_get_container(element);
    CernRectangle parent_display_rect 
      = cern_iarranged_element_get_display_rectangle(container);

    gint32 parent_width = parent_display_rect.width;
    gint32 parent_height = parent_display_rect.height;

    anchor_info->left -= parent_display_rect.x;
    anchor_info->top -= parent_display_rect.y;
    anchor_info->right -= parent_display_rect.x;
    anchor_info->bottom -= parent_display_rect.y;

    CernAnchorStyles anchor = cern_default_layout_get_anchor(element);

    if (cern_default_layout_get_is_anchored(anchor, CernAnchorStyles_Right)) {
      if (cern_dpi_hepler_get_enable_anchor_layout_high_dpi_improvements()
          && (anchor_info->right - parent_width) > 0
          && (old_anchor_info->right < 0)) {
        anchor_info->right = old_anchor_info->right;
        anchor_info->left = old_anchor_info->right - bounds.width;
      } else {
        anchor_info->right -= parent_width;

        if (!cern_default_layout_get_is_anchored(anchor, CernAnchorStyles_Left)) {
          anchor_info->left -= parent_width;
        }
      }
    } else if (!cern_default_layout_get_is_anchored(anchor, CernAnchorStyles_Left)) {
      anchor_info->right -= (parent_width / 2);
      anchor_info->left -= (parent_width / 2);
    }

    if (cern_default_layout_get_is_anchored(anchor, CernAnchorStyles_Bottom)) {
      if (cern_dpi_hepler_get_enable_anchor_layout_high_dpi_improvements()
          && (anchor_info->bottom - parent_height > 0)
          && (old_anchor_info->bottom < 0)) {
        anchor_info->bottom
          = old_anchor_info->bottom;
        anchor_info->top
          = old_anchor_info->bottom - bounds.height;
      } else {
        anchor_info->bottom -= parent_height;

        if (!cern_default_layout_get_is_anchored(anchor, CernAnchorStyles_Top)) {
          anchor_info->top -= parent_height;
        }
      }
    } else if (!cern_default_layout_get_is_anchored(anchor, CernAnchorStyles_Top)) {
      anchor_info->bottom -= (parent_height / 2);
      anchor_info->top -= (parent_height / 2);
    }
  }
}

static
void
cern_default_layout_x_layout_docked_control(CernIArrangedElement *element,
                                            CernRectangle *new_element_bounds,
                                            gboolean mo,
                                            CernSize *preferred_size,
                                            CernRectangle *remaining_bounds) {
  if (mo) {
    CernSize needed_size = {
      .width = MAX(0, new_element_bounds->width - remaining_bounds->width),
      .height = MAX(0, new_element_bounds->height - remaining_bounds->height)
    };

    CernDockStyle dock_style = cern_default_layout_get_dock(element);

    if ((dock_style == CernDockStyle_Top) 
        || (dock_style == CernDockStyle_Bottom)) {
      needed_size.width = 0;
    }

    if ((dock_style == CernDockStyle_Right)
        || (dock_style == CernDockStyle_Left)) {
      needed_size.height = 0;
    }

    if (dock_style != CernDockStyle_Fill) {
      preferred_size->width += needed_size.width;
      preferred_size->height += needed_size.height;

      CernSize remain_size = cern_rectangle_get_size(remaining_bounds);

      CernSize new_size = {
        .width = remain_size.width + needed_size.width,
        .height = remain_size.height + needed_size.height
      };

      cern_rectangle_set_size(remaining_bounds, &new_size);
    } else if (dock_style == CernDockStyle_Fill 
               && cern_common_properties_get_auto_size(element)) {
      CernSize element_pref_size
        = cern_iarranged_element_get_preffered_size(element, preferred_size);

      CernSize tmp = cern_rectangle_get_size(remaining_bounds);

      CernSize rem_bs = {
        .width = tmp.width + element_pref_size.width,
        .height = tmp.height + element_pref_size.height
      };

      element_pref_size.width += rem_bs.width;
      element_pref_size.height += rem_bs.height;

      preferred_size->width += element_pref_size.width;
      preferred_size->height += element_pref_size.height;
    }

  } else {
    CernPropertyStore *store = cern_iarranged_element_get_properties(element);
    cern_iarranged_element_set_bounds(element, new_element_bounds, CernBoundsSpecified_None);
  }
}

static
CernSize
cern_default_layout_x_get_docked_size(CernIArrangedElement *element,
                                      CernSize *remain_size,
                                      CernSize *constraints,
                                      gboolean mo) {
  CernSize desired_size = { 0 };

  if (cern_common_properties_get_auto_size(element)) {
    desired_size = cern_iarranged_element_get_preffered_size(element, constraints);
  } else {
    CernRectangle bounds = cern_iarranged_element_get_bounds(element);
    desired_size = cern_rectangle_get_size(&bounds);
  }

  return desired_size;
}

static
CernSize
cern_default_layout_get_vertical_docked_size(CernIArrangedElement *element,
                                             CernSize *remain_size,
                                             gboolean mo) {
  CernSize new_size
    = cern_default_layout_x_get_docked_size(element,
                                            remain_size,
                                            &(CernSize) { 
                                              .width = remain_size->width, 
                                              .height = 1 
                                            },
                                            mo);

  if (!mo) {
    new_size.width = remain_size->width;
  } else {
    new_size.width = MAX(new_size.width, remain_size->width);
  }

  return new_size;
}

static
CernSize
cern_default_layout_get_horizontal_docked_size(CernIArrangedElement *element,
                                               CernSize *remain_size,
                                               gboolean mo) {
  CernSize new_size
    = cern_default_layout_x_get_docked_size(element,
                                            remain_size,
                                            &(CernSize) { .width = 1, .height = remain_size->height },
                                            mo);

  if (!mo) {
    new_size.height = remain_size->height;
  } else {
    new_size.height = MAX(new_size.height, remain_size->height);
  }

  return new_size;
}

static
CernSize
cern_default_layout_layout_docked_controls(CernIArrangedElement *container, gboolean mo) {
  CernRectangle remaining_bounds 
    = mo 
      ? (CernRectangle) { 0 }
      : cern_iarranged_element_get_display_rectangle(container);

  CernSize preferred_size = { 0 };

  CernIArrangedElement *mdi_client = NULL;

  GPtrArray *children = cern_iarranged_element_get_children(container);

  for (gsize i = 0; i < children->len; i++) {
    CernIArrangedElement *element = g_ptr_array_index(children, i);

    if (cern_common_properties_get_needs_dock_layout(element)) {
      switch (cern_default_layout_get_dock(element)) {
        case CernDockStyle_Top: {
          CernSize remain_bounds_size
            = cern_rectangle_get_size(&remaining_bounds);

          CernSize element_size
            = cern_default_layout_get_vertical_docked_size(element, &remain_bounds_size, mo);

          CernRectangle new_element_bounds = {
            .x = remaining_bounds.x,
            .y = remaining_bounds.y,
            .width = element_size.width,
            .height = element_size.height
          };

          cern_default_layout_x_layout_docked_control(element, &new_element_bounds, mo, &preferred_size, &remaining_bounds);

          
          CernRectangle temp = cern_iarranged_element_get_bounds(element);

          remaining_bounds.y += temp.height;
          remaining_bounds.height -= temp.height;
        } break;
        case CernDockStyle_Bottom: {
          CernSize remain_bounds_size
            = cern_rectangle_get_size(&remaining_bounds);

          CernSize element_size
            = cern_default_layout_get_vertical_docked_size(element, &remain_bounds_size, mo);

          CernRectangle new_element_bounds = {
            .x = remaining_bounds.x,
            .y = cern_rectangle_get_bottom(&remaining_bounds) - element_size.height,
            .width = element_size.width,
            .height = element_size.height
          };

          cern_default_layout_x_layout_docked_control(element, &new_element_bounds, mo, &preferred_size, &remaining_bounds);

          CernRectangle temp = cern_iarranged_element_get_bounds(element);

          remaining_bounds.height -= temp.height;
        } break;
        case CernDockStyle_Left: {
          CernSize remain_bounds_size
            = cern_rectangle_get_size(&remaining_bounds);

          CernSize element_size
            = cern_default_layout_get_horizontal_docked_size(element, &remain_bounds_size, mo);

          CernRectangle new_element_bounds = {
            .x = remaining_bounds.x,
            .y = remaining_bounds.y,
            .width = element_size.width,
            .height = element_size.height
          };

          cern_default_layout_x_layout_docked_control(element, &new_element_bounds, mo, &preferred_size, &remaining_bounds);

          CernRectangle temp = cern_iarranged_element_get_bounds(element);

          remaining_bounds.x += temp.width;
          remaining_bounds.width -= temp.width;
        } break;
        case CernDockStyle_Right: {
          CernSize remain_bounds_size
            = cern_rectangle_get_size(&remaining_bounds);

          CernSize element_size
            = cern_default_layout_get_horizontal_docked_size(element, &remain_bounds_size, mo);

          CernRectangle new_element_bounds = {
            .x = cern_rectangle_get_right(&remaining_bounds) - remaining_bounds.width,
            .y = remaining_bounds.y,
            .width = element_size.width,
            .height = element_size.height
          };

          cern_default_layout_x_layout_docked_control(element, &new_element_bounds, mo, &preferred_size, &remaining_bounds);

          CernRectangle temp = cern_iarranged_element_get_bounds(element);

          remaining_bounds.width += temp.width;
        } break;
        case CernDockStyle_Fill: {
          if (CERN_IS_MDI_CLIENT(element)) {
            g_assert(mdi_client == NULL
                     && "How did we end up with multiple CernMdiClient's?");
            mdi_client = element;
          } else {
            CernSize element_size
              = cern_rectangle_get_size(&remaining_bounds);

            CernRectangle new_element_bounds = {
              .x = remaining_bounds.x,
              .y = remaining_bounds.y,
              .width = remaining_bounds.width,
              .height = remaining_bounds.height
            };

            cern_default_layout_x_layout_docked_control(element, &new_element_bounds, mo, &preferred_size, &remaining_bounds);
          }
        } break;
        default: {
          g_critical("Unsupported value for dock.");
          return (CernSize) { 0 };
        }
      }
    }

    if (mdi_client != NULL) {
      cern_default_layout_set_cached_bounds(mdi_client, &remaining_bounds);
    }
  }

  return preferred_size;
}

static
gboolean
cern_default_layout_x_layout(CernIArrangedElement *container,
                             gboolean measure_only,
                             CernSize *out_preferred_size) {
  GPtrArray *collection = cern_iarranged_element_get_children(container);

  *out_preferred_size = (CernSize) { .width = -7103, .height = -7105 };

  if (!measure_only && collection->len == 0) {
    return cern_common_properties_get_auto_size(container);
  }

  gboolean dock = FALSE;
  gboolean anchor = FALSE;
  gboolean auto_size = FALSE;

  for (gsize i = 0; i < collection->len; ++i) {
    CernIArrangedElement *current = g_ptr_array_index(collection, i);

    if (cern_common_properties_get_needs_dock_and_anchor_layout(current)) {
      if (!dock
          && cern_common_properties_get_needs_dock_layout(current)) {
        dock = TRUE;
      }

      if (!anchor
          && cern_common_properties_get_needs_anchor_layout(current)) {
        anchor = TRUE;
      }

      if (!auto_size
          && cern_common_properties_x_get_auto_size_and_anchored(current)) {
        auto_size = TRUE;
      }
    }
  }

  CernSize preferred_size_for_docking = { 0 };
  CernSize preferred_size_for_anchoring = { 0 };

  if (dock) {
    preferred_size_for_docking
      = cern_default_layout_layout_docked_controls(container, measure_only);
  }

  if (anchor && !measure_only) {
    cern_default_layout_layout_anchored_controls(container);
  }

  if (auto_size) {
    cern_default_layout_layout_auto_sized_controls(container);
  }

  if (!measure_only) {
    cern_default_layout_apply_cached_bounds(container);
  } else {
    preferred_size_for_anchoring
      = cern_default_layout_get_anchor_preferred_size(container);

    CernPadding container_padding = { 0 };

    CernControl *control = NULL;

    if (CERN_IS_CONTROL(container)) {
      control = CERN_CONTROL(container);
      container_padding = cern_control_get_padding(control);
    } else {
      container_padding = cern_common_properties_get_padding(container, &(CernPadding) { 0 });
    }

    preferred_size_for_anchoring.width -= container_padding.left;
    preferred_size_for_anchoring.height -= container_padding.top;

    cern_default_layout_clear_cached_bounds(container);

    (*out_preferred_size)
      = cern_layout_utils_union_sizes(&preferred_size_for_docking, 
                                      &preferred_size_for_anchoring);
  }

  return cern_common_properties_get_auto_size(container);
}

static
gboolean
real_cern_default_layout_core(CernLayoutEngine *parent,
                              CernIArrangedElement *container,
                              CernLayoutEventArgs *args) {
  CernLayoutEngineClass *parent_class
    = CERN_LAYOUT_ENGINE_CLASS(cern_default_layout_parent_class);

  parent_class->layout_core(parent, container, args);

  CernSize garbage = { 0 };
  return cern_default_layout_x_layout(container, FALSE, &garbage);
}

static
void
real_cern_default_init_layout_core(CernLayoutEngine *parent,
                                   CernIArrangedElement *element,
                                   CernBoundsSpecified specified) {
  CernLayoutEngineClass *parent_class
    = CERN_LAYOUT_ENGINE_CLASS(cern_default_layout_parent_class);

  parent_class->init_layout_core(parent, element, specified);


  if (specified != CernBoundsSpecified_None
      && cern_common_properties_get_needs_anchor_layout(element)) {
    cern_default_layout_update_anchor_info(element);
  }
}

gboolean
cern_default_layout_get_has_cached_bounds(CernIArrangedElement *element) {
  CernPropertyStore *store
    = cern_iarranged_element_get_properties(element);

  return
    element != NULL
      && cern_property_store_get_object(store, CachedBoundsProperty) != NULL;
}

static
CernSize
real_cern_default_get_preferred_size(CernLayoutEngine *parent,
                                     CernIArrangedElement *element,
                                     CernSize *proposed_bounds) {
  CernIArrangedElement *container
    = cern_iarranged_element_get_container(element);

  gboolean has_cached_bounds
    = cern_default_layout_get_has_cached_bounds(container);

  g_assert(!has_cached_bounds);

  CernSize pref_size = { 0 };
  cern_default_layout_x_layout(element, TRUE, &pref_size);
  return pref_size;
}

static
void
cern_defualt_layot_class_init(CernDefaultLayoutClass *klass) {
  CernLayoutEngineClass *parent_class = CERN_LAYOUT_ENGINE_CLASS(klass);

  parent_class->layout_core       = real_cern_default_layout_core;
  parent_class->init_layout_core  = real_cern_default_init_layout_core;
  parent_class->get_prefered_size = real_cern_default_get_preferred_size;
}


CernDockStyle
cern_default_layout_get_dock(CernIArrangedElement *element) {
  return cern_common_properties_x_get_dock(element);
}

void
cern_default_layout_set_dock(CernIArrangedElement *element, CernDockStyle value) {
  if (cern_default_layout_get_dock(element) != value) {
    gboolean dock_needs_layout
      = cern_common_properties_get_needs_dock_layout(element);

    cern_common_properties_x_set_dock(element, value);

    CernControl *container
      = CERN_CONTROL(cern_iarranged_element_get_container(element));

    CernLayoutTransaction *ltr
      = cern_layout_transaction_new(container,
                                    element,
                                    cern_property_names_dock());

    if (value == CernDockStyle_None) {
      if (dock_needs_layout) {
        CernRectangle bounds
          = cern_common_properties_get_specified_bounds(element);

        cern_iarranged_element_set_bounds(element,
                                          &bounds,
                                          CernBoundsSpecified_None);

        cern_default_layout_update_anchor_info(element);
      }
    }

    g_clear_object(&ltr);
  }
}

void
cern_default_layout_scale_anchor_info(CernIArrangedElement *element,
                                      CernSizeF factor) {
  CernAnchorInfo *info
    = cern_default_layout_get_anchor_info(element);
  
  if (info != NULL) {
    info->left    = (gint32) (info->left    * factor.width);
    info->top     = (gint32) (info->top     * factor.height);
    info->right   = (gint32) (info->right   * factor.width);
    info->bottom  = (gint32) (info->bottom  * factor.height);
    cern_default_layout_set_anchor_info(element, info);
  }
}

gboolean
cern_default_layout_get_is_anchored(CernAnchorStyles anchor,
                                    CernAnchorStyles desired_anchor) {
  return (anchor & desired_anchor) == desired_anchor;
}

