#ifndef CERN_UI_MOUSE_BUTTONS_H
#define CERN_UI_MOUSE_BUTTONS_H 1

typedef enum _CernMouseButtons {
  CernMouseButtons_Left = 0x00100000,
  CernMouseButtons_None = 0x00000000,
  CernMouseButtons_Right = 0x00200000,
  CernMouseButtons_Middle = 0x00400000,
  CernMouseButtons_XButton1 = 0x00800000,
  CernMouseButtons_XButton2 = 0x01000000,
} CernMouseButtons;

#endif /* CERN_UI_MOUSE_BUTTONS_H */
