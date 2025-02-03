#ifndef CERN_DRAWING_2D_PIXEL_OFFSET_MODE_H
#define CERN_DRAWING_2D_PIXEL_OFFSET_MODE_H 1

#include "quality_mode.h"

/// <include file='doc\PixelOffsetMode.uex' path='docs/doc[@for="PixelOffsetMode"]/*' />
/// <devdoc>
///    Specifies how pixels are offset during
///    rendering.
/// </devdoc>
typedef enum _CernPixelOffsetMode {
  /// <include file='doc\PixelOffsetMode.uex' path='docs/doc[@for="PixelOffsetMode.Invalid"]/*' />
  /// <devdoc>
  ///    Specifies an invalid mode.
  /// </devdoc>
  CernPixelOffsetMode_Invalid = CernQualityMode_Invalid,
  /// <include file='doc\PixelOffsetMode.uex' path='docs/doc[@for="PixelOffsetMode.Default"]/*' />
  /// <devdoc>
  ///    Specifies the default mode.
  /// </devdoc>
  CernPixelOffsetMode_Default = CernQualityMode_Default,
  /// <include file='doc\PixelOffsetMode.uex' path='docs/doc[@for="PixelOffsetMode.HighSpeed"]/*' />
  /// <devdoc>
  ///    Specifies high low quality (high
  ///    performance) mode.
  /// </devdoc>
  CernPixelOffsetMode_HighSpeed = CernQualityMode_Low,
  /// <include file='doc\PixelOffsetMode.uex' path='docs/doc[@for="PixelOffsetMode.HighQuality"]/*' />
  /// <devdoc>
  ///    Specifies high quality (lower performance)
  ///    mode.
  /// </devdoc>
  CernPixelOffsetMode_HighQuality = CernQualityMode_High,
  /// <include file='doc\PixelOffsetMode.uex' path='docs/doc[@for="PixelOffsetMode.None"]/*' />
  /// <devdoc>
  ///    Specifies no pixel offset.
  /// </devdoc>
  CernPixelOffsetMode_None,                   // no pixel offset
  /// <include file='doc\PixelOffsetMode.uex' path='docs/doc[@for="PixelOffsetMode.Half"]/*' />
  /// <devdoc>
  ///    Specifies that pixels are offset by -.5
  ///    units both horizontally and vertically for high performance anti-aliasing.
  /// </devdoc>
  CernPixelOffsetMode_Half                    // offset by -0.5, -0.5 for fast anti-alias perf
} CernPixelOffsetMode;

#endif /* CERN_DRAWING_2D_PIXEL_OFFSET_MODE_H */
