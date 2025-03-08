#ifndef CERN_DRAWING_2F_DASH_STYLE_H
#define CERN_DRAWING_2F_DASH_STYLE_H 1


/**
 * @brief Specifies the style of dashed lines drawn with a Pen
 */
typedef enum _CernDashStyle {
  /**
   * @brief Specifies a solid line
   */
  CernDashStyle_Solid = 0,
  /**
   * @brief Specifies a line comprised of dashes
   */
  CernDashStyle_Dash = 1,
  /**
   * @brief Specifies a line comprised of dots
   */
  CernDashStyle_Dot = 2,
  /**
   * @brief Specifies a line comprised of an alternating pattern of dash-dot-dash-dot
   */
  CernDashStyle_DashDot = 3,
  /**
   * @brief Specifies a line comprised of an alternating pattern of dash-dot-dot-dash-dot-dot
   */
  CernDashStyle_DashDotDot = 4,
  /**
   * @brief Specifies a user-defined custom dash style
   */
  CernDashStyle_Custom = 5
} CernDashStyle;

#endif /* CERN_DRAWING_2F_DASH_STYLE_H */
