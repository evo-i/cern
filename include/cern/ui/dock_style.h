#ifndef CERN_UI_DOCK_STYLE_H
#define CERN_UI_DOCK_STYLE_H 1

/**
 * @brief Control Dock values.
 *
 * When a control is docked to an edge of it's container it will
 * always be positioned flush against that edge while the container
 * resizes. If more than one control is docked to an edge, the controls
 * will not be placed on top of each other.
 */
typedef enum _CernDockStyle {
  /**
   * @brief No docking
   */
  CernDockStyle_None = 0,
  /**
   * @brief Dock to the top edge
   */
  CernDockStyle_Top = 1,
  /**
   * @brief Dock to the bottom edge
   */
  CernDockStyle_Bottom = 2,
  /**
   * @brief Dock to the left edge
   */
  CernDockStyle_Left = 3,
  /**
   * @brief Dock to the right edge
   */
  CernDockStyle_Right = 4,
  /**
   * @brief Fill the entire container
   */
  CernDockStyle_Fill = 5,
} CernDockStyle;

#endif /* CERN_UI_DOCK_STYLE_H */
