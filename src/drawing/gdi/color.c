#include "cern/drawing/color.h"

#define CERN_COLOR_STATE_KNOWN_COLOR_VALID  (gint16) 0x0001
#define CERN_COLOR_STATE_ARGB_VALUE_VALID   (gint16) 0x0002
#define CERN_COLOR_STATE_VALUE_MASK         (gint16) (CERN_COLOR_STATE_ARGB_VALUE_VALID)
#define CERN_COLOR_STATE_NAME_VALID         (gint16) 0x0008
#define CERN_COLOR_NOT_DEFINED_VALUE        (gint64) 0L

#define CERN_COLOR_ARGB_ALPHA_SHIFT         24
#define CERN_COLOR_ARGB_RED_SHIFT           16
#define CERN_COLOR_ARGB_GREEN_SHIFT         8
#define CERN_COLOR_ARGB_BLUE_SHIFT          0

G_DEFINE_BOXED_TYPE(CernColor, cern_color, cern_color_copy, cern_color_free)

static
CernColor
cern_color_create_known_color(CernKnownColor known_color) {
  return (CernColor) {
    .known_color = known_color,
    .state = CERN_COLOR_STATE_KNOWN_COLOR_VALID,
    0
  };
}

CernColor *
cern_color_new(void) {
  return g_new0(CernColor, 1);
}

CernColor *
cern_color_copy(CernColor *self) {
  CernColor *copy;

  copy = cern_color_new();

  if (self->name.freedable) {
    copy->name.value = g_strdup(self->name.value);
    copy->name.freedable = TRUE;
  }

  copy->value = self->value;
  copy->state = self->state;
  copy->known_color = self->known_color;

  return copy;
}

void
cern_color_free(CernColor *self) {
  if(!self->name.freedable) {
    g_warning("cern_color_free: An attempt was made to delete an object without directly allocating memory or the object from the cache.");
    return;
  }
  if (self->name.freedable) {
    g_free(self->name.value);
  }
  g_free(self);
}

static
CernColor *
cern_color_new_internal(gint64 value, gint16 state, gchar const *name, CernKnownColor known_color) {
  CernColor *self = cern_color_new();

  self->value = value;
  self->state = state;
  self->name.value = name ? g_strdup(name) : NULL;
  self->name.freedable = name ? TRUE : FALSE;
  self->known_color = known_color;

  return self;
}

CernColor
cern_color_create(gint64 value, gint16 state, gchar const *name, CernKnownColor known_color) {
  return (CernColor) {
    .known_color = known_color,
    .state = state,
    .value = value,
    .name.value = name ? g_strdup(name) : NULL,
    .name.freedable = name ? TRUE : FALSE
  };
}

static
gint64
cern_color_make_argb(guint8 alpha, guint8 red, guint8 green, guint8 blue) {
  return
    ((gint64) (guint32) red << CERN_COLOR_ARGB_RED_SHIFT |
      ((gint64) (guint32) green << CERN_COLOR_ARGB_GREEN_SHIFT) |
      ((gint64) (guint32) blue << CERN_COLOR_ARGB_BLUE_SHIFT) |
      ((gint64) (guint32) alpha << CERN_COLOR_ARGB_ALPHA_SHIFT));
}

CernColor
cern_color_from_argb(guint32 argb) {
  return
    cern_color_create((argb & 0xffffffff),
                   CERN_COLOR_STATE_ARGB_VALUE_VALID, NULL, 0);
}

CernColor
cern_color_from_argb_uint8(guint8 a, guint8 r, guint8 g, guint8 b) {
  return
    cern_color_create(cern_color_make_argb(a, r, g, b),
                      CERN_COLOR_STATE_ARGB_VALUE_VALID, NULL, 0);
}

void
cern_color_init_argb(CernColor *self, guint32 argb) {
  self->value = argb;
  self->state = CERN_COLOR_STATE_ARGB_VALUE_VALID;
  self->known_color = 0;
  self->name.value = NULL;
  self->name.freedable = FALSE;
}

CernColor
cern_color_from_argb_other(guint8 alpha, CernColor *other) {
  return
    cern_color_create(cern_color_make_argb(alpha,
                      cern_color_get_red(other),
                      cern_color_get_green(other),
                      cern_color_get_blue(other)),
                      CERN_COLOR_STATE_ARGB_VALUE_VALID, NULL, 0);
}

