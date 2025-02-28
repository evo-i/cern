#ifndef CERN_DRAWING_2D_PIXEL_OFFSET_MODE_H
#define CERN_DRAWING_2D_PIXEL_OFFSET_MODE_H 1

#include "quality_mode.h"

/**
 * @enum _CernPixelOffsetMode
 *
 * @brief Specifies how pixels are offset during rendering.
 */
typedef enum _CernPixelOffsetMode {
  /**
   * @brief Specifies an invalid mode.
   */
  CernPixelOffsetMode_Invalid = CernQualityMode_Invalid,
  /**
   * @brief Specifies the default mode.
   */
  CernPixelOffsetMode_Default = CernQualityMode_Default,
  /**
   * @brief Specifies high low quality (high performance) mode.
   */
  CernPixelOffsetMode_HighSpeed = CernQualityMode_Low,
  /**
   * @brief Specifies high quality (lower performance) mode.
   */
  CernPixelOffsetMode_HighQuality = CernQualityMode_High,
  /**
   * @brief Specifies no pixel offset.
   */
  CernPixelOffsetMode_None,
  /**
   * @brief Specifies that pixels are offset by -.5 units both horizontally and vertically for high performance anti-aliasing.
   */
  CernPixelOffsetMode_Half
} CernPixelOffsetMode;

#endif /* CERN_DRAWING_2D_PIXEL_OFFSET_MODE_H */
