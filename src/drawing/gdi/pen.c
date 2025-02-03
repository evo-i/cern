#include "cern/drawing/pen.h"
#include "cern/core/icloneable.h"
#include "cern/drawing/graphics_unit.h"
#include "cern/drawing/internal/native_gdi_object.h"

#include <Windows.h>
#include <gdiplus.h>

struct _CernPen {
  GObject parent_instance;

  gpointer pen;
  CernColor *color;
};

static
gpointer
cern_pen_get_instance_private(CernPen *self);

static
CernPen *
cern_pen_new_from_native_handle(gpointer handle) {
  CernPen *self;
  self = g_object_new(CERN_TYPE_PEN, NULL);
  self->pen = handle;
  return self;
}

CernICLoneable  *
cern_pen_clone(CernICLoneable  *cloneable) {
  CernPen *self = CERN_PEN(cloneable);
  GpPen *pen;
  GpStatus status;

  status = GdipClonePen(self->pen, &pen);

  if (status != Ok) {
    return NULL;
  }

  return CERN_ICLONEABLE(cern_pen_new_from_native_handle(pen));
}

static
void
cern_pen_cloneable_iface(CernICLoneableInterface *iface) {
  iface->clone = cern_pen_clone;
}

static
gpointer
cern_pen_get_native_handle(CernPen *self) {
  return self->pen;
}

static
void
cern_pen_set_native_handle(CernPen *self, gpointer handle) {
  self->pen = handle;
}

static
void
cern_pen_native_gdi_object_iface(CernNativeGdiObjectInterface *iface) {
  iface->get_native_handle
    = (gpointer(*)(CernNativeGdiObject *)) cern_pen_get_native_handle;
  iface->set_native_handle
    = (void(*)(CernNativeGdiObject *, gpointer)) cern_pen_get_native_handle;
}

G_DEFINE_FINAL_TYPE_WITH_CODE(CernPen, cern_pen, G_TYPE_OBJECT,
  G_IMPLEMENT_INTERFACE(CERN_TYPE_ICLONEABLE, cern_pen_cloneable_iface)
  G_IMPLEMENT_INTERFACE(CERN_TYPE_NATIVE_GDI_OBJECT, cern_pen_native_gdi_object_iface)
)

static
void
cern_pen_init(CernPen *self) {
  self->pen = NULL;
}

static
void
cern_pen_class_init(CernPenClass *klass) { }

static
void
cern_pen_finalize(GObject *object) {
  CernPen *self = CERN_PEN(object);

  if (self->pen) {
    GdipDeletePen(self->pen);
    self->pen = NULL;
  }
}

CernPen *
cern_pen_new (CernColor *color) {
  return cern_pen_new_width(color, 1.0f);
}

CernPen *
cern_pen_new_width(CernColor *color, gfloat width) {
  CernPen *self;
  GpPen *pen;
  ARGB argb;
  GpStatus status;

  if (color == NULL) {
    g_critical("cern_pen_new_width(): color cannot be NULL");
    return NULL;
  }

  argb = (ARGB) cern_color_to_argb(color);
  status = GdipCreatePen1(argb, width, (Unit) CernGraphicsUnit_World, &pen);

  if (status != Ok) {
    g_critical("cern_pen_new_width(): GdipCreatePen1() failed");
    return NULL;
  }

  self = cern_pen_new_from_native_handle(pen);
  self->color = cern_color_from_argb(argb);

  return self;
}

CernPen *
cern_pen_new_brush(CernBrush *brush) {
  return cern_pen_new_brush_and_width(brush, 1.0f);
}

CernPen *
cern_pen_new_brush_and_width(CernBrush *brush, gfloat width) {
  CernPen *self;
  GpPen *pen;
  GpBrush *native_brush;
  GpStatus status;
  CernNativeGdiObject *brush_native;

  if (brush == NULL) {
    g_critical("cern_pen_new_width(): brush cannot be NULL");
    return NULL;
  }

  brush_native = CERN_NATIVE_GDI_OBJECT(brush);

  native_brush = cern_native_gdi_object_get_native_handle(brush_native);
  status = GdipCreatePen2(native_brush, width, (Unit) CernGraphicsUnit_World, &pen);

  if (status != Ok) {
    g_critical("cern_pen_new_brush_and_width(): GdipCreatePen1() failed");
    return NULL;
  }

  self = cern_pen_new_from_native_handle(pen);

  return self;
}

/* Getters and setters */
gfloat
cern_pen_get_width(CernPen *self) {
  GpPen *pen;
  GpStatus status;
  REAL width;

  status = GdipGetPenWidth(self->pen, &width);

  if (status != Ok) {
    g_critical("cern_pen_get_width(): GdipGetPenWidth() failed");
    return 0.0f;
  }

  return width;
}

