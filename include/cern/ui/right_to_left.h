#ifndef CERN_UI_RIGHT_TO_LEFT_H
#define CERN_UI_RIGHT_TO_LEFT_H 1

/**
 * Specifies a value indicating whether the text appears
 * from right to left, as when using Hebrew or Arabic fonts.
 */
typedef enum _CernRightToLeft {
  /**
   * The text reads from left to right. This is the default.
   */
  CernRightToLeft_No = 0,

  /**
   * The text reads from right to left.
   */
  CernRightToLeft_Yes = 1,

  /**
   * The direction the text appears in is inherited from the parent control.
   */
  CernRightToLeft_Inherit = 2
} CernRightToLeft;

#endif /* CERN_UI_RIGHT_TO_LEFT_H */
