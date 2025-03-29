#ifndef CERN_UI_IMAGE_LAYOUT_H
#define CERN_UI_IMAGE_LAYOUT_H 1

/**
 * @brief Specifies the position of the image on the control.
 */
typedef enum _CernImageLayout {
  /**
   * @brief The image is aligned TOP - LEFT across the controls client rectangle.
   */
  CernImageLayout_None,
  /**
   * @brief The image is tiled across the controls client rectangle.
   */
  CernImageLayout_Tile,
  /**
   * @brief The image is centred within the controls client rectangle.
   */
  CernImageLayout_Center,
  /**
   * @brief The image is streched across the controls client rectangle.
   */
  CernImageLayout_Stretch,
  /**
   * @brief The image is streched across the controls client rectangle.
   */
  CernImageLayout_Zoom,
} CernImageLayout;

#endif /* CERN_UI_IMAGE_LAYOUT_H */
