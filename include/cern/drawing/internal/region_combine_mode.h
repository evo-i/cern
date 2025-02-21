#ifndef CERN_DRAWING_INTERNAL_REGION_COMBINE_MODE_H
#define CERN_DRAWING_INTERNAL_REGION_COMBINE_MODE_H 1

typedef enum _CernRegionCombineMode {
  CernRegionCombineMode_And  = 1,
  CernRegionCombineMode_Or   = 2,
  CernRegionCombineMode_Xor  = 3,
  CernRegionCombineMode_Diff = 4,
  CernRegionCombineMode_Copy = 5,
  CernRegionCombineMode_Min  = CernRegionCombineMode_And,
  CernRegionCombineMode_Max  = CernRegionCombineMode_Copy
} CernRegionCombineMode;

#endif /* CERN_DRAWING_INTERNAL_REGION_COMBINE_MODE_H */
