#ifndef CERN_DRAWING_DEVICE_CONTEXT_TYPE_H
#define CERN_DRAWING_DEVICE_CONTEXT_TYPE_H 1

enum CernDeviceContextType {
  // Unknown device
  CernDeviceContextType_Unknown     = 0x00,

  // Display DC - obtained from GetDC/GetDCEx/BeginPaint.
  CernDeviceContextType_Display     = 0x01,

  // Window DC including non-client area - obtained from GetWindowDC
  CernDeviceContextType_NCWindow    = 0x02,

  // Printer DC - obtained from CreateDC.
  CernDeviceContextType_NamedDevice = 0x03,

  // Information context - obtained from CreateIC.
  CernDeviceContextType_Information = 0x04,

  // Memory dc - obtained from CreateCompatibleDC.
  CernDeviceContextType_Memory      = 0x05,

  // Metafile dc - obtained from CreateEnhMetafile.
  CernDeviceContextType_Metafile    = 0x06 // currently not supported.
};

#endif /* CERN_DRAWING_DEVICE_CONTEXT_TYPE_H */
