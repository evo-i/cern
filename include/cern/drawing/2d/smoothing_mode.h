#ifndef CERN_DRAWING_2D_SMOOTHING_MODE_H
#define CERN_DRAWING_2D_SMOOTHING_MODE_H 1

#include "quality_mode.h"

/// \brief Specifies the overall quality of rendering of graphics shapes.
typedef enum _CernSmoothingMode {
  /// \brief Specifies an invalid mode.
  CernSmoothingMode_Invalid = CernQualityMode_Invalid,
  /// \brief Specifies the default mode.
  CernSmoothingMode_Default = CernQualityMode_Default,
  /// \brief Specifies low quality, high performance rendering.
  CernSmoothingMode_HighSpeed = CernQualityMode_Low,
  /// \brief Specifies high quality, lower performance rendering.
  CernSmoothingMode_HighQuality = CernQualityMode_High,
  /// \brief Specifies no anti-aliasing.
  CernSmoothingMode_None,
  /// \brief Specifies anti-aliased rendering.
  CernSmoothingMode_AntiAlias
} CernSmoothingMode;

#endif /* CERN_DRAWING_2D_SMOOTHING_MODE_H */