void
cern_pen_set_width(CernPen *self, gfloat width) {
  GpStatus status;

  status = GdipSetPenWidth(self->pen, width);

  if (status != Ok) {
    g_critical("cern_pen_set_width(): GdipSetPenWidth() failed");
  }
}

void
cern_pen_set_line_cap(CernPen *self, CernLineCap start_cap,
                      CernLineCap end_cap, CernLineCap dash_cap) {
  GpStatus status;

  status
    = GdipSetPenLineCap197819(self->pen,
                              (GpLineCap) start_cap,
                              (GpLineCap) end_cap,
                              (GpLineCap) dash_cap);

  if (status != Ok) {
    g_critical("cern_pen_set_line_cap(): GdipSetPenLineCap197819() failed");
  }
}

CernLineCap
cern_pen_get_start_cap(CernPen *self) {
  GpPen *pen;
  GpStatus status;
  GpLineCap line_cap;

  status = GdipGetPenStartCap(self->pen, &line_cap);

  if (status != Ok) {
    g_critical("cern_pen_get_start_cap(): GdipGetPenStartCap() failed");
    return CernLineCap_Flat;
  }

  return (CernLineCap) line_cap;
}

void
cern_pen_set_start_cap(CernPen *self, CernLineCap start_cap) {
  GpStatus status;

  status = GdipSetPenStartCap(self->pen, (GpLineCap) start_cap);

  if (status != Ok) {
    g_critical("cern_pen_set_start_cap(): GdipSetPenStartCap() failed");
  }
}

CernLineCap
cern_pen_get_end_cap(CernPen *self) {
  GpPen *pen;
  GpStatus status;
  GpLineCap line_cap;

  status = GdipGetPenEndCap(self->pen, &line_cap);

  if (status != Ok) {
    g_critical("cern_pen_get_end_cap(): GdipGetPenEndCap() failed");
    return CernLineCap_Flat;
  }

  return (CernLineCap) line_cap;
}

void
cern_pen_set_end_cap(CernPen *self, CernLineCap end_cap) {
  GpStatus status;

  status = GdipSetPenEndCap(self->pen, (GpLineCap) end_cap);

  if (status != Ok) {
    g_critical("cern_pen_set_end_cap(): GdipSetPenEndCap() failed");
  }
}

CernDashCap
cern_pen_get_dash_cap(CernPen *self) {
  GpPen *pen;
  GpStatus status;
  GpDashCap dash_cap;

  status = GdipGetPenDashCap197819(self->pen, &dash_cap);

  if (status != Ok) {
    g_critical("cern_pen_get_dash_cap(): GdipGetPenDashCap197819() failed");
    return CernDashCap_Flat;
  }

  return (CernDashCap) dash_cap;
}

void
cern_pen_set_dash_cap(CernPen *self, CernDashCap dash_cap) {
  GpStatus status;

  status = GdipSetPenDashCap197819(self->pen, (GpDashCap) dash_cap);

  if (status != Ok) {
    g_critical("cern_pen_set_dash_cap(): GdipSetPenDashCap197819() failed");
  }
}

CernLineJoin
cern_pen_get_line_join(CernPen *self) {
  GpPen *pen;
  GpStatus status;
  GpLineJoin line_join;

  status = GdipGetPenLineJoin(self->pen, &line_join);

  if (status != Ok) {
    g_critical("cern_pen_get_line_join(): GdipGetPenLineJoin() failed");
    return CernLineJoin_Miter;
  }
}

void
cern_pen_set_line_join(CernPen *self, CernLineJoin line_join) {
  GpStatus status;

  status = GdipSetPenLineJoin(self->pen, (GpLineJoin) line_join);

  if (status != Ok) {
    g_critical("cern_pen_set_line_join(): GdipSetPenLineJoin() failed");
  }
}

CernCustomLineCap *
cern_pen_get_custom_start_cap(CernPen *self) {
  GpPen *pen;
  GpStatus status = InvalidParameter;
  GpCustomLineCap *custom_line_cap;

  //status = GdipGetPenCustomStartCap(self->pen, &custom_line_cap);

  if (status != Ok) {
    g_critical("cern_pen_get_custom_start_cap(): GdipGetPenCustomStartCap() failed");
    return NULL;
  }

  g_critical("cern_pen_get_custom_start_cap(): not implemented");

  return NULL;
}

void
cern_pen_set_custom_start_cap(CernPen *self, CernCustomLineCap *custom_line_cap) {
  g_critical("cern_pen_set_custom_start_cap(): not implemented");
}