static
CernColor *
cern_color_new_from_known_color(CernKnownColor known_color) {
  return
    cern_color_new_internal(0, CERN_COLOR_STATE_KNOWN_COLOR_VALID, NULL, known_color);
}

CernColor
cern_color_from_known_color(CernKnownColor known_color) {
  return cern_color_create_known_color(known_color);
}

CernColor
cern_color_from_name(gchar const *name) {
  g_critical("cern_color_from_name: not implemented");
  return cern_color_empty();
}

guint8
cern_color_get_alpha(CernColor *self) {
  return (guint8) ((self->value >> CERN_COLOR_ARGB_ALPHA_SHIFT) & 0xff);
}

guint8
cern_color_get_red(CernColor *self) {
  return (guint8) ((self->value >> CERN_COLOR_ARGB_RED_SHIFT) & 0xff);
}

guint8
cern_color_get_green(CernColor *self) {
  return (guint8) ((self->value >> CERN_COLOR_ARGB_GREEN_SHIFT) & 0xff);
}

guint8
cern_color_get_blue(CernColor *self) {
  return (guint8) ((self->value >> CERN_COLOR_ARGB_BLUE_SHIFT) & 0xff);

}

gchar const *
cern_color_get_name(CernColor *self) {
  return self->name.value;
}

gboolean
cern_color_is_known(CernColor *self) {
  return self->known_color != 0;
}

gboolean
cern_color_is_empty(CernColor *self) {
  return self->value == CERN_COLOR_NOT_DEFINED_VALUE;
}

gboolean
cern_color_is_named(CernColor *self) {
  return self->name.value != NULL;
}

gboolean
cern_color_is_system(CernColor *self) {
  return self->known_color == 0;
}

float
cern_color_get_brightness(CernColor *self) {
  return 0.0f;
}

float
cern_color_get_hue(CernColor *self) {
  return 0.0f;
}

float
cern_color_get_saturation(CernColor *self) {
  return 0.0f;
}

CernKnownColor
cern_color_to_known_color(CernColor *self) {
  return self->known_color;
}

static
struct {
  CernKnownColor known_color;
  CernColor const *color;
} known_color_table[CERN_KNOWN_COLOR_END] = { 0 };

static
void
cern_color_table_ensure_created(void) {
  if (known_color_table[0].color) {
    return;
  }

  CernKnownColor i;
  for (i = CernKnownColor_ActiveBorder; i < CERN_KNOWN_COLOR_END; ++i) {
    known_color_table[i].known_color = i;
    known_color_table[i].color = cern_color_new_from_known_color(i);
  }
}

static
CernColor
cern_copy_of_table_value(CernKnownColor known_color) {
  cern_color_table_ensure_created();
  CernColor copy = (*known_color_table[known_color].color);
  copy.name.freedable = FALSE;
  return copy;
}

static
gint32
cern_color_known_color_to_argb(CernKnownColor known_color) {
  cern_color_table_ensure_created();
  if (known_color <= CernKnownColor_MenuHighlight) {
    return known_color_table[known_color].color->value;
  }
  return 0;
}

gint32
cern_color_to_argb(CernColor *self) {
  if ((self->state & CERN_COLOR_STATE_VALUE_MASK)) {
    return (gint32) self->value;
  }
  if (cern_color_is_known(self)) {
    return cern_color_known_color_to_argb(self->known_color);
  }

  return 0;
}

CernColor
cern_color_empty(void) {
  return (CernColor) { 0 };
}

CernColor
cern_color_transparent(void) {
  return cern_copy_of_table_value(CernKnownColor_Transparent);
}

CernColor
cern_color_alice_blue(void) {
  return cern_copy_of_table_value(CernKnownColor_AliceBlue);
}

CernColor
cern_color_antique_white(void) {
  return cern_copy_of_table_value(CernKnownColor_AntiqueWhite);
}

CernColor
cern_color_aqua(void) {
  return cern_copy_of_table_value(CernKnownColor_Aqua);
}

CernColor
cern_color_aquamarine(void) {
  return cern_copy_of_table_value(CernKnownColor_Aquamarine);
}

CernColor
cern_color_azure(void) {
  return cern_copy_of_table_value(CernKnownColor_Azure);
}

CernColor
cern_color_beige(void) {
  return cern_copy_of_table_value(CernKnownColor_Beige);
}

