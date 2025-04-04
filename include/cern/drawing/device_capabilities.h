#ifndef CERN_DRAWING_DEVICE_CAPABILITIES_H
#define CERN_DRAWING_DEVICE_CAPABILITIES_H 1

typedef enum _CernDeviceCapabilities {
  CernDeviceCapabilities_DriverVersion = 0,
  CernDeviceCapabilities_Technology = 2,
  CernDeviceCapabilities_HorizontalSize = 4,
  CernDeviceCapabilities_VerticalSize = 6,
  CernDeviceCapabilities_HorizontalResolution = 8,
  CernDeviceCapabilities_VerticalResolution = 10,
  CernDeviceCapabilities_BitsPerPixel = 12,
  CernDeviceCapabilities_LogicalPixelsX = 88,
  CernDeviceCapabilities_LogicalPixelsY = 90,
  CernDeviceCapabilities_PhysicalWidth = 110,
  CernDeviceCapabilities_PhysicalHeight = 111,
  CernDeviceCapabilities_PhysicalOffsetX = 112,
  CernDeviceCapabilities_PhysicalOffsetY = 113,
  CernDeviceCapabilities_ScalingFactorX = 114,
  CernDeviceCapabilities_ScalingFactorY = 115,
} CernDeviceCapabilities;

#endif /* !CERN_DRAWING_DEVICE_CAPABILITIES_H */