CernCustomLineCap *
cern_pen_get_custom_end_cap(CernPen *self) {
  g_critical("cern_pen_get_custom_end_cap(): not implemented");
  return NULL;
}

void
cern_pen_set_custom_end_cap(CernPen *self, CernCustomLineCap *custom_line_cap) {
  g_critical("cern_pen_set_custom_end_cap(): not implemented");
}

gfloat
cern_pen_get_miter_limit(CernPen *self) {
  GpPen *pen;
  GpStatus status;
  REAL miter_limit;

  status = GdipGetPenMiterLimit(self->pen, &miter_limit);

  if (status != Ok) {
    g_critical("cern_pen_get_miter_limit(): GdipGetPenMiterLimit() failed");
    return 0.0f;
  }

  return miter_limit;
}

void
cern_pen_set_miter_limit(CernPen *self, gfloat miter_limit) {
  GpStatus status;

  status = GdipSetPenMiterLimit(self->pen, miter_limit);

  if (status != Ok) {
    g_critical("cern_pen_set_miter_limit(): GdipSetPenMiterLimit() failed");
  }
}

CernPenAlignment
cern_pen_get_alignment(CernPen *self) {
  GpStatus status;
  GpPenAlignment alignment;

  status = GdipGetPenMode(self->pen, &alignment);

  if (status != Ok) {
    g_critical("cern_pen_get_alignment(): GdipGetPenMode() failed");
    return CernPenAlignment_Center;
  }
}

void
cern_pen_set_alignment(CernPen *self, CernPenAlignment alignment) {
  GpStatus status;

  status = GdipSetPenMode(self->pen, (GpPenAlignment) alignment);

  if (status != Ok) {
    g_critical("cern_pen_set_alignment(): GdipSetPenMode() failed");
  }
}

CernMatrix *
cern_pen_get_transform(CernPen *self) {
  g_critical("cern_pen_get_transform(): not implemented");
  return NULL;
}

void
cern_pen_set_transform(CernPen *self, CernMatrix *transform) {
  g_critical("cern_pen_set_transform(): not implemented");
}

void
cern_pen_reset_transform(CernPen *self) {
  GpStatus status;

  status = GdipResetPenTransform(self->pen);

  if (status != Ok) {
    g_critical("cern_pen_reset_transform(): GdipResetPenTransform() failed");
  }
}

void
cern_pen_multiply_transform(CernPen *self, CernMatrix *transform) {
  g_critical("cern_pen_multiply_transform(): not implemented");
}

void
cern_pen_muliply_transform_with_order(CernPen *self, CernMatrix *transform,
                                      CernMatrixOrder order) {
  g_critical("cern_pen_muliply_transform_with_order(): not implemented");
}

void
cern_pen_translate_transform(CernPen *self, gfloat offset_x, gfloat offset_y) {
  g_critical("cern_pen_translate_transform(): not implemented");
}

void
cern_pen_translate_transform_with_order(CernPen *self,
                                        gfloat offset_x, gfloat offset_y,
                                        CernMatrixOrder order) {
  g_critical("cern_pen_translate_transform_with_order(): not implemented");
}

void
cern_pen_scale_transform(CernPen *self, gfloat scale_x, gfloat scale_y) {
  cern_pen_scale_transform_with_order(self, scale_x, scale_y,
                                      CernMatrixOrder_Prepend);
}

void
cern_pen_scale_transform_with_order(CernPen *self,
                                    gfloat scale_x, gfloat scale_y,
                                    CernMatrixOrder order) {
  GpStatus status;

  status
    = GdipScalePenTransform(self->pen, scale_x, scale_y, (GpMatrixOrder) order);

  if (status != Ok) {
    g_critical("cern_pen_scale_transform_with_order(): GdipScalePenTransform() failed");
  }
}

void
cern_pen_rotate_transform(CernPen *self, gfloat angle) {
  cern_pen_rotate_transform_with_order(self, angle, CernMatrixOrder_Prepend);
}

void
cern_pen_rotate_transform_with_order(CernPen *self, gfloat angle,
                                     CernMatrixOrder order) {
  GpStatus status;

  status = GdipRotatePenTransform(self->pen, angle, (GpMatrixOrder) order);

  if (status != Ok) {
    g_critical("cern_pen_rotate_transform_with_order(): GdipRotatePenTransform() failed");
  }
}

CernPenType
cern_pen_get_pen_type(CernPen *self) {
  GpStatus status;
  GpPenType pen_type;

  status = GdipGetPenFillType(self->pen, &pen_type);

  if (status != Ok) {
    g_critical("cern_pen_get_pen_type(): GdipGetPenType() failed");
    return CernPenType_SolidColor;
  }

  return pen_type;
}

