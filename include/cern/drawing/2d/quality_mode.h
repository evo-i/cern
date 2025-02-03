#ifndef CERN_DRAWING_2D_QUALITY_MODE_H
#define CERN_DRAWING_2D_QUALITY_MODE_H 1

#include <glib-object.h>

G_BEGIN_DECLS

/**
 * @enum _CernQualityMode
 * @brief Specifies the overall quality of rendering of graphics shapes.
 */
typedef enum _CernQualityMode {
  /**
   * @brief Specifies an invalid mode.
   */
  CernQualityMode_Invalid = -1,

  /**
   * @brief Specifies the default mode.
   */
  CernQualityMode_Default = 0,

  /**
   * @brief Specifies low quality, high performance rendering.
   * For applications that need the best performance.
   */
  CernQualityMode_Low = 1,

  /**
   * @brief Specifies high quality, lower performance rendering.
   * For applications that need the best rendering quality.
   */
  CernQualityMode_High = 2
} CernQualityMode;

G_END_DECLS

#endif /* CERN_DRAWING_2D_QUALITY_MODE_H */
