#ifndef CERN_UI_CONTENT_ALIGNMENT_H_DEFINED
#define CERN_UI_CONTENT_ALIGNMENT_H_DEFINED 1

typedef enum _CernContentAlignment {
  CernContentAlignment_TopLeft = 0x001,
  CernContentAlignment_TopCenter = 0x002,
  CernContentAlignment_TopRight = 0x004,
  CernContentAlignment_MiddleLeft = 0x010,
  CernContentAlignment_MiddleCenter = 0x020,
  CernContentAlignment_MiddleRight = 0x040,
  CernContentAlignment_BottomLeft = 0x100,
  CernContentAlignment_BottomCenter = 0x200,
  CernContentAlignment_BottomRight = 0x400,
} CernContentAlignment;

#endif /* CERN_UI_CONTENT_ALIGNMENT_H_DEFINED */
