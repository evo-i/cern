#ifndef CERN_UI_LAYOUT_UTILS_H
#define CERN_UI_LAYOUT_UTILS_H 1

#include <glib-object.h>
#include "cern/drawing/size.h"
#include "cern/drawing/rectangle.h"
#include "cern/drawing/font.h"
#include "cern/drawing/point.h"
#include "iarranged_element.h"
#include "default_layout.h"
#include "../padding.h"
#include "../text_image_relation.h"
#include "../content_alignment.h"
#include "../anchor_styles.h"
#include "../dock_style.h"


G_BEGIN_DECLS

static
inline
CernAnchorStyles
cern_layout_utils_docking_to_anchor(CernDockStyle dock) {
  switch (dock) {
    case CernDockStyle_None:
      return
        CernAnchorStyles_Top
        | CernAnchorStyles_Left;
    case CernDockStyle_Top:
      return
        CernAnchorStyles_Top
        | CernAnchorStyles_Left
        | CernAnchorStyles_Right;
    case CernDockStyle_Bottom:
      return 
        CernAnchorStyles_Bottom
        | CernAnchorStyles_Left
        | CernAnchorStyles_Right;
    case CernDockStyle_Left:
      return 
        CernAnchorStyles_Left
        | CernAnchorStyles_Top
        | CernAnchorStyles_Bottom;
    case 4:
      return 
        CernAnchorStyles_Right
        | CernAnchorStyles_Top
        | CernAnchorStyles_Bottom;
    case 5:
      return 
        CernAnchorStyles_Right
        | CernAnchorStyles_Top 
        | CernAnchorStyles_Bottom
        | CernAnchorStyles_Left;
    default:
      return CernAnchorStyles_None;
  }
}

static
inline
CernSize
cern_layout_utils_get_max_size(void) {
  return (CernSize) {
    .width = G_MAXINT32,
    .height = G_MAXINT32
  };
}

static
inline
CernSize
cern_layout_utils_get_invalid_size(void) {
  return (CernSize) {
    .width = G_MININT32,
    .height = G_MININT32
  };
}

static
inline
CernRectangle
cern_layout_utils_get_max_rectangle(void) {
  return (CernRectangle) {
    .x = 0,
    .y = 0,
    .width = G_MAXINT32,
    .height = G_MAXINT32
  };
}

static
inline
CernContentAlignment
cern_layout_utils_get_any_top(void) {
  return 
    CernContentAlignment_TopLeft 
      | CernContentAlignment_TopCenter
      | CernContentAlignment_TopRight;
}

static
inline
CernContentAlignment
cern_layout_utils_get_any_bottom(void) {
  return 
    CernContentAlignment_BottomLeft
      | CernContentAlignment_BottomCenter
      | CernContentAlignment_BottomRight;
}

static
inline
CernContentAlignment
cern_layout_utils_get_any_left(void) {
  return 
    CernContentAlignment_BottomLeft
      | CernContentAlignment_TopLeft
      | CernContentAlignment_MiddleLeft;
}

static
inline
CernContentAlignment
cern_layout_utils_get_any_right(void) {
  return 
    CernContentAlignment_TopRight
      | CernContentAlignment_BottomRight
      | CernContentAlignment_MiddleRight;

}

static
inline
CernContentAlignment
cern_layout_utils_get_any_center(void) {
  return 
    CernContentAlignment_TopCenter
      | CernContentAlignment_MiddleCenter
      | CernContentAlignment_BottomCenter;

}

static
inline
CernContentAlignment
cern_layout_utils_get_any_middle(void) {
  return 
    CernContentAlignment_MiddleLeft
      | CernContentAlignment_MiddleCenter
      | CernContentAlignment_MiddleRight;
}

static
inline
CernAnchorStyles
cern_layout_utils_horizontal_anchor_styles(void) {
  return CernAnchorStyles_Left | CernAnchorStyles_Right;
}

static
inline
CernAnchorStyles
cern_layout_utils_vertical_anchor_styles(void) {
  return CernDockStyle_Top | CernAnchorStyles_Bottom;
}

