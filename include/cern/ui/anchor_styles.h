#ifndef CERN_UI_ANCHOR_STYLES_H
#define CERN_UI_ANCHOR_STYLES_H 1

/**
 * @brief Specifies how a control anchors to the edges of its container.
 */
typedef enum _CernAnchorStyles {
  /**
   * @brief The control is anchored to the top edge of its container.
   */
  CernAnchorStyles_Top = 0x01,
  /**
   * @brief The control is anchored to the bottom edge of its container.
   */
  CernAnchorStyles_Bottom = 0x02,
  /**
   * @brief The control is anchored to the left edge of its container.
   */
  CernAnchorStyles_Left = 0x04,
  /**
   * @brief The control is anchored to the right edge of its container.
   */
  CernAnchorStyles_Right = 0x08,

  /**
   * @brief The control is not anchored to any edges of its container.
   */
  CernAnchorStyles_None = 0,
} CernAnchorStyles;

#endif /* CERN_UI_ANCHOR_STYLES_H */
