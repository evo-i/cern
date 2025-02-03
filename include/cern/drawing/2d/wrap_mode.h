#ifndef CERN_DRAWING_2D_WRAP_MODE_H
#define CERN_DRAWING_2D_WRAP_MODE_H 1

/**
 * @enum CernWrapMode
 * @brief Specifies how a texture or gradient is tiled when it is larger than the area being filled.
 */
typedef enum _CernWrapMode
{
  /**
   * @brief Tiles the gradient or texture.
   */
  CernWrapMode_Tile = 0,
  /**
   * @brief Reverses the texture or gradient horizontally and then tiles the texture or gradient.
   */
  CernWrapMode_TileFlipX = 1,
  /**
   * @brief Reverses the texture or gradient vertically and then tiles the texture or gradient.
   */
  CernWrapMode_TileFlipY = 2,
  /**
   * @brief Reverses the texture or gradient horizontally and vertically and then tiles the texture or gradient.
   */
  CernWrapMode_TileFlipXY = 3,
  /**
   * @brief Clamps the texture or gradient to the object boundary.
   */
  CernWrapMode_Clamp = 4
} CernWrapMode;

#endif /* CERN_DRAWING_2D_WRAP_MODE_H */