static
inline
CernSize
cern_layout_utils_get_old_largest_string_size_in_collection(CernFont *font,
                                                            GPtrArray *objects) {
  CernSize largest_size = { 0 };

  if (objects != NULL) {
    for (gsize i = 0; i < objects->len; ++i) {
      CernSize max_size = cern_layout_utils_get_max_size();
      CernSize text_size
         = cern_text_renderer_measure_text((gchar *) g_ptr_array_index(objects, i),
                                           font, &max_size, CernTextFormatFlags_SingleLine);
      largest_size.width = MAX(largest_size.width, text_size.width);
      largest_size.height = MAX(largest_size.height, text_size.height);
    }
  }

  return largest_size;
}

gint32
cern_layout_utils_content_alignment_to_index(CernContentAlignment alignment);

static
inline
CernSize
cern_layout_utils_convert_zero_to_unbounded(CernSize size) {
  if (size.width == 0) {
    size.width = G_MAXINT32;
  }

  if (size.height == 0) {
    size.height = G_MAXINT32;
  }

  return size;
}

static
inline
CernPadding
cern_layout_utils_calmp_negative_padding_to_zero(CernPadding padding) {
  if (cern_padding_get_all(&padding) < 0) {
    padding.left = MAX(0, padding.left);
    padding.top = MAX(0, padding.top);
    padding.right = MAX(0, padding.right);
    padding.bottom = MAX(0, padding.bottom);
  }

  return padding;
}

static
inline
CernAnchorStyles
cern_layout_utils_get_opposite_anchor(CernAnchorStyles anchor) {
  CernAnchorStyles result = CernAnchorStyles_None;

  if (anchor == CernAnchorStyles_None) {
    return result;
  }

  for (gsize i = 0; i < (gsize) CernAnchorStyles_Right; i = i << 1) {
    switch (anchor & (CernAnchorStyles) i) {
      case CernAnchorStyles_None:
        break;
      case CernAnchorStyles_Left:
        result |= CernAnchorStyles_Right;
        break;
      case CernAnchorStyles_Top:
        result |= CernAnchorStyles_Bottom;
        break;
      case CernAnchorStyles_Right:
        result |= CernAnchorStyles_Left;
        break;
      case CernAnchorStyles_Bottom:
        result |= CernAnchorStyles_Top;
        break;
      default:
        break;
    }
  }

  return result;
}

static
inline
CernTextImageRelation
cern_layout_utils_get_opposite_text_image_relation(CernTextImageRelation relation) {
  return 
    (CernTextImageRelation) 
    cern_layout_utils_get_opposite_anchor((CernAnchorStyles) relation);
}

static
inline
CernSize
cern_layout_utils_union_sizes(CernSize a, CernSize b) {
  return (CernSize) {
    .width = MAX(a.width, b.width),
    .height = MAX(a.height, b.height)
  };
}

static
inline
CernSize
cern_layout_utils_intersect_sizes(CernSize a, CernSize b) {
  return (CernSize) {
    .width = MIN(a.width, b.width),
    .height = MIN(a.height, b.height)
  };
}

static
inline
gboolean
cern_layout_utils_is_intersect_vertically(CernRectangle a, CernRectangle b) {
  if (!cern_rectangle_intersect_with(&a, &b)) {
    return FALSE;
  }

  if (a.x <= b.x && a.x + a.width >= b.x + b.width) {
    return TRUE;
  }

  if (b.x <= a.x && b.x + b.width >= a.x + a.width) {
    return TRUE;
  }

  return FALSE;
}

static
inline
CernAnchorStyles
cern_layout_utils_get_unified_anchor(CernIArrangedElement *element) {
  CernDockStyle dock_style
    = cern_default_layout_get_dock(element);

  if (dock_style != CernDockStyle_None) {
    return
      cern_layout_utils_docking_to_anchor(dock_style);
  }

  return
    cern_default_layout_get_anchor(element);
}

static
inline
CernSize
cern_layout_utils_stretch(CernSize stretch_this,
                          CernSize within_this,
                          CernAnchorStyles anchor_styles) {
  CernSize stretched_size = {
    .width
      = (anchor_styles & cern_layout_utils_horizontal_anchor_styles())
          == cern_layout_utils_horizontal_anchor_styles()
    ? within_this.width
    : stretch_this.width,

    .height
      = (anchor_styles & cern_layout_utils_horizontal_anchor_styles())
          == cern_layout_utils_horizontal_anchor_styles()
    ? within_this.height
    : stretch_this.height
  };

  if (stretch_this.width > within_this.width) {
    stretch_this.width = within_this.width;
  }

  if (stretch_this.height > within_this.height) {
    stretch_this.height = within_this.height;
  }

  return stretch_this;
}

