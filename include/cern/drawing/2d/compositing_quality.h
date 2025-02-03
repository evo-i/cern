#ifndef CERN_DRAWING_2D_COMPOSITING_QUALITY_H
#define CERN_DRAWING_2D_COMPOSITING_QUALITY_H 1

#include <glib-object.h>
#include "quality_mode.h"

G_BEGIN_DECLS

/**
 * @enum _CernCompositingQuality
 * @brief Specifies the quality level to use during compositing.
 */
typedef enum _CernCompositingQuality {
  /**
   * @brief Invalid quality.
   */
  CernCompositingQuality_Invalid = CernQualityMode_Invalid,

  /**
   * @brief Default quality.
   */
  CernCompositingQuality_Default = CernQualityMode_Default,

  /**
   * @brief Low quality, high speed.
   */
  CernCompositingQuality_HighSpeed = CernQualityMode_Low,

  /**
   * @brief High quality, low speed.
   */
  CernCompositingQuality_HighQuality = CernQualityMode_High,

  /**
   * @brief Gamma correction is used.
   */
  CernCompositingQuality_GammaCorrected,

  /**
   * @brief Assume linear values.
   */
  CernCompositingQuality_AssumeLinear
} CernCompositingQuality;

G_END_DECLS

#endif /* CERN_DRAWING_2D_COMPOSITING_QUALITY_H */
