#ifndef CERN_GDI_OBJECT_TYPE_H
#define CERN_GDI_OBJECT_TYPE_H 1

typedef enum _CernGdiObjectType {
  CernGdiObjectType_Pen                 = 1,
  CernGdiObjectType_Brush               = 2,
  CernGdiObjectType_DisplayDC           = 3,
  CernGdiObjectType_MetafileDC          = 4,
  CernGdiObjectType_Palette             = 5,
  CernGdiObjectType_Font                = 6,
  CernGdiObjectType_Bitmap              = 7,
  CernGdiObjectType_Region              = 8,
  CernGdiObjectType_Metafile            = 9,
  CernGdiObjectType_MemoryDC            = 10,
  CernGdiObjectType_ExtendedPen         = 11,
  CernGdiObjectType_EnhancedMetafileDC  = 12,
  CernGdiObjectType_EnhMetafile         = 13,
  CernGdiObjectType_ColorSpace          = 14

  /*
  OBJ_PEN       = 1,
  OBJ_BRUSH     = 2,
  OBJ_FONT      = 6,
  OBJ_EXTPEN    = 11,

  OBJ_DC        = 3,
  OBJ_METADC    = 4,
  OBJ_MEMDC     = 10,
  OBJ_ENHMETADC = 12,
  */
} CernGdiObjectType;

#endif /* CERN_GDI_OBJECT_TYPE_H */