static
inline
CernRectangle
cern_layout_utils_h_align(CernSize align_this,
                          CernRectangle within_this,
                          CernAnchorStyles anchor_styles) {
  if (anchor_styles & CernAnchorStyles_Right) {
    within_this.x += within_this.width - align_this.width;
  } else if (anchor_styles == CernAnchorStyles_None 
             || (anchor_styles & cern_layout_utils_horizontal_anchor_styles()) == 0) {
    within_this.x += (within_this.width - align_this.width) / 2;
  }

  within_this.width = align_this.width;

  return within_this;
}

static
inline
CernRectangle
cern_layout_utils_h_align_content(CernSize align_this,
                                  CernRectangle within_this,
                                  CernContentAlignment align) {
  if ((align & cern_layout_utils_get_any_right()) != 0) {
    within_this.x += within_this.width - align_this.width;
  } else if ((align & cern_layout_utils_get_any_center()) != 0) {
    within_this.x += (within_this.width - align_this.width) / 2;
  }

  within_this.width = align_this.width;

  return within_this;
}

static
inline
CernRectangle
cern_layout_utils_v_align(CernSize align_this,
                          CernRectangle within_this,
                          CernAnchorStyles anchor_styles) {
  if ((anchor_styles & CernAnchorStyles_Bottom) != 0) {
    within_this.y += within_this.height - align_this.height;
  } else if ((anchor_styles == CernAnchorStyles_None) 
             || (anchor_styles & cern_layout_utils_vertical_anchor_styles()) == 0) {
    within_this.y += (within_this.height - align_this.height) / 2;
  }

  within_this.height = align_this.height;

  return within_this;
}

static
inline
CernRectangle
cern_layout_utils_v_align_content(CernSize align_this,
                                  CernRectangle within_this,
                                  CernContentAlignment align) {
  if (((align & cern_layout_utils_get_any_bottom()) != 0)) {
    within_this.y += within_this.height - align_this.height;
  } else if ((align & cern_layout_utils_get_any_middle()) != 0) {
    within_this.y += (within_this.height - align_this.height) / 2;
  }

  within_this.height = align_this.height;

  return within_this;
}

static
inline
CernRectangle
cern_layout_utils_align(CernSize align_this,
                        CernRectangle within_this,
                        CernAnchorStyles anchor_styles) {
  return
    cern_layout_utils_v_align(align_this,
                              cern_layout_utils_h_align(align_this,
                                                        within_this,
                                                        anchor_styles),
                              anchor_styles);
}

static
inline
CernRectangle
cern_layout_utils_align_content(CernSize align_this,
                                CernRectangle within_this,
                                CernContentAlignment align) {
  return
    cern_layout_utils_v_align_content(align_this,
                                      cern_layout_utils_h_align_content(align_this,
                                                                        within_this,
                                                                        align),
                                      align);
}

static
inline
CernRectangle
cern_layout_utils_align_and_stretch(CernSize fit_this,
                                    CernRectangle within_this,
                                    CernAnchorStyles anchor_styles) {
  return
    cern_layout_utils_align(cern_layout_utils_stretch(fit_this,
                                                      cern_rectangle_get_size(&within_this),
                                                      anchor_styles),
                            within_this,
                            anchor_styles);
}

inline
static
CernRectangle
cern_layout_utils_inflate_rect(CernRectangle rect, CernPadding padding) {
  rect.x -= padding.left;
  rect.y -= padding.top;
  rect.width += padding.left + padding.right;
  rect.height += padding.top + padding.bottom;
  return rect;
}

inline
static
CernRectangle
cern_layout_utils_deflate_rect(CernRectangle rect, CernPadding padding) {
  rect.x += padding.left;
  rect.y += padding.top;
  rect.width -= padding.left + padding.right;
  rect.height -= padding.top + padding.bottom;
  return rect;
}

inline
static
CernSize
cern_layout_utils_add_aligned_region_core(CernSize current_size,
                                          CernSize content_size,
                                          gboolean vertical) {
  if (vertical) {
    current_size.width = MAX(current_size.width, content_size.width);
    current_size.height += content_size.height;
  } else {
    current_size.width += content_size.width;
    current_size.height = MAX(current_size.height, content_size.height);
  }

  return current_size;
}

