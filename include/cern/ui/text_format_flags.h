#ifndef CERN_UI_TEXT_FOMRAT_FLAGS_H
#define CERN_UI_TEXT_FOMRAT_FLAGS_H 1

typedef enum _CernTextFormatFlags {
  CernTextFormatFlags_Bottom                            = 0x00000008,
  CernTextFormatFlags_CalculateRectangle                = 0x00000400,
  CernTextFormatFlags_EndEllipsis                       = 0x00008000,
  CernTextFormatFlags_ExpandTabs                        = 0x00000040,
  CernTextFormatFlags_ExternalLeading                   = 0x00000200,
  CernTextFormatFlags_HidePrefix                        = 0x00100000,
  CernTextFormatFlags_HorizontalCenter                  = 0x00000001,
  CernTextFormatFlags_Internal                          = 0x00001000,
  CernTextFormatFlags_Left                              = 0x00000000,
  CernTextFormatFlags_ModifyString                      = 0x00010000,
  CernTextFormatFlags_NoClipping                        = 0x00000100,
  CernTextFormatFlags_NoPrefix                          = 0x00000800,
  CernTextFormatFlags_NoFullWidthCharacterBreak         = 0x00080000,
  CernTextFormatFlags_PathEllipsis                      = 0x00004000,
  CernTextFormatFlags_PrefixOnly                        = 0x00200000,
  CernTextFormatFlags_Right                               = 0x00000002,
  CernTextFormatFlags_RightToLeft                         = 0x00020000,
  CernTextFormatFlags_SingleLine                          = 0x00000020,
  CernTextFormatFlags_TextBoxControl                      = 0x00002000,
  CernTextFormatFlags_Top                                 = 0x00000000,
  CernTextFormatFlags_VerticalCenter                      = 0x00000004,
  CernTextFormatFlags_WordBreak                           = 0x00000010,
  CernTextFormatFlags_WordEllipsis                        = 0x00040000,
  CernTextFormatFlags_PreserveGraphicsClipping            = 0x01000000,
  CernTextFormatFlags_PreserveGraphicsTranslateTransform  = 0x02000000,
  CernTextFormatFlags_GlyphOverhangPadding                = 0x00000000,
  CernTextFormatFlags_NoPadding                           = 0x10000000,
  CernTextFormatFlags_LeftAndRightPadding                 = 0x20000000,
  CernTextFormatFlags_Default                             = CernTextFormatFlags_Top 
                                                              | CernTextFormatFlags_Left
} CernTextFormatFlags;

#endif /* CERN_UI_TEXT_FOMRAT_FLAGS_H */
