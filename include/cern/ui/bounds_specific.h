#ifndef CERN_BOUNDS_SPECIFIC_H
#define CERN_BOUNDS_SPECIFIC_H 1

typedef enum _CernBoundsSpecified {
  /**
   * @brief Specifies the left edge of the control is defined.
   */
  CernBoundsSpecified_X = 0x1,
  /**
   * @brief Specifies the top edge of the control is defined.
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
   * @brief Both width and height property values of the control are defined.
   */
  CernBoundsSpecified_Size = CernBoundsSpecified_Width | CernBoundsSpecified_Height,
  /**
   * @brief Both location and size property values are defined.
   */
  CernBoundsSpecified_All = CernBoundsSpecified_Location | CernBoundsSpecified_Size,
  /**
   * @brief No bounds are specified.
   */
  CernBoundsSpecified_None = 0,
} CernBoundsSpecified;

#endif /* CERN_BOUNDS_SPECIFIC_H */
