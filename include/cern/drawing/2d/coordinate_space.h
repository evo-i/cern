#ifndef CERN_DRAWING_2D_COORDINATE_SPACE_H
#define CERN_DRAWING_2D_COORDINATE_SPACE_H 1

/**
 * Specifies the system to use when evaluating coordinates.
 */
typedef enum _CernCoordinateSpace {
  /**
   * Specifies that coordinates are in the world coordinate context. World
   * coordinates are used in a non physical enviroment such as a modeling
   * environment.
   */
  CernCoordinateSpace_World = 0,
  /**
   * Specifies that coordinates are in the page coordinate context. Page
   * coordinates are typically used in a multiple page documents environment.
   */
  CernCoordinateSpace_Page = 1,
  /**
   * Specifies that coordinates are in the device coordinate context. Device
   * coordinates occur in screen coordinates just before they are drawn on the
   * screen.
   */
  CernCoordinateSpace_Device = 2
} CernCoordinateSpace;
#endif /* CERN_DRAWING_2D_COORDINATE_SPACE_H */
