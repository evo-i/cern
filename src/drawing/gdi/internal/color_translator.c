#include "cern/drawing/internal/color_translator.h"
#include "cern/drawing/color.h"

#define WIN32_RED_SHIFT 0
#define WIN32_GREEN_SHIFT 8
#define WIN32_BLUE_SHIFT 16

gint32
cern_color_translator_to_win32(CernColor *self) {
  gint32 argb;

  gint8 r, g, b;

  r = cern_color_get_red(self);
  g = cern_color_get_green(self);
  b = cern_color_get_blue(self);

  return
    (gint32)
      (r << WIN32_RED_SHIFT
        | g << WIN32_GREEN_SHIFT
        | b << WIN32_BLUE_SHIFT);
}

CernColor *
cern_color_translator_from_win32(gint32 argb) {
  return
    cern_color_translator_from_ole(argb);
}

gint32
cern_color_translator_to_ole(CernColor *self) {
  if (cern_color_is_known(self)) {
    switch (cern_color_to_known_color(self)) {
      case CernKnownColor_ActiveBorder:
        return 0x8000000A;
      case CernKnownColor_ActiveCaption:
        return 0x80000002;
      case CernKnownColor_ActiveCaptionText:
        return 0x80000009;
      case CernKnownColor_AppWorkspace:
        return 0x8000000C;
      case CernKnownColor_ButtonFace:
        return 0x8000000F;
      case CernKnownColor_ButtonHighlight:
        return 0x80000014;
      case CernKnownColor_ButtonShadow:
        return 0x80000010;
      case CernKnownColor_Control:
        return 0x8000000F;
      case CernKnownColor_ControlDark:
        return 0x80000010;
      case CernKnownColor_ControlDarkDark:
        return 0x80000015;
      case CernKnownColor_ControlLight:
        return 0x80000016;
      case CernKnownColor_ControlLightLight:
        return 0x80000014;
      case CernKnownColor_ControlText:
        return 0x80000012;
      case CernKnownColor_Desktop:
        return 0x80000001;
      case CernKnownColor_GradientActiveCaption:
        return 0x8000001B;
      case CernKnownColor_GradientInactiveCaption:
        return 0x8000001C;
      case CernKnownColor_GrayText:
        return 0x80000011;
      case CernKnownColor_Highlight:
        return 0x8000000D;
      case CernKnownColor_HighlightText:
        return 0x8000000E;
      case CernKnownColor_HotTrack:
        return 0x8000001A;
      case CernKnownColor_InactiveBorder:
        return 0x8000000B;
      case CernKnownColor_InactiveCaption:
        return 0x80000003;
      case CernKnownColor_InactiveCaptionText:
        return 0x80000013;
      case CernKnownColor_Info:
        return 0x80000018;
      case CernKnownColor_InfoText:
        return 0x80000017;
      case CernKnownColor_Menu:
        return 0x80000004;
      case CernKnownColor_MenuBar:
        return 0x8000001E;
      case CernKnownColor_MenuHighlight:
        return 0x8000001D;
      case CernKnownColor_MenuText:
        return 0x80000007;
      case CernKnownColor_ScrollBar:
        return 0x80000000;
      case CernKnownColor_Window:
        return 0x80000005;
      case CernKnownColor_WindowFrame:
        return 0x80000006;
      case CernKnownColor_WindowText:
        return 0x80000008;
    }
  }

  return cern_color_translator_to_win32(self);
}

CernColor *
cern_color_translator_from_ole(gint32 ole) {
  gint32 argb_color;

  if (((ole & 0xff000000) == 0x80000000
        && (ole & 0xffffff) <= 0x1E)) {
    switch (ole) {
      case 0x8000000A:
        return cern_color_from_known_color(CernKnownColor_ActiveBorder);
      case 0x80000002:
        return cern_color_from_known_color(CernKnownColor_ActiveCaption);
      case 0x80000009:
        return cern_color_from_known_color(CernKnownColor_ActiveCaptionText);
      case 0x8000000C:
        return cern_color_from_known_color(CernKnownColor_AppWorkspace);
      case 0x8000000F:
        return cern_color_from_known_color(CernKnownColor_Control);
      case 0x80000010:
        return cern_color_from_known_color(CernKnownColor_ControlDark);
      case 0x80000015:
        return cern_color_from_known_color(CernKnownColor_ControlDarkDark);
      case 0x80000016:
        return cern_color_from_known_color(CernKnownColor_ControlLight);
      case 0x80000014:
        return cern_color_from_known_color(CernKnownColor_ControlLightLight);
      case 0x80000012:
        return cern_color_from_known_color(CernKnownColor_ControlText);
      case 0x80000001:
        return cern_color_from_known_color(CernKnownColor_Desktop);
      case 0x8000001B:
        return cern_color_from_known_color(CernKnownColor_GradientActiveCaption);
      case 0x8000001C:
        return cern_color_from_known_color(CernKnownColor_GradientInactiveCaption);
      case 0x80000011:
        return cern_color_from_known_color(CernKnownColor_GrayText);
      case 0x8000000D:
        return cern_color_from_known_color(CernKnownColor_Highlight);
      case 0x8000000E:
        return cern_color_from_known_color(CernKnownColor_HighlightText);
      case 0x8000001A:
        return cern_color_from_known_color(CernKnownColor_HotTrack);
      case 0x8000000B:
        return cern_color_from_known_color(CernKnownColor_InactiveBorder);
      case 0x80000003:
        return cern_color_from_known_color(CernKnownColor_InactiveCaption);
      case 0x80000013:
        return cern_color_from_known_color(CernKnownColor_InactiveCaptionText);
      case 0x80000018:
        return cern_color_from_known_color(CernKnownColor_Info);
      case 0x80000017:
        return cern_color_from_known_color(CernKnownColor_InfoText);
      case 0x80000004:
        return cern_color_from_known_color(CernKnownColor_Menu);
      case 0x8000001E:
        return cern_color_from_known_color(CernKnownColor_MenuBar);
      case 0x8000001D:
        return cern_color_from_known_color(CernKnownColor_MenuHighlight);
      case 0x80000007:
        return cern_color_from_known_color(CernKnownColor_MenuText);
      case 0x80000000:
        return cern_color_from_known_color(CernKnownColor_ScrollBar);
      case 0x80000005:
        return cern_color_from_known_color(CernKnownColor_Window);
      case 0x80000006:
        return cern_color_from_known_color(CernKnownColor_WindowFrame);
      case 0x80000008:
        return cern_color_from_known_color(CernKnownColor_WindowText);
    }
  }

  argb_color
    = (ole >> WIN32_RED_SHIFT) & 0xff
      | (ole >> WIN32_GREEN_SHIFT) & 0xff
      | (ole >> WIN32_BLUE_SHIFT) & 0xff;

  return
    cern_color_from_argb(argb_color);
}
