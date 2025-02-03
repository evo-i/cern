#ifndef CERN_DRAWING_2D_PEN_TYPE_H
#define CERN_DRAWING_2D_PEN_TYPE_H 1

#include "brush_type.h"

/**
 * @brief Specifies the type of fill a Pen uses to fill lines.
 */
typedef enum _CernPenType {
  /**
   * @brief Specifies a solid fill.
   */
  CernPenType_SolidColor       = CernBrushType_SolidColor,
  /**
   * @brief Specifies a hatch fill.
   */
  CernPenType_HatchFill        = CernBrushType_HatchFill,
  /**
   * @brief Specifies a bitmap texture fill.
   */
  CernPenType_TextureFill      = CernBrushType_TextureFill,
  /**
   * @brief Specifies a path gradient fill.
   */
  CernPenType_PathGradient     = CernBrushType_PathGradient,
  /**
   * @brief Specifies a linear gradient fill.
   */
  CernPenType_LinearGradient   = CernBrushType_LinearGradient,
} CernPenType;

#endif /* CERN_DRAWING_2D_PEN_TYPE_H */
