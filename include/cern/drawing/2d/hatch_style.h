#ifndef CERN_DRAWING_2D_HATCH_STYLE_H
#define CERN_DRAWING_2D_HATCH_STYLE_H 1

/**
 * @enum CernHatchStyle
 * @brief Specifies the different patterns available for HatchBrush objects.
 */
typedef enum _CernHatchStyle {
  /**
   * @brief A pattern of horizontal lines.
   */
  CernHatchStyle_Horizontal = 0,
  /**
   * @brief A pattern of vertical lines.
   */
  CernHatchStyle_Vertical = 1,
  /**
   * @brief A pattern of lines on a diagonal from top-left to bottom-right.
   */
  CernHatchStyle_ForwardDiagonal = 2,
  /**
   * @brief A pattern of lines on a diagonal from top-right to bottom-left.
   */
  CernHatchStyle_BackwardDiagonal = 3,
  /**
   * @brief A pattern of criss-cross horizontal and vertical lines.
   */
  CernHatchStyle_Cross = 4,
  /**
   * @brief A pattern of criss-cross diagonal lines.
   */
  CernHatchStyle_DiagonalCross = 5,
  /**
   * @brief A pattern of 5% fill.
   */
  CernHatchStyle_Percent05 = 6,
  /**
   * @brief A pattern of 10% fill.
   */
  CernHatchStyle_Percent10 = 7,
  /**
   * @brief A pattern of 20% fill.
   */
  CernHatchStyle_Percent20 = 8,
  /**
   * @brief A pattern of 25% fill.
   */
  CernHatchStyle_Percent25 = 9,
  /**
   * @brief A pattern of 30% fill.
   */
  CernHatchStyle_Percent30 = 10,
  /**
   * @brief A pattern of 40% fill.
   */
  CernHatchStyle_Percent40 = 11,
  /**
   * @brief A pattern of 50% fill.
   */
  CernHatchStyle_Percent50 = 12,
  /**
   * @brief A pattern of 60% fill.
   */
  CernHatchStyle_Percent60 = 13,
  /**
   * @brief A pattern of 70% fill.
   */
  CernHatchStyle_Percent70 = 14,
  /**
   * @brief A pattern of 75% fill.
   */
  CernHatchStyle_Percent75 = 15,
  /**
   * @brief A pattern of 80% fill.
   */
  CernHatchStyle_Percent80 = 16,
  /**
   * @brief A pattern of 90% fill.
   */
  CernHatchStyle_Percent90 = 17,
  /**
   * @brief A pattern of light downward diagonal lines.
   */
  CernHatchStyle_LightDownwardDiagonal = 18,
  /**
   * @brief A pattern of light upward diagonal lines.
   */
  CernHatchStyle_LightUpwardDiagonal = 19,
  /**
   * @brief A pattern of dark downward diagonal lines.
   */
  CernHatchStyle_DarkDownwardDiagonal = 20,
  /**
   * @brief A pattern of dark upward diagonal lines.
   */
  CernHatchStyle_DarkUpwardDiagonal = 21,
  /**
   * @brief A pattern of wide downward diagonal lines.
   */
  CernHatchStyle_WideDownwardDiagonal = 22,
  /**
   * @brief A pattern of wide upward diagonal lines.
   */
  CernHatchStyle_WideUpwardDiagonal = 23,
  /**
   * @brief A pattern of light vertical lines.
   */
  CernHatchStyle_LightVertical = 24,
  /**
   * @brief A pattern of light horizontal lines.
   */
  CernHatchStyle_LightHorizontal = 25,
  /**
   * @brief A pattern of narrow vertical lines.
   */
  CernHatchStyle_NarrowVertical = 26,
  /**
   * @brief A pattern of narrow horizontal lines.
   */
  CernHatchStyle_NarrowHorizontal = 27,
  /**
   * @brief A pattern of dark vertical lines.
   */
  CernHatchStyle_DarkVertical = 28,
  /**
   * @brief A pattern of dark horizontal lines.
   */
  CernHatchStyle_DarkHorizontal = 29,
  /**
   * @brief A pattern of dashed downward diagonal lines.
   */
  CernHatchStyle_DashedDownwardDiagonal = 30,
  /**
   * @brief A pattern of dashed upward diagonal lines.
   */
  CernHatchStyle_DashedUpwardDiagonal = 31,
  /**
   * @brief A pattern of dashed horizontal lines.
   */
  CernHatchStyle_DashedHorizontal = 32,
  /**
   * @brief A pattern of dashed vertical lines.
   */
  CernHatchStyle_DashedVertical = 33,
  /**
   * @brief A pattern of small confetti.
   */
  CernHatchStyle_SmallConfetti = 34,
  /**
   * @brief A pattern of large confetti.
   */
  CernHatchStyle_LargeConfetti = 35,
  /**
   * @brief A pattern of zigzag lines.
   */
  CernHatchStyle_ZigZag = 36,
  /**
   * @brief A pattern of wave lines.
   */
  CernHatchStyle_Wave = 37,
  /**
   * @brief A pattern of diagonal bricks.
   */
  CernHatchStyle_DiagonalBrick = 38,
  /**
   * @brief A pattern of horizontal bricks.
   */
  CernHatchStyle_HorizontalBrick = 39,
  /**
   * @brief A pattern of weave.
   */
  CernHatchStyle_Weave = 40,
  /**
   * @brief A pattern of plaid.
   */
  CernHatchStyle_Plaid = 41,
  /**
   * @brief A pattern of divot.
   */
  CernHatchStyle_Divot = 42,
  /**
   * @brief A pattern of dotted grid.
   */
  CernHatchStyle_DottedGrid = 43,
  /**
   * @brief A pattern of dotted diamond.
   */
  CernHatchStyle_DottedDiamond = 44,
  /**
   * @brief A pattern of shingle.
   */
  CernHatchStyle_Shingle = 45,
  /**
   * @brief A pattern of trellis.
   */
  CernHatchStyle_Trellis = 46,
  /**
   * @brief A pattern of sphere.
   */
  CernHatchStyle_Sphere = 47,
  /**
   * @brief A pattern of small grid.
   */
  CernHatchStyle_SmallGrid = 48,
  /**
   * @brief A pattern of small checkerboard.
   */
  CernHatchStyle_SmallCheckerBoard = 49,
  /**
   * @brief A pattern of large checkerboard.
   */
  CernHatchStyle_LargeCheckerBoard = 50,
  /**
   * @brief A pattern of outlined diamond.
   */
  CernHatchStyle_OutlinedDiamond = 51,
  /**
   * @brief A pattern of solid diamond.
   */
  CernHatchStyle_SolidDiamond = 52,

  /**
   * @brief A pattern of large grid.
   */
  CernHatchStyle_LargeGrid = CernHatchStyle_Cross,

  /**
   * @brief Minimum hatch style value.
   */
  CernHatchStyle_Min = CernHatchStyle_Horizontal,
  /**
   * @brief Maximum hatch style value.
   */
  CernHatchStyle_Max = CernHatchStyle_LargeGrid

} CernHatchStyle;

#endif /* CERN_DRAWING_2D_HATCH_STYLE_H */
