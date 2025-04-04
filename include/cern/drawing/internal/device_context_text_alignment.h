#ifndef CERN_DRAWING_INTERNAL_DEVICE_CONTEXT_TEXT_ALIGN_H
#define CERN_DRAWING_INTERNAL_DEVICE_CONTEXT_TEXT_ALIGN_H 1

typedef enum _CernDeviceContextTextAlignment {
  CernDeviceContextTextAlignment_BaseLine = 24,
  CernDeviceContextTextAlignment_Bottom = 8,
  CernDeviceContextTextAlignment_Top = 0,
  CernDeviceContextTextAlignment_Center = 6,
  CernDeviceContextTextAlignment_Default = 0,
  CernDeviceContextTextAlignment_Left = 0,
  CernDeviceContextTextAlignment_Right = 2,
  CernDeviceContextTextAlignment_RtlReading = 256,
  CernDeviceContextTextAlignment_NoUpdateCP = 0,
  CernDeviceContextTextAlignment_UpdateCP = 1,
  CernDeviceContextTextAlignment_VerticalBaseLine = 2,
  CernDeviceContextTextAlignment_VerticalCenter = 3,
} CernDeviceContextTextAlignment;

#endif // !CERN_DRAWING_INTERNAL_DEVICE_CONTEXT_TEXT_ALIGN_H

