#ifndef CERN_DRAWING_2D_PEN_ALIGNMENT_H
#define CERN_DRAWING_2D_PEN_ALIGNMENT_H

#include <glib-object.h>

G_BEGIN_DECLS

/**
 * @brief Specifies the alignment of a Pen in relation to the line being drawn.
 */
typedef enum _CernPenAlignment {
  /**
   * @brief Specifies that the Pen is positioned at the center of the line being drawn.
   */
  CernPenAlignment_Center      = 0,
  /**
   * @brief Specifies that the Pen is positioned on the inside of the line being drawn.
   */
  CernPenAlignment_Inset       = 1,
  /**
   * @brief Specifies that the Pen is positioned on the outside of the line being drawn.
   */
  CernPenAlignment_Outset      = 2,
  /**
   * @brief Specifies that the Pen is positioned to the left of the line being drawn.
   */
  CernPenAlignment_Left        = 3,
  /**
   * @brief Specifies that the Pen is positioned to the right of the line being drawn.
   */
  CernPenAlignment_Right       = 4
} CernPenAlignment;

G_END_DECLS

#endif /* CERN_DRAWING_2D_PEN_ALIGNMENT_H */