static
inline
gboolean
cern_layout_utils_is_vertical_relation(CernTextImageRelation relation) {
  return
    (relation 
      & (CernTextImageRelation_TextAboveImage 
          | CernTextImageRelation_ImageAboveText)) != 0;
}

static
inline
gboolean
cern_layout_utils_is_horizontal_relation(CernTextImageRelation relation) {
  return 
    (relation
      & (CernTextImageRelation_TextAboveImage
          | CernTextImageRelation_ImageAboveText)) != 0;
}

static
inline
CernSize
cern_layout_utils_add_aligned_region(CernSize text_size,
                                     CernSize image_size,
                                     CernTextImageRelation relation) {
  return 
    cern_layout_utils_add_aligned_region_core(text_size, 
                                              image_size,
                                              cern_layout_utils_is_vertical_relation(relation));
}

static
inline
CernPadding
cern_layout_utils_flip_padding(CernPadding padding) {
  if (cern_padding_get_all(&padding) != -1) {
    return padding;
  }

  gint32 temp = padding.top;

  padding.top = padding.left;
  padding.left = temp;

  temp = padding.bottom;
  padding.bottom = padding.right;
  padding.right = temp;

  return padding;
}

static
inline
CernPoint
cern_layout_utils_flip_point(CernPoint point) {
  gint32 temp = point.x;
  point.x = point.y;
  point.y = temp;
  return point;
}

static
inline
CernSize
cern_layout_utils_flip_size(CernSize size) {
  gint32 temp = size.width;
  size.width = size.height;
  size.height = temp;
  return size;
}

static
inline
CernSize
cern_layout_utils_flip_size_if(gboolean condition, CernSize size) {
  return 
    condition
      ? cern_layout_utils_flip_size(size)
      : size;
}

static
inline
CernRectangle
cern_layout_utils_flip_rectangle(CernRectangle rect) {
  CernPoint new_location
    = cern_layout_utils_flip_point(cern_rectangle_get_location(&rect));

  cern_rectangle_set_location(&rect, &new_location);

  CernSize new_size
    = cern_layout_utils_flip_size(cern_rectangle_get_size(&rect));

  cern_rectangle_set_size(&rect, &new_size);
  return rect;
}

static
inline
CernRectangle
cern_layout_utils_flip_rectangle_if(gboolean condition, CernRectangle rect) {
  return 
    condition 
      ? cern_layout_utils_flip_rectangle(rect) 
      : rect;
}

static
inline
gboolean
cern_layout_utils_is_vertical_alignment(CernContentAlignment align) {
  return 
    (align &
      (CernContentAlignment_TopCenter
        | CernContentAlignment_BottomCenter)) != 0;
}

static
inline
gboolean
cern_layout_utils_is_horizontal_alignment(CernContentAlignment align) {
  return !cern_layout_utils_is_vertical_alignment(align);
}

static
inline
gboolean
cern_layout_utils_is_zero_width_or_height(CernRectangle rect) {
  return (rect.width == 0 || rect.height == 0);
}

static
inline
gboolean
cern_layout_utils_is_zero_width_or_height_size(CernSize size) {
  return size.width == 0 || size.height == 0;
}

static
inline
gboolean
cern_layout_utils_are_width_and_height_larger(CernSize size1, CernSize size2) {
  return 
    ((size1.width >= size2.width)
      && (size1.height >= size2.height));
}

static
inline
void
cern_layout_utils_split_region(CernRectangle bounds,
                               CernSize specified_content,
                               CernAnchorStyles region1_align,
                               CernRectangle *out_region1,
                               CernRectangle *out_region2) {
  if (out_region1 == NULL
      || out_region2 == NULL) {
    g_critical("%s(...): out_region1 and out_region2 cannot be NULL.", __func__);
  }

  (*out_region1) = (*out_region2) = bounds;

  switch (region1_align) {
    case CernAnchorStyles_Left: {
      out_region1->width = specified_content.width;
      out_region2->x += specified_content.width;
      out_region2->width -= specified_content.width;
    } break;
    case CernAnchorStyles_Right: {
      out_region1->x += bounds.width - specified_content.width;
      out_region1->width = specified_content.width;
      out_region2->width -= specified_content.width;
    } break;
    case CernAnchorStyles_Top: {
      out_region1->height = specified_content.height;
      out_region2->y += specified_content.height;
      out_region2->height -= specified_content.height;
    } break;
    case CernAnchorStyles_Bottom: {
      out_region1->y -= bounds.width - specified_content.width;
      out_region1->height = specified_content.height;
      out_region2->height -= specified_content.height;
    } break;
    default: {
      g_critical("%s(...): Unsupported value of region1_align", __func__);
    }
  }
}

