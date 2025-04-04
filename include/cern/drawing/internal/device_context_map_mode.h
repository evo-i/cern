#ifndef CERN_DRAWING_INTERNAL_DEVICE_CONTEXT_MAP_MODE_H
#define CERN_DRAWING_INTERNAL_DEVICE_CONTEXT_MAP_MODE_H 1

typedef enum _CernDeviceContextMapMode {
  CernDeviceContextMapMode_Text = 1,
  CernDeviceContextMapMode_LoMetric = 2,
  CernDeviceContextMapMode_HiMetric = 3,
  CernDeviceContextMapMode_LoEnglish = 4,
  CernDeviceContextMapMode_HiEnglish = 5,
  CernDeviceContextMapMode_Twips = 6,
  CernDeviceContextMapMode_Isotropic = 7,
  CernDeviceContextMapMode_Anisotropic = 8,
} CernDeviceContextMapMode;

#endif // !CERN_DRAWING_INTERNAL_DEVICE_CONTEXT_MAP_MODE_H

