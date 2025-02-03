#ifndef CERN_DRAWING_COPY_PIXEL_OPERATION_H
#define CERN_DRAWING_COPY_PIXEL_OPERATION_H 1

/// \brief Specifies the Copy Pixel (ROP) operation.
typedef enum _CernCopyPixelOperation {

  /// \brief Fills the Destination Rectangle using the color associated with the index 0 in the physical palette.
  CernCopyPixelOperation_Blackness = 0x00000042,

  /// \brief Includes any windows that are Layered on Top.
  CernCopyPixelOperation_CaptureBlt = 0x40000000,

  /// \brief DestinationInvert.
  CernCopyPixelOperation_DestinationInvert = 0x00550009,

  /// \brief MergeCopy.
  CernCopyPixelOperation_MergeCopy = 0x00C000CA,

  /// \brief MergePaint.
  CernCopyPixelOperation_MergePaint = 0x00BB0226,

  /// \brief NoMirrorBitmap.
  CernCopyPixelOperation_NoMirrorBitmap = 0x80000000,

  /// \brief NotSourceCopy.
  CernCopyPixelOperation_NotSourceCopy = 0x00330008,

  /// \brief NotSourceErase.
  CernCopyPixelOperation_NotSourceErase = 0x001100A6,

  /// \brief PatCopy.
  CernCopyPixelOperation_PatCopy = 0x00F00021,

  /// \brief PatInvert.
  CernCopyPixelOperation_PatInvert = 0x005A0049,

  /// \brief PatPaint.
  CernCopyPixelOperation_PatPaint = 0x00FB0A09,

  /// \brief SourceAnd.
  CernCopyPixelOperation_SourceAnd = 0x008800C6,

  /// \brief SourceCopy.
  CernCopyPixelOperation_SourceCopy = 0x00CC0020,

  /// \brief SourceErase.
  CernCopyPixelOperation_SourceErase = 0x00440328,

  /// \brief SourceInvert.
  CernCopyPixelOperation_SourceInvert = 0x00660046,

  /// \brief SourcePaint.
  CernCopyPixelOperation_SourcePaint = 0x00EE0086,

  /// \brief Whiteness.
  CernCopyPixelOperation_Whiteness = 0x00FF0062,
} CernCopyPixelOperation;

#endif /* CERN_DRAWING_COPY_PIXEL_OPERATION_H */
