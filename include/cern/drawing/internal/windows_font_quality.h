#ifndef CERN_DRAWING_INTERNAL_WINDOWS_FONT_QUALITY_H
#define CERN_DRAWING_INTERNAL_WINDOWS_FONT_QUALITY_H 1

/**
 * @brief Device capability indexes - See Win32 GetDeviceCaps().
 */
typedef enum _CernWindowsFontQuality {
  CernWindowsFontQuality_Default = 0,
  CernWindowsFontQuality_Draft = 1,
  CernWindowsFontQuality_Proof = 2,
  CernWindowsFontQuality_NonAntiAliased = 3,
  CernWindowsFontQuality_AntiAliased = 4,
  CernWindowsFontQuality_ClearType = 5,
  CernWindowsFontQuality_ClearTypeNatural = 6,
} CernWindowsFontQuality;

#endif /* CERN_DRAWING_INTERNAL_WINDOWS_FONT_QUALITY_H */
