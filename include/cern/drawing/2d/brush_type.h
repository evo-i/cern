#ifndef CERN_BRUSH_TYPE_H
#define CERN_BRUSH_TYPE_H 1

typedef enum _CernBrushType {
  CernBrushType_SolidColor     = 0,
  CernBrushType_HatchFill      = 1,
  CernBrushType_TextureFill    = 2,
  CernBrushType_PathGradient   = 3,
  CernBrushType_LinearGradient = 4
} CernBrushType;

#endif /* CERN_BRUSH_TYPE_H */