CernColor
cern_color_bisque(void) {
  return cern_copy_of_table_value(CernKnownColor_Bisque);
}

CernColor
cern_color_black(void) {
  return cern_copy_of_table_value(CernKnownColor_Black);
}

CernColor
cern_color_blanched_almond(void) {
  return cern_copy_of_table_value(CernKnownColor_BlanchedAlmond);
}

CernColor
cern_color_blue(void) {
  return cern_copy_of_table_value(CernKnownColor_Blue);
}

CernColor
cern_color_blue_violet(void) {
  return cern_copy_of_table_value(CernKnownColor_BlueViolet);
}

CernColor
cern_color_brown(void) {
  return cern_copy_of_table_value(CernKnownColor_Brown);
}

CernColor
cern_color_burly_wood(void) {
  return cern_copy_of_table_value(CernKnownColor_BurlyWood);
}

CernColor
cern_color_cadet_blue(void) {
  return cern_copy_of_table_value(CernKnownColor_CadetBlue);
}

CernColor
cern_color_chartreuse(void) {
  return cern_copy_of_table_value(CernKnownColor_Chartreuse);
}

CernColor
cern_color_chocolate(void) {
  return cern_copy_of_table_value(CernKnownColor_Chocolate);
}

CernColor
cern_color_coral(void) {
  return cern_copy_of_table_value(CernKnownColor_Coral);
}

CernColor
cern_color_cornflower_blue(void) {
  return cern_copy_of_table_value(CernKnownColor_CornflowerBlue);
}

CernColor
cern_color_cornsilk(void) {
  return cern_copy_of_table_value(CernKnownColor_Cornsilk);
}

CernColor
cern_color_crimson(void) {
  return cern_copy_of_table_value(CernKnownColor_Crimson);
}

CernColor
cern_color_cyan(void) {
  return cern_copy_of_table_value(CernKnownColor_Cyan);
}

CernColor
cern_color_dark_blue(void) {
  return cern_copy_of_table_value(CernKnownColor_DarkBlue);
}

CernColor
cern_color_dark_cyan(void) {
  return cern_copy_of_table_value(CernKnownColor_DarkCyan);
}

CernColor
cern_color_dark_goldenrod(void) {
  return cern_copy_of_table_value(CernKnownColor_DarkGoldenrod);
}

CernColor
cern_color_dark_gray(void) {
  return cern_copy_of_table_value(CernKnownColor_DarkGray);
}

CernColor
cern_color_dark_green(void) {
  return cern_copy_of_table_value(CernKnownColor_DarkGreen);
}

CernColor
cern_color_dark_khaki(void) {
  return cern_copy_of_table_value(CernKnownColor_DarkKhaki);
}

CernColor
cern_color_dark_magenta(void) {
  return cern_copy_of_table_value(CernKnownColor_DarkMagenta);
}

CernColor
cern_color_dark_olive_green(void) {
  return cern_copy_of_table_value(CernKnownColor_DarkOliveGreen);
}

CernColor
cern_color_dark_orange(void) {
  return cern_copy_of_table_value(CernKnownColor_DarkOrange);
}

CernColor
cern_color_dark_orchid(void) {
  return cern_copy_of_table_value(CernKnownColor_DarkOrchid);
}

CernColor
cern_color_dark_red(void) {
  return cern_copy_of_table_value(CernKnownColor_DarkRed);
}

CernColor
cern_color_dark_salmon(void) {
  return cern_copy_of_table_value(CernKnownColor_DarkSalmon);
}

CernColor
cern_color_dark_sea_green(void) {
  return cern_copy_of_table_value(CernKnownColor_DarkSeaGreen);
}

CernColor
cern_color_dark_slate_blue(void) {
  return cern_copy_of_table_value(CernKnownColor_DarkSlateBlue);
}

CernColor
cern_color_dark_slate_gray(void) {
  return cern_copy_of_table_value(CernKnownColor_DarkSlateGray);
}

CernColor
cern_color_dark_turquoise(void) {
  return cern_copy_of_table_value(CernKnownColor_DarkTurquoise);
}

CernColor
cern_color_dark_violet(void) {
  return cern_copy_of_table_value(CernKnownColor_DarkViolet);
}

CernColor
cern_color_deep_pink(void) {
  return cern_copy_of_table_value(CernKnownColor_DeepPink);
}

