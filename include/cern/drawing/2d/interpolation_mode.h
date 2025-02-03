#ifndef INTERPOLATION_MODE
#define INTERPOLATION_MODE 1

#include "quality_mode.h"

/// \brief Specifies how data is interpolated between endpoints.
typedef enum _CernInterpolationMode {
  /// \brief Equivalent to CernQualityMode_Invalid.
  CernInterpolationMode_Invalid = CernQualityMode_Invalid,
  /// \brief Specifies default mode.
  CernInterpolationMode_Default = CernQualityMode_Default,
  /// \brief Specifies low quality.
  CernInterpolationMode_Low = CernQualityMode_Low,
  /// \brief Specifies high quality.
  CernInterpolationMode_High = CernQualityMode_High,
  /// \brief Specifies bilinear interpolation.
  CernInterpolationMode_Bilinear,
  /// \brief Specifies bicubic interpolation.
  CernInterpolationMode_Bicubic,
  /// \brief Specifies nearest neighbor interpolation.
  CernInterpolationMode_NearestNeighbor,
  /// \brief Specifies high quality bilinear interpolation.
  CernInterpolationMode_HighQualityBilinear,
  /// \brief Specifies high quality bicubic interpolation.
  CernInterpolationMode_HighQualityBicubic
} CernInterpolationMode;

#endif /* INTERPOLATION_MODE */
