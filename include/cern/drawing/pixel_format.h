#ifndef CERN_PIXEL_FORMAT_H
#define CERN_PIXEL_FORMAT_H 1

typedef enum CernPixelFormat {
  /**
   * Specifies that pixel data contains
   * color indexed values which means they are an index to colors in the system color
   * table, as opposed to individual color values.
   */
  CernPixelFormat_Indexed      = 0x00010000,
  /**
   * Specifies that pixel data contains GDI
   * colors.
   */
  CernPixelFormat_Gdi          = 0x00020000,
  /**
   * Specifies that pixel data contains alpha values that are
   * not pre-multiplied.
   */
  CernPixelFormat_Alpha        = 0x00040000,
  /**
   * Specifies that pixel format contains pre-multipled alpha values.
   */
  CernPixelFormat_PAlpha       = 0x00080000,
  /**
   * Specifies that
   */
  CernPixelFormat_Extended     = 0x00100000,
  /**
   * Specifies that
   */
  CernPixelFormat_Canonical    = 0x00200000,
  /**
   * Specifies that pixel format is undefined.
   */
  CernPixelFormat_Undefined        =  0,
  /**
   * Specifies that pixel format is a don't care.
   */
  CernPixelFormat_DontCare         =  0,
  /**
   * Specifies that pixel format is 1 bit per pixel indexed
   * color. The color table therefore has two colors in it.
   */
  CernPixelFormat_Format1bppIndexed
    =  1 | ( 1 << 8)
         | CernPixelFormat_Indexed
         | CernPixelFormat_Gdi,
  /**
   * Specifies that pixel format is 4 bits per pixel indexed
   * color. The color table therefore has 16 colors in it.
   */
  CernPixelFormat_Format4bppIndexed
    =  2 | ( 4 << 8)
         | CernPixelFormat_Indexed
         | CernPixelFormat_Gdi,
  /**
   * Specifies that pixel format is 8 bits per pixel indexed
   * color. The color table therefore has 256 colors in it.
   */
  CernPixelFormat_Format8bppIndexed
    =  3 | (8 << 8)
         | CernPixelFormat_Indexed
         | CernPixelFormat_Gdi,
  /**
   * [To be supplied.]
   */
  CernPixelFormat_Format16bppGrayScale
    =  4 | (16 << 8)
         | CernPixelFormat_Extended,
  /**
   * Specifies that pixel format is 16 bits per pixel. The
   * color information specifies 65536 shades of gray.
   */
  CernPixelFormat_Format16bppRgb555
    =  5 | (16 << 8)
         | CernPixelFormat_Gdi,
  /**
   * Specifies that pixel format is 16 bits per pixel. The
   * color information specifies 32768 shades of color of which 5 bits are red, 5
   * bits are green and 5 bits are blue.
   */
  CernPixelFormat_Format16bppRgb565
    =  6 | (16 << 8)
         | CernPixelFormat_Gdi,
  /**
   * Specifies that pixel format
   * is 16 bits per pixel. The color information specifies 32768
   * shades of color of which 5 bits are red, 5 bits are green, 5
   * bits are blue and 1 bit is alpha.
   */
  CernPixelFormat_Format16bppArgb1555
    =  7 | (16 << 8)
         | CernPixelFormat_Alpha
         | CernPixelFormat_Gdi,
  /**
   * Specifies that pixel format is 24 bits per pixel. The
   * color information specifies 16777216 shades of color of which 8 bits are red, 8
   * bits are green and 8 bits are blue.
   */
  CernPixelFormat_Format24bppRgb
    =  8 | (24 << 8)
         | CernPixelFormat_Gdi,
  /**
   * Specifies that pixel format is 24 bits per pixel. The
   * color information specifies 16777216 shades of color of which 8 bits are red, 8
   * bits are green and 8 bits are blue.
   */
  CernPixelFormat_Format32bppRgb
    =  9 | (32 << 8)
         | CernPixelFormat_Gdi,
  /**
   * Specifies that pixel format is 32 bits per pixel. The
   * color information specifies 16777216 shades of color of which 8 bits are red, 8
   * bits are green and 8 bits are blue. The 8 additional bits are alpha bits.
   */
  CernPixelFormat_Format32bppArgb
    = 10 | (32 << 8)
         | CernPixelFormat_Alpha
         | CernPixelFormat_Gdi
         | CernPixelFormat_Canonical,
  /**
   * Specifies that pixel format is 32 bits per pixel. The color information
   * specifies 16777216 shades of color of which 8 bits are red, 8 bits are
   * green and 8 bits are blue. The 8 additional bits are pre-multiplied alpha bits.
   */
  CernPixelFormat_Format32bppPArgb
    = 11 | (32 << 8)
         | CernPixelFormat_Alpha
         | CernPixelFormat_PAlpha
         | CernPixelFormat_Gdi,
  /**
   * Specifies that pixel format is 48 bits per pixel.
   * The color information specifies 16777216 shades of color of which 8 bits are
   * red, 8 bits are green and 8 bits are blue. The 8 additional bits are alpha bits.
   */
  CernPixelFormat_Format48bppRgb
    = 12 | (48 << 8)
         | CernPixelFormat_Extended,
  /**
   * Specifies pixel format is 64 bits per pixel. The
   * color information specifies 16777216 shades of color of which 16 bits are red, 16
   * bits are green and 16 bits are blue. The 16 additional bits are alpha bits.
   */
  CernPixelFormat_Format64bppArgb
    = 13 | (64 << 8)
         | CernPixelFormat_Alpha
         | CernPixelFormat_Canonical
         | CernPixelFormat_Extended,
  /**
   * Specifies that pixel format is 64 bits per pixel. The
   * color information specifies 16777216 shades of color of which 16 bits are red,
   * 16 bits are green and 16 bits are blue. The 16 additional bits are
   * pre-multiplied alpha bits.
   */
  CernPixelFormat_Format64bppPArgb
    = 14 | (64 << 8)
         | CernPixelFormat_Alpha
         | CernPixelFormat_PAlpha
         | CernPixelFormat_Extended,

  /**
   * Specifies that pixel format is 64 bits per pixel. The
   * color information specifies 16777216 shades of color of which 16 bits are red,
   * 16 bits are green and 16 bits are blue. The 16 additional bits are alpha bits.
   */
  CernPixelFormat_Max                   = 15,

  CernPixelFormat_Unused = 0xffffffff
} CernPixelFormat;