CernColor *
cern_pen_get_color(CernPen *self) {
  ARGB argb;
  GpStatus status;

  if (cern_color_is_empty(self->color)) {
    status = GdipGetPenColor(self->pen, &argb);
    if (status != Ok) {
      g_critical("cern_pen_get_color(): GdipGetPenColor() failed");
      return cern_color_empty();
    }

    self->color = cern_color_from_argb(argb);
  }

  return self->color;
}

void
cern_pen_set_color(CernPen *self, CernColor *color) {
  GpStatus status;
  ARGB argb;

  if (cern_color_is_empty(color)) {
    g_critical("cern_pen_set_color(): color cannot be empty");
    return;
  }

  argb = (ARGB) cern_color_to_argb(color);
  status = GdipSetPenColor(self->pen, argb);

  if (status != Ok) {
    g_critical("cern_pen_set_color(): GdipSetPenColor() failed");
  }
}

CernBrush *
cern_pen_get_brush(CernPen *self) {
  g_critical("cern_pen_get_brush(): not implemented");
  return NULL;
}

void
cern_pen_set_brush(CernPen *self, CernBrush *brush) {
  g_critical("cern_pen_set_brush(): not implemented");
}

CernDashStyle
cern_pen_get_dash_style(CernPen *self) {
  GpStatus status;
  GpDashStyle dash_style;

  status = GdipGetPenDashStyle(self->pen, &dash_style);

  if (status != Ok) {
    g_critical("cern_pen_get_dash_style(): GdipGetPenDashStyle() failed");
    return CernDashStyle_Solid;
  }

  return (CernDashStyle) dash_style;
}

void
cern_pen_set_dash_style(CernPen *self, CernDashStyle dash_style) {
  GpStatus status;
  GpDashStyle gdip_dash_style;

  gdip_dash_style = (GpDashStyle) dash_style;
  status = GdipSetPenDashStyle(self->pen, gdip_dash_style);

  if (status != Ok) {
    g_critical("cern_pen_set_dash_style(): GdipSetPenDashStyle() failed");
  }
}

gfloat
cern_pen_get_dash_offset(CernPen *self) {
  REAL offset;
  GpStatus status;

  status = GdipGetPenDashOffset(self->pen, &offset);
  if (status != Ok) {
    g_critical("cern_pen_get_dash_offset(): GdipGetPenDashOffset() failed");
    return 0.0f;
  }

  return offset;
}

void
cern_pen_set_dash_offset(CernPen *self, gfloat dash_offset) {
  GpStatus status;
  REAL offset;

  offset = (REAL) dash_offset;
  status = GdipSetPenDashOffset(self->pen, offset);

  if (status != Ok) {
    g_critical("cern_pen_set_dash_offset(): GdipSetPenDashOffset() failed");
  }
}

void
cern_pen_get_dash_pattern(CernPen *self, gfloat *pattern, gint32 *count) {
  GpStatus status;

  status = GdipGetPenDashCount(self->pen, count);

  if (status != Ok) {
    g_critical("cern_pen_get_dash_pattern(): GdipGetPenDashCount() failed");
    return;
  }

  pattern = g_alloca0(sizeof(gfloat) * (*count));
  status = GdipGetPenDashArray(self->pen, pattern, (*count));

  if (status != Ok) {
    g_critical("cern_pen_get_dash_pattern(): GdipGetPenDashArray() failed");
    g_free(pattern);
  }
}

void
cern_pen_set_dash_pattern(CernPen *self, gfloat *pattern, gint32 count) {
  GpStatus status;
  status = GdipSetPenDashArray(self->pen, pattern, count);
  if (status != Ok) {
    g_critical("cern_pen_set_dash_pattern(): GdipSetPenDashArray() failed");
  }
}

void
cern_pen_get_compound_array(CernPen *self, gfloat *array, gint32 *count) {
  GpStatus status;

  status = GdipGetPenCompoundCount(self->pen, count);
  if (status != Ok) {
    g_critical("cern_pen_get_compound_array(): GdipGetPenCompoundCount() failed");
    return;
  }

  array = g_alloca0(sizeof(gfloat) * (*count));
  status = GdipGetPenCompoundArray(self->pen, array, *count);

  if (status != Ok) {
    g_critical("cern_pen_get_compound_array(): GdipGetPenCompoundArray() failed");
    g_free(array);
  }
}

void
cern_pen_set_compound_array(CernPen *self, gfloat const *array, gint32 count) {
  GpStatus status;
  status = GdipSetPenCompoundArray(self->pen, array, count);
  if (status != Ok) {
    g_critical("cern_pen_set_compound_array(): GdipSetPenCompoundArray() failed");
  }
}
