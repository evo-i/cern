#ifndef CERN_DRAWING_TEXTURE_BRUSH_H
#define CERN_DRAWING_TEXTURE_BRUSH_H 1

#include <glib-object.h>

#include "brush.h"
#include "image.h"
#include "2d/wrap_mode.h"
#include "rectangle_f.h"
#include "rectangle.h"

G_BEGIN_DECLS

#define CERN_TYPE_TEXTURE_BRUSH (cern_texture_brush_get_type())

G_DECLARE_FINAL_TYPE(CernTextureBrush, cern_texture_brush, CERN, TEXTURE_BRUSH, CernBrush)

CernBrush *
cern_texture_brush_new(CernImage *bitmap);

CernBrush *
cern_texture_brush_new_with_wrap_mode(CernImage *bitmap, CernWrapMode wrap_mode);

CernBrush *
cern_texture_brush_new_with_wrap_mode_and_rectangle_f(CernImage *bitmap,
  CernWrapMode wrap_mode,
  CernRectangleF *rectangle);

CernBrush *
cern_texture_brush_new_with_wrap_mode_and_rectangle(CernImage *bitmap,
  CernWrapMode wrap_mode,
  CernRectangle *rectangle);

CernBrush *
cern_texture_brush_new_with_rectangle(CernImage *bitmap, CernRectangle *rectangle);


G_END_DECLS

#endif /* CERN_DRAWING_TEXTURE_BRUSH_H */