/**
 * @brief Return the pixel size for the specified format (in bits)
 */
inline
gint32
cern_get_pixel_format_size(CernPixelFormat pixel_format) {
  return((pixel_format >> 8) & 0xFF);
}


/**
 * @brief Determine if the pixel format can have alpha channel
 * @return Returns a value indicating whether the
 * pixel format contains alpha information.
 */
inline
gboolean
cern_is_alpha_pixel_format(CernPixelFormat pixel_format) {
  return((pixel_format & CernPixelFormat_Alpha) != 0);
}

/**
 * @brief Determine if the pixel format is an extended format,
 * i.e. supports 16-bit per channel.
 */
inline
gboolean
cern_is_extended_pixel_format(CernPixelFormat pixel_format) {
  return((pixel_format & CernPixelFormat_Extended) != 0);
}


/**
 * @brief Determine if the pixel format is canonical format:
 * CernPixelFormat_32bppARGB
 * CernPixelFormat_32bppPARGB
 * CernPixelFormat_64bppARGB
 * CernPixelFormat_64bppPARGB
 * @return Returns a value indicating whether the pixel format is canonical.
 */
inline
gboolean
cern_is_canonical_pixel_format(CernPixelFormat pixel_format) {
  return((pixel_format & CernPixelFormat_Canonical) != 0);
}

#endif /* CERN_PIXEL_FORMAT_H */
