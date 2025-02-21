#ifndef CERN_DRAWING_INTERNAL_DEVICE_CONTEXT_GRAPHICS_MODE_H
#define CERN_DRAWING_INTERNAL_DEVICE_CONTEXT_GRAPHICS_MODE_H 1

/**
 * @brief Specifies the graphics mode of a device context.
 */
typedef enum _CernDeviceContextGraphicsMode {
  /**
   * @brief Sets the graphics mode that is compatible with 16-bit Windows. This is the default mode.
   *
   * If this value is specified, the application can only modify the world-to-device transform
   * by calling functions that set window and viewport extents and origins, but not by using
   * SetWorldTransform or ModifyWorldTransform; calls to those functions will fail.
   * Examples of functions that set window and viewport extents and origins are SetViewportExtEx
   * and SetWindowExtEx.
   */
  CernDeviceContextGraphicsMode_Compatible = 1,

  /**
   * @brief Sets the advanced graphics mode that allows world transformations.
   *
   * Windows NT/2000/XP: This value must be specified if the application will set or modify the world
   * transformation for the specified device context. In this mode all graphics, including text output,
   * fully conform to the world-to-device transformation specified in the device context.
   * Windows 95/98/Me: The CernDeviceContextGraphicsMode_Advanced value is not supported. When playing enhanced metafiles, the system
   * attempts to make enhanced metafiles look the same as they do on Windows NT/2000/XP. To accomplish this,
   * the system may simulate CernDeviceContextGraphicsMode_Advanced mode when playing specific enhanced metafile records.
   */
  CernDeviceContextGraphicsMode_Advanced   = 2,

  /**
   * @brief Resets the current world transformation by using the identity matrix.
   *
   * If this mode is specified, the XFORM structure pointed to by lpXform is ignored.
   */
  CernDeviceContextGraphicsMode_ModifyWorldIdentity = 1
} CernDeviceContextGraphicsMode;

#endif /* CERN_DRAWING_INTERNAL_DEVICE_CONTEXT_GRAPHICS_MODE_H */
