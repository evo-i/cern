#ifndef CERN_DRAWING_TEXT_HENDERING_HINT_H
#define CERN_DRAWING_TEXT_HENDERING_HINT_H 1

/// \brief Specifies the quality of text rendering.
typedef enum _CernTextRenderingHint {
  /// \brief [To be supplied.]
  SystemDefault = 0,        // Glyph with system default rendering hint
  /// \brief [To be supplied.]
  SingleBitPerPixelGridFit, // Glyph bitmap with hinting
  /// \brief [To be supplied.]
  SingleBitPerPixel,        // Glyph bitmap without hinting
  /// \brief [To be supplied.]
  AntiAliasGridFit,         // Anti-aliasing with hinting
  /// \brief [To be supplied.]
  AntiAlias,                // Glyph anti-alias bitmap without hinting
  /// \brief [To be supplied.]
  ClearTypeGridFit          // Glyph CT bitmap with hinting
} CernTextRenderingHint;

#endif /* CERN_DRAWING_TEXT_HENDERING_HINT_H */
