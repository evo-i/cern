#ifndef CERN_DRAWING_2D_COMPOSITING_MODE_H
#define CERN_DRAWING_2D_COMPOSITING_MODE_H 1

/// @brief Defines how the source image is composited with the background image.
typedef enum _CernCompositingMode {
  /// @brief The source pixels overwrite the background pixels.
  CernCompositingMode_SourceOver = 0,
  /// @brief The source pixels are combined with the background pixels.
  CernCompositingModeSourceCopy = 1
} CernCompositingMode;

#endif /* CERN_DRAWING_2D_COMPOSITING_MODE_H */