CernColor
cern_color_deep_sky_blue(void) {
  return cern_copy_of_table_value(CernKnownColor_DeepSkyBlue);
}

CernColor
cern_color_dim_gray(void) {
  return cern_copy_of_table_value(CernKnownColor_DimGray);
}

CernColor
cern_color_dodger_blue(void) {
  return cern_copy_of_table_value(CernKnownColor_DodgerBlue);
}

CernColor
cern_color_firebrick(void) {
  return cern_copy_of_table_value(CernKnownColor_Firebrick);
}

CernColor
cern_color_floral_white(void) {
  return cern_copy_of_table_value(CernKnownColor_FloralWhite);
}

CernColor
cern_color_forest_green(void) {
  return cern_copy_of_table_value(CernKnownColor_ForestGreen);
}

CernColor
cern_color_fuchsia(void) {
  return cern_copy_of_table_value(CernKnownColor_Fuchsia);
}

CernColor
cern_color_gainsboro(void) {
  return cern_copy_of_table_value(CernKnownColor_Gainsboro);
}

CernColor
cern_color_ghost_white(void) {
  return cern_copy_of_table_value(CernKnownColor_GhostWhite);
}

CernColor
cern_color_gold(void) {
  return cern_copy_of_table_value(CernKnownColor_Gold);
}

CernColor
cern_color_goldenrod(void) {
  return cern_copy_of_table_value(CernKnownColor_Goldenrod);
}

CernColor
cern_color_gray(void) {
  return cern_copy_of_table_value(CernKnownColor_Gray);
}

CernColor
cern_color_green(void) {
  return cern_copy_of_table_value(CernKnownColor_Green);
}

CernColor
cern_color_green_yellow(void) {
  return cern_copy_of_table_value(CernKnownColor_GreenYellow);
}

CernColor
cern_color_honeydew(void) {
  return cern_copy_of_table_value(CernKnownColor_Honeydew);
}

CernColor
cern_color_hot_pink(void) {
  return cern_copy_of_table_value(CernKnownColor_HotPink);
}

CernColor
cern_color_indian_red(void) {
  return cern_copy_of_table_value(CernKnownColor_IndianRed);
}

CernColor
cern_color_indigo(void) {
  return cern_copy_of_table_value(CernKnownColor_Indigo);
}

CernColor
cern_color_ivory(void) {
  return cern_copy_of_table_value(CernKnownColor_Ivory);
}

CernColor
cern_color_khaki(void) {
  return cern_copy_of_table_value(CernKnownColor_Khaki);
}

CernColor
cern_color_lavender(void) {
  return cern_copy_of_table_value(CernKnownColor_Lavender);
}

CernColor
cern_color_lavender_blush(void) {
  return cern_copy_of_table_value(CernKnownColor_LavenderBlush);
}

CernColor
cern_color_lawn_green(void) {
  return cern_copy_of_table_value(CernKnownColor_LawnGreen);
}

CernColor
cern_color_lemon_chiffon(void) {
  return cern_copy_of_table_value(CernKnownColor_LemonChiffon);
}

CernColor
cern_color_light_blue(void) {
  return cern_copy_of_table_value(CernKnownColor_LightBlue);
}

CernColor
cern_color_light_coral(void) {
  return cern_copy_of_table_value(CernKnownColor_LightCoral);
}

CernColor
cern_color_light_cyan(void) {
  return cern_copy_of_table_value(CernKnownColor_LightCyan);
}

CernColor
cern_color_light_goldenrod_yellow(void) {
  return cern_copy_of_table_value(CernKnownColor_LightGoldenrodYellow);
}

CernColor
cern_color_light_green(void) {
  return cern_copy_of_table_value(CernKnownColor_LightGreen);
}

CernColor
cern_color_light_gray(void) {
  return cern_copy_of_table_value(CernKnownColor_LightGray);
}

CernColor
cern_color_light_pink(void) {
  return cern_copy_of_table_value(CernKnownColor_LightPink);
}

CernColor
cern_color_light_salmon(void) {
  return cern_copy_of_table_value(CernKnownColor_LightSalmon);
}

CernColor
cern_color_light_sea_green(void) {
  return cern_copy_of_table_value(CernKnownColor_LightSeaGreen);
}

