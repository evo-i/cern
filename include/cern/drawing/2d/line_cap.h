#ifndef CERN_DRAWING_2D_LINE_CAP_H
#define CERN_DRAWING_2D_LINE_CAP_H 1

#include <glib-object.h>

G_BEGIN_DECLS

/**
 * @brief Line cap constants
 */
typedef enum _CernLineCap {
  /**
   * @brief Specifies a flat line cap.
   */
  CernLineCap_Flat             = 0,
  /**
   * @brief Specifies a square line cap.
   */
  CernLineCap_Square           = 1,
  /**
   * @brief Specifies a round line cap.
   */
  CernLineCap_Round            = 2,
  /**
   * @brief Specifies a triangular line cap.
   */
  CernLineCap_Triangle         = 3,

  /**
   * @brief Specifies no anchor cap.
   */
  CernLineCap_NoAnchor         = 0x10, // corresponds to flat cap
  /**
   * @brief Specifies a square anchor cap.
   */
  CernLineCap_SquareAnchor     = 0x11, // corresponds to square cap
  /**
   * @brief Specifies a round anchor cap.
   */
  CernLineCap_RoundAnchor      = 0x12, // corresponds to round cap
  /**
   * @brief Specifies a diamond anchor cap.
   */
  CernLineCap_DiamondAnchor    = 0x13, // corresponds to triangle cap
  /**
   * @brief Specifies an arrow-shaped anchor cap.
   */
  CernLineCap_ArrowAnchor      = 0x14, // no correspondence

  /**
   * @brief Specifies a custom line cap.
   */
  CernLineCap_Custom           = 0xff, // custom cap

  /**
   * @brief Specifies a mask used to check whether a line cap is an anchor cap.
   */
  CernLineCap_AnchorMask       = 0xf0  // mask to check for anchor or not.
} CernLineCap;

G_END_DECLS

#endif /* CERN_DRAWING_2D_LINE_CAP_H */
