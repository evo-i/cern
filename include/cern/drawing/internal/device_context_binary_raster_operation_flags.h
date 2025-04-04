#ifndef CERN_DRAWING_INTERNAL_DEVICE_CONTEXT_BINARY_RASTER_OPERATION_FLAGS
#define CERN_DRAWING_INTERNAL_DEVICE_CONTEXT_BINARY_RASTER_OPERATION_FLAGS 1

typedef enum _CernDeviceContextBinaryRasterOperationFlags {
  CernDeviceContextBinaryRasterOperationFlags_Black = 1,
  CernDeviceContextBinaryRasterOperationFlags_NotMergePen = 2,
  CernDeviceContextBinaryRasterOperationFlags_MaskNotPen = 3,
  CernDeviceContextBinaryRasterOperationFlags_NotCopyPen = 4,
  CernDeviceContextBinaryRasterOperationFlags_MaskPenNot = 5,
  CernDeviceContextBinaryRasterOperationFlags_Not = 6,
  CernDeviceContextBinaryRasterOperationFlags_XorPen = 7,
  CernDeviceContextBinaryRasterOperationFlags_NotMaskPen = 8,
  CernDeviceContextBinaryRasterOperationFlags_MaskPen = 9,
  CernDeviceContextBinaryRasterOperationFlags_NotXorPen = 10,
  CernDeviceContextBinaryRasterOperationFlags_Nop = 11,
  CernDeviceContextBinaryRasterOperationFlags_MergeNotPen = 12,
  CernDeviceContextBinaryRasterOperationFlags_CopyPen = 13,
  CernDeviceContextBinaryRasterOperationFlags_MergePenNot = 14,
  CernDeviceContextBinaryRasterOperationFlags_MergePen = 15,
  CernDeviceContextBinaryRasterOperationFlags_White = 16,
} CernDeviceContextBinaryRasterOperationFlags;

#endif /* CERN_DRAWING_INTERNAL_DEVICE_CONTEXT_BINARY_RASTER_OPERATION_FLAGS */
