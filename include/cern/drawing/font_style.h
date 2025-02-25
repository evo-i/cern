#ifndef CERN_DRAWING_FONT_STYLE_H
#define CERN_DRAWING_FONT_STYLE_H 1

/**
 * @brief Specifies style information applied to text.
 */
typedef enum _CernFontStyle {
  /**
   * @brief Normal text.
   */
  CernFontStyle_Regular   = 0,
  /**
   * @brief Bold text.
   */
  CernFontStyle_Bold      = 1,
  /**
   * @brief Italic text.
   */
  CernFontStyle_Italic    = 2,
  /**
   * @brief Underlined text.
   */
  CernFontStyle_Underline = 4,
  /**
   * @brief Text with a line through the middle.
   */
  CernFontStyle_Strikeout = 8,
} CernFontStyle;
#endif /* CERN_DRAWING_FONT_STYLE_H */
