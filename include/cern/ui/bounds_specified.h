#ifndef CERN_UI_BOUNDS_SPECIFIED_H
#define CERN_UI_BOUNDS_SPECIFIED_H 1

/**
 * @brief Specifies the bounds of the control to use when defining a control's size and position.
 */
typedef enum _CernBoundsSpecified {
  /**
   * @brief Specifies the left edge of the control is defined.
   */
  CernBoundsSpecified_X = 0x1,
  /**
   * @brief Specifies the top edge of the control of the control is defined.
   */
  CernBoundsSpecified_Y = 0x2,
  /**
   * @brief Specifies the width of the control is defined.
   */
  CernBoundsSpecified_Width = 0x4,
  /**
   * @brief Specifies the height of the control is defined.
   */
  CernBoundsSpecified_Height = 0x8,
  /**
   * @brief Both X and Y coordinates of the control are defined.
   */
  CernBoundsSpecified_Location = CernBoundsSpecified_X | CernBoundsSpecified_Y,
  /**
   * @brief Both Width and Height property values of the control are defined.
   */
  CernBoundsSpecified_Size = CernBoundsSpecified_Width | CernBoundsSpecified_Height,
  /**
   * @brief Both Location and Size property values are defined.
   */
  CernBoundsSpecified_All = CernBoundsSpecified_Location | CernBoundsSpecified_Size,
  /**
   * @brief No bounds are specified.
   */
  CernBoundsSpecified_None = 0,
} CernBoundsSpecified;
#endif /* CERN_UI_BOUNDS_SPECIFIED_H */
