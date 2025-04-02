#ifndef CERN_UI_TEXT_IMAGE_RELATION_H
#define CERN_UI_TEXT_IMAGE_RELATION_H 1

#include "anchor_styles.h"

typedef enum _CernTextImageRelation {
  CernTextImageRelation_Overlay = CernAnchorStyles_None,
  CernTextImageRelation_ImageBeforeText = CernAnchorStyles_Left,
  CernTextImageRelation_TextBeforeImage = CernAnchorStyles_Right,
  CernTextImageRelation_ImageAboveText = CernAnchorStyles_Top,
  CernTextImageRelation_TextAboveImage = CernAnchorStyles_Bottom
} CernTextImageRelation;

#endif /* CERN_UI_TEXT_IMAGE_RELATION_H */
