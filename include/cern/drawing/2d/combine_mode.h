#ifndef DERN_DRAWING_2D_COMBINE_MODE_H
#define DERN_DRAWING_2D_COMBINE_MODE_H 1

/**
 * @brief Defines how different clipping regions can be combined.
 */
typedef enum _CernCombineMode {
  /**
   * @brief One clipping region is replaced by another.
   */
  CernCombineMode_Replace = 0,

  /**
   * @brief The two clipping regions are combined by taking their intersection.
   */
  CernCombineMode_Intersect = 1,

  /**
   * @brief The two clipping regions are combined by taking the union of both.
   */
  CernCombineMode_Union = 2,

  /**
   * @brief The two clipping regions are combined by taking only the area enclosed by one or the other regions, but not both.
   */
  CernCombineMode_Xor = 3,

  /**
   * @brief Two clipping regions are combined by taking the area of the first region that does not intersect with the second.
   */
  CernCombineMode_Exclude = 4,

  /**
   * @brief Two clipping regions are combined by taking the area of the second region that does not intersect with the first.
   */
  CernCombineMode_Complement = 5
} CernCombineMode;

#endif /* DERN_DRAWING_2D_COMBINE_MODE_H */
