#ifndef CERN_UI_GET_CHILD_AT_POINT_SKIP_H
#define CERN_UI_GET_CHILD_AT_POINT_SKIP_H 1

typedef enum _CernGetChildAtPointSkip {
  CernGetChildAtPointSkip_None = 0x0000,
  CernGetChildAtPointSkip_Invisible = 0x0001,
  CernGetChildAtPointSkip_Disabled = 0x0002,
  CernGetChildAtPointSkip_Transparent = 0x0004
} CernGetChildAtPointSkip;

#endif /* CERN_UI_GET_CHILD_AT_POINT_SKIP_H */
