#ifndef CERN_ROTATE_FLIP_TYPE_H
#define CERN_ROTATE_FLIP_TYPE_H 1

typedef enum _CernRotateFlipType {
  //! No rotation or flip.
  CernRotateNoneFlipNone = 0,
  //! Rotate 90 degrees without flip.
  CernRotate90FlipNone   = 1,
  //! Rotate 180 degrees without flip.
  CernRotate180FlipNone  = 2,
  //! Rotate 270 degrees without flip.
  CernRotate270FlipNone  = 3,
  //! No rotation with X-axis flip.
  CernRotateNoneFlipX    = 4,
  //! Rotate 90 degrees with X-axis flip.
  CernRotate90FlipX      = 5,
  //! Rotate 180 degrees with X-axis flip.
  CernRotate180FlipX     = 6,
  //! Rotate 270 degrees with X-axis flip.
  CernRotate270FlipX     = 7,
  //! No rotation with Y-axis flip.
  CernRotateNoneFlipY    = CernRotate180FlipX,
  //! Rotate 90 degrees with Y-axis flip.
  CernRotate90FlipY      = CernRotate270FlipX,
  //! Rotate 180 degrees with Y-axis flip.
  CernRotate180FlipY     = CernRotateNoneFlipX,
  //! Rotate 270 degrees with Y-axis flip.
  CernRotate270FlipY     = CernRotate90FlipX,
  //! No rotation with both X and Y axis flip.
  CernRotateNoneFlipXY   = CernRotate180FlipNone,
  //! Rotate 90 degrees with both X and Y axis flip.
  CernRotate90FlipXY     = CernRotate270FlipNone,
  //! Rotate 180 degrees with both X and Y axis flip.
  CernRotate180FlipXY    = CernRotateNoneFlipNone,
  //! Rotate 270 degrees with both X and Y axis flip.
  CernRotate270FlipXY    = CernRotate90FlipNone
} CernRotateFlipType;
#endif /* CERN_ROTATE_FLIP_TYPE_H */