CernColor
cern_color_light_sky_blue(void) {
  return cern_copy_of_table_value(CernKnownColor_LightSkyBlue);
}

CernColor
cern_color_light_slate_gray(void) {
  return cern_copy_of_table_value(CernKnownColor_LightSlateGray);
}

CernColor
cern_color_light_steel_blue(void) {
  return cern_copy_of_table_value(CernKnownColor_LightSteelBlue);
}

CernColor
cern_color_light_yellow(void) {
  return cern_copy_of_table_value(CernKnownColor_LightYellow);
}

CernColor
cern_color_lime(void) {
  return cern_copy_of_table_value(CernKnownColor_Lime);
}

CernColor
cern_color_lime_green(void) {
  return cern_copy_of_table_value(CernKnownColor_LimeGreen);
}

CernColor
cern_color_linen(void) {
  return cern_copy_of_table_value(CernKnownColor_Linen);
}

CernColor
cern_color_magenta(void) {
  return cern_copy_of_table_value(CernKnownColor_Magenta);
}

CernColor
cern_color_maroon(void) {
  return cern_copy_of_table_value(CernKnownColor_Maroon);
}

CernColor
cern_color_medium_aquamarine(void) {
  return cern_copy_of_table_value(CernKnownColor_MediumAquamarine);
}

CernColor
cern_color_medium_blue(void) {
  return cern_copy_of_table_value(CernKnownColor_MediumBlue);
}

CernColor
cern_color_medium_orchid(void) {
  return cern_copy_of_table_value(CernKnownColor_MediumOrchid);
}

CernColor
cern_color_medium_purple(void) {
  return cern_copy_of_table_value(CernKnownColor_MediumPurple);
}

CernColor
cern_color_medium_sea_green(void) {
  return cern_copy_of_table_value(CernKnownColor_MediumSeaGreen);
}

CernColor
cern_color_medium_slate_blue(void) {
  return cern_copy_of_table_value(CernKnownColor_MediumSlateBlue);
}

CernColor
cern_color_medium_spring_green(void) {
  return cern_copy_of_table_value(CernKnownColor_MediumSpringGreen);
}

CernColor
cern_color_medium_turquoise(void) {
  return cern_copy_of_table_value(CernKnownColor_MediumTurquoise);
}

CernColor
cern_color_medium_violet_red(void) {
  return cern_copy_of_table_value(CernKnownColor_MediumVioletRed);
}

CernColor
cern_color_midnight_blue(void) {
  return cern_copy_of_table_value(CernKnownColor_MidnightBlue);
}

CernColor
cern_color_mint_cream(void) {
  return cern_copy_of_table_value(CernKnownColor_MintCream);
}

CernColor
cern_color_misty_rose(void) {
  return cern_copy_of_table_value(CernKnownColor_MistyRose);
}

CernColor
cern_color_moccasin(void) {
  return cern_copy_of_table_value(CernKnownColor_Moccasin);
}

CernColor
cern_color_navajo_white(void) {
  return cern_copy_of_table_value(CernKnownColor_NavajoWhite);
}

CernColor
cern_color_navy(void) {
  return cern_copy_of_table_value(CernKnownColor_Navy);
}

CernColor
cern_color_old_lace(void) {
  return cern_copy_of_table_value(CernKnownColor_OldLace);
}

CernColor
cern_color_olive(void) {
  return cern_copy_of_table_value(CernKnownColor_Olive);
}

CernColor
cern_color_olive_drab(void) {
  return cern_copy_of_table_value(CernKnownColor_OliveDrab);
}

CernColor
cern_color_orange(void) {
  return cern_copy_of_table_value(CernKnownColor_Orange);
}

CernColor
cern_color_orange_red(void) {
  return cern_copy_of_table_value(CernKnownColor_OrangeRed);
}

CernColor
cern_color_orchid(void) {
  return cern_copy_of_table_value(CernKnownColor_Orchid);
}

CernColor
cern_color_pale_goldenrod(void) {
  return cern_copy_of_table_value(CernKnownColor_PaleGoldenrod);
}

CernColor
cern_color_pale_green(void) {
  return cern_copy_of_table_value(CernKnownColor_PaleGreen);
}

CernColor
cern_color_pale_turquoise(void) {
  return cern_copy_of_table_value(CernKnownColor_PaleTurquoise);
}