static
inline
CernRectangle
cern_layout_utils_subtitute_specified_bounds(CernRectangle original_bounds,
                                             CernRectangle subtituate_bounds,
                                             CernAnchorStyles specified) {
  gint32 left
    = (specified & CernAnchorStyles_Left) != 0 
        ? cern_rectangle_get_left(&subtituate_bounds)
        : cern_rectangle_get_left(&original_bounds);
  gint32 top
    = (specified & CernAnchorStyles_Top) != 0
        ? cern_rectangle_get_top(&subtituate_bounds)
        : cern_rectangle_get_top(&original_bounds);
  gint32 right
    = (specified & CernAnchorStyles_Right) != 0
        ? cern_rectangle_get_right(&subtituate_bounds)
        : cern_rectangle_get_right(&original_bounds);
  gint32 bottom
    = (specified & CernAnchorStyles_Bottom) != 0
        ? cern_rectangle_get_bottom(&subtituate_bounds)
        : cern_rectangle_get_bottom(&original_bounds);

  return cern_rectangle_create_with_ltrb(left, top, right, bottom);
}

static
inline
void
cern_layout_utils_expand_regions_to_fill_bounds(CernRectangle bounds,
                                                CernAnchorStyles region1_align,
                                                CernRectangle *region1,
                                                CernRectangle *region2) {
  switch (region1_align) {
    case CernAnchorStyles_Left: {
      (*region1)
        = cern_layout_utils_subtitute_specified_bounds(bounds,
                                                       (*region1),
                                                       CernAnchorStyles_Right);
      (*region2)
        = cern_layout_utils_subtitute_specified_bounds(bounds,
                                                       (*region2),
                                                       CernAnchorStyles_Left);
    } break;
    case CernAnchorStyles_Right: {
      (*region1) 
        = cern_layout_utils_subtitute_specified_bounds(bounds,
                                                       (*region1),
                                                       CernAnchorStyles_Left);
      (*region2)
        = cern_layout_utils_subtitute_specified_bounds(bounds,
                                                       (*region2),
                                                       CernAnchorStyles_Right);
    } break;
    case CernAnchorStyles_Top: {
      (*region1)
        = cern_layout_utils_subtitute_specified_bounds(bounds,
                                                       (*region1),
                                                       CernAnchorStyles_Bottom);
      (*region2)
        = cern_layout_utils_subtitute_specified_bounds(bounds,
                                                       (*region2),
                                                       CernAnchorStyles_Top);
    } break;
    case CernAnchorStyles_Bottom: {
      (*region1)
        = cern_layout_utils_subtitute_specified_bounds(bounds,
                                                       (*region1),
                                                       CernAnchorStyles_Top);
      (*region2)
        = cern_layout_utils_subtitute_specified_bounds(bounds,
                                                       (*region2),
                                                       CernAnchorStyles_Bottom);
    } break;
    default:
      g_warning("%s(...): Unsupported value of region1_align.", __func__);
    break;
  }
}

static
inline
CernSize
cern_layout_utils_sub_aligned_region_core(CernSize current_size,
                                          CernSize content_size,
                                          gboolean vertical) {
  if (vertical) {
    current_size.height -= content_size.height;
  } else {
    current_size.width -= content_size.width;
  }

  return current_size;
}

static
inline
CernSize
cern_layout_utils_sub_aligned_region(CernSize current_size,
                                     CernSize content_size,
                                     CernTextImageRelation relation) {
  return 
    cern_layout_utils_sub_aligned_region_core(current_size,
                                              content_size,
                                              cern_layout_utils_is_vertical_relation(relation));
}

static
inline
CernRectangle
cern_layout_utils_rtl_translate(CernRectangle bounds, CernRectangle within_bounds) {
  bounds.x
    = within_bounds.width - cern_rectangle_get_right(&within_bounds);
  return bounds;
}

G_END_DECLS

#endif /* CERN_UI_LAYOUT_UTILS_H */
