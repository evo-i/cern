#ifndef CERN_DRAWING_INTERNAL_REGION_FLAGS_H
#define CERN_DRAWING_INTERNAL_REGION_FLAGS_H 1

typedef enum _CernRegionFlags {
  CernRegionFlags_Error           = 0,
  CernRegionFlags_NullRegion      = 1,
  CernRegionFlags_SimpleRegion    = 2,
  CernRegionFlags_ComplexRegion   = 3,
} CernRegionFlags;

#endif /* CERN_DRAWING_INTERNAL_REGION_FLAGS_H */
