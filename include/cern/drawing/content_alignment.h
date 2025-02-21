#ifndef CERN_DRAWING_CONTENT_ALIGNMENT_H
#define CERN_DRAWING_CONTENT_ALIGNMENT_H 1

/**
 * @brief Specifies alignment of content on the drawing surface.
 */
typedef enum _CernContentAlignment {
  /**
   * Content is vertically aligned at the top, and horizontally
   * aligned on the left.
   */
  CernContentAlignment_TopLeft = 0x001,
  /**
   * Content is vertically aligned at the top, and
   * horizontally aligned at the center.
   */
  CernContentAlignment_TopCenter = 0x002,
  /**
   * Content is vertically aligned at the top, and
   * horizontally aligned on the right.
   */
  CernContentAlignment_TopRight = 0x004,
  /**
   * Content is vertically aligned in the middle, and
   * horizontally aligned on the left.
   */
  CernContentAlignment_MiddleLeft = 0x010,
  /**
   * Content is vertically aligned in the middle, and
   * horizontally aligned at the center.
   */
  CernContentAlignment_MiddleCenter = 0x020,
  /**
   * Content is vertically aligned in the middle, and horizontally aligned on the
   * right.
   */
  CernContentAlignment_MiddleRight = 0x040,
  /**
   * Content is vertically aligned at the bottom, and horizontally aligned on the
   * left.
   */
  CernContentAlignment_BottomLeft = 0x100,
  /**
   * Content is vertically aligned at the bottom, and horizontally aligned at the
   * center.
   */
  CernContentAlignment_BottomCenter = 0x200,
  /**
   * Content is vertically aligned at the bottom, and horizontally aligned on the
   * right.
   */
  CernContentAlignment_BottomRight = 0x400,
} CernContentAlignment;

#endif /* CERN_DRAWING_CONTENT_ALIGNMENT_H */