CernColor
cern_color_pale_violet_red(void) {
  return cern_copy_of_table_value(CernKnownColor_PaleVioletRed);
}

CernColor
cern_color_papaya_whip(void) {
  return cern_copy_of_table_value(CernKnownColor_PapayaWhip);
}

CernColor
cern_color_peach_puff(void) {
  return cern_copy_of_table_value(CernKnownColor_PeachPuff);
}

CernColor
cern_color_peru(void) {
  return cern_copy_of_table_value(CernKnownColor_Peru);
}

CernColor
cern_color_pink(void) {
  return cern_copy_of_table_value(CernKnownColor_Pink);
}

CernColor
cern_color_plum(void) {
  return cern_copy_of_table_value(CernKnownColor_Plum);
}

CernColor
cern_color_powder_blue(void) {
  return cern_copy_of_table_value(CernKnownColor_PowderBlue);
}

CernColor
cern_color_purple(void) {
  return cern_copy_of_table_value(CernKnownColor_Purple);
}

CernColor
cern_color_red(void) {
  return cern_copy_of_table_value(CernKnownColor_Red);
}

CernColor
cern_color_rosy_brown(void) {
  return cern_copy_of_table_value(CernKnownColor_RosyBrown);
}

CernColor
cern_color_royal_blue(void) {
  return cern_copy_of_table_value(CernKnownColor_RoyalBlue);
}

CernColor
cern_color_saddle_brown(void) {
  return cern_copy_of_table_value(CernKnownColor_SaddleBrown);
}

CernColor
cern_color_salmon(void) {
  return cern_copy_of_table_value(CernKnownColor_Salmon);
}

CernColor
cern_color_sandy_brown(void) {
  return cern_copy_of_table_value(CernKnownColor_SandyBrown);
}

CernColor
cern_color_sea_green(void) {
  return cern_copy_of_table_value(CernKnownColor_SeaGreen);
}

CernColor
cern_color_sea_shell(void) {
  return cern_copy_of_table_value(CernKnownColor_SeaShell);
}

CernColor
cern_color_sienna(void) {
  return cern_copy_of_table_value(CernKnownColor_Sienna);
}

CernColor
cern_color_silver(void) {
  return cern_copy_of_table_value(CernKnownColor_Silver);
}

CernColor
cern_color_sky_blue(void) {
  return cern_copy_of_table_value(CernKnownColor_SkyBlue);
}

CernColor
cern_color_slate_blue(void) {
  return cern_copy_of_table_value(CernKnownColor_SlateBlue);
}

CernColor
cern_color_slate_gray(void) {
  return cern_copy_of_table_value(CernKnownColor_SlateGray);
}

CernColor
cern_color_snow(void) {
  return cern_copy_of_table_value(CernKnownColor_Snow);
}

CernColor
cern_color_spring_green(void) {
  return cern_copy_of_table_value(CernKnownColor_SpringGreen);
}

CernColor
cern_color_steel_blue(void) {
  return cern_copy_of_table_value(CernKnownColor_SteelBlue);
}

CernColor
cern_color_tan(void) {
  return cern_copy_of_table_value(CernKnownColor_Tan);
}

CernColor
cern_color_teal(void) {
  return cern_copy_of_table_value(CernKnownColor_Teal);
}

CernColor
cern_color_thistle(void) {
  return cern_copy_of_table_value(CernKnownColor_Thistle);
}

CernColor
cern_color_tomato(void) {
  return cern_copy_of_table_value(CernKnownColor_Tomato);
}

CernColor
cern_color_turquoise(void) {
  return cern_copy_of_table_value(CernKnownColor_Turquoise);
}

CernColor
cern_color_violet(void) {
  return cern_copy_of_table_value(CernKnownColor_Violet);
}

CernColor
cern_color_wheat(void) {
  return cern_copy_of_table_value(CernKnownColor_Wheat);
}

CernColor
cern_color_white(void) {
  return cern_copy_of_table_value(CernKnownColor_White);
}

CernColor
cern_color_white_smoke(void) {
  return cern_copy_of_table_value(CernKnownColor_WhiteSmoke);
}

CernColor
cern_color_yellow(void) {
  return cern_copy_of_table_value(CernKnownColor_Yellow);
}

CernColor
cern_color_yellow_green(void) {
  return cern_copy_of_table_value(CernKnownColor_YellowGreen);
}
