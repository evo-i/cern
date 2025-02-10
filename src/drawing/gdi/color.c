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

struct _CernColor {
  GObject parent_instance;

  gchar *name;
  gint64 value;
  gint16 known_color;
  gint16 state;
};

G_DEFINE_FINAL_TYPE(CernColor, cern_color, G_TYPE_OBJECT)

static
void
cern_color_init(CernColor *self) {
  // Initialization code here
}

static
void
cern_color_finalize(GObject *object) {
  CernColor *self = CERN_COLOR(object);
  g_free(self->name);
  G_OBJECT_CLASS(cern_color_parent_class)->finalize(object);
}

static
void
cern_color_class_init(CernColorClass *klass) {
  GObjectClass *object_class = G_OBJECT_CLASS(klass);
  object_class->finalize = cern_color_finalize;
}

static
CernColor *
cern_color_new_known_color(CernKnownColor known_color) {
  CernColor *self = g_object_new(CERN_TYPE_COLOR, NULL);

  self->known_color = known_color;
  self->state = CERN_COLOR_STATE_KNOWN_COLOR_VALID;

  return self;
}

static
CernColor *
cern_color_new(gint64 value, gint16 state, gchar const *name, CernKnownColor known_color) {
  CernColor *self = g_object_new(CERN_TYPE_COLOR, NULL);

  self->value = value;
  self->state = state;
  self->name = name ? g_strdup(name) : NULL;
  self->known_color = known_color;

  return self;
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

CernColor *
cern_color_from_argb(guint32 argb) {
  return
    cern_color_new((argb & 0xffffffff),
                   CERN_COLOR_STATE_ARGB_VALUE_VALID, NULL, 0);
}

CernColor *
cern_color_from_argb_uint8(guint8 a, guint8 r, guint8 g, guint8 b) {
  return
    cern_color_new(cern_color_make_argb(a, r, g, b),
                   CERN_COLOR_STATE_ARGB_VALUE_VALID, NULL, 0);
}

CernColor *
cern_color_from_argb_other(guint8 alpha, CernColor *other) {
  return
    cern_color_new(cern_color_make_argb(alpha,
                                         cern_color_get_red(other),
                                         cern_color_get_green(other),
                                         cern_color_get_blue(other)),
                   CERN_COLOR_STATE_ARGB_VALUE_VALID, NULL, 0);
}

CernColor *
cern_color_from_known_color(CernKnownColor known_color) {
  return cern_color_new_known_color(known_color);
}

CernColor *
cern_color_from_name(gchar const *name) {

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
  return self->name;
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
  return self->name != NULL;
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
} knwon_color_table[CERN_KNOWN_COLOR_END] = { 0 };

static
void
cern_color_table_ensure_created() {
  if (knwon_color_table[0].color) {
    return;
  }

  CernKnownColor i;
  for (i = CernKnownColor_ActiveBorder; i < CERN_KNOWN_COLOR_END; ++i) {
    knwon_color_table[i].known_color = i;
    knwon_color_table[i].color = cern_color_from_known_color(i);
  }
}

static
gint32
cern_color_known_color_to_argb(CernKnownColor known_color) {
  cern_color_table_ensure_created();
  if (known_color <= CernKnownColor_MenuHighlight) {
    return knwon_color_table[known_color].color->value;
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

CernColor const *
cern_color_empty(void) {
  return NULL;
}

CernColor const *
cern_color_transparent(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_Transparent].color;
}

CernColor const *
cern_color_alice_blue(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_AliceBlue].color;
}

CernColor const *
cern_color_antique_white(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_AntiqueWhite].color;
}

CernColor const *
cern_color_aqua(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_Aqua].color;
}

CernColor const *
cern_color_aquamarine(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_Aquamarine].color;
}

CernColor const *
cern_color_azure(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_Azure].color;
}

CernColor const *
cern_color_beige(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_Beige].color;
}

CernColor const *
cern_color_bisque(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_Bisque].color;
}

CernColor const *
cern_color_black(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_Black].color;
}

CernColor const *
cern_color_blanched_almond(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_BlanchedAlmond].color;
}

CernColor const *
cern_color_blue(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_Blue].color;
}

CernColor const *
cern_color_blue_violet(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_BlueViolet].color;
}

CernColor const *
cern_color_brown(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_Brown].color;
}

CernColor const *
cern_color_burly_wood(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_BurlyWood].color;
}

CernColor const *
cern_color_cadet_blue(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_CadetBlue].color;
}

CernColor const *
cern_color_chartreuse(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_Chartreuse].color;
}

CernColor const *
cern_color_chocolate(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_Chocolate].color;
}

CernColor const *
cern_color_coral(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_Coral].color;
}

CernColor const *
cern_color_cornflower_blue(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_CornflowerBlue].color;
}

CernColor const *
cern_color_cornsilk(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_Cornsilk].color;
}

CernColor const *
cern_color_crimson(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_Crimson].color;
}

CernColor const *
cern_color_cyan(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_Cyan].color;
}

CernColor const *
cern_color_dark_blue(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_DarkBlue].color;
}

CernColor const *
cern_color_dark_cyan(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_DarkCyan].color;
}

CernColor const *
cern_color_dark_goldenrod(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_DarkGoldenrod].color;
}

CernColor const *
cern_color_dark_gray(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_DarkGray].color;
}

CernColor const *
cern_color_dark_green(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_DarkGreen].color;
}

CernColor const *
cern_color_dark_khaki(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_DarkKhaki].color;
}

CernColor const *
cern_color_dark_magenta(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_DarkMagenta].color;
}

CernColor const *
cern_color_dark_olive_green(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_DarkOliveGreen].color;
}

CernColor const *
cern_color_dark_orange(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_DarkOrange].color;
}

CernColor const *
cern_color_dark_orchid(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_DarkOrchid].color;
}

CernColor const *
cern_color_dark_red(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_DarkRed].color;
}

CernColor const *
cern_color_dark_salmon(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_DarkSalmon].color;
}

CernColor const *
cern_color_dark_sea_green(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_DarkSeaGreen].color;
}

CernColor const *
cern_color_dark_slate_blue(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_DarkSlateBlue].color;
}

CernColor const *
cern_color_dark_slate_gray(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_DarkSlateGray].color;
}

CernColor const *
cern_color_dark_turquoise(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_DarkTurquoise].color;
}

CernColor const *
cern_color_dark_violet(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_DarkViolet].color;
}

CernColor const *
cern_color_deep_pink(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_DeepPink].color;
}

CernColor const *
cern_color_deep_sky_blue(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_DeepSkyBlue].color;
}

CernColor const *
cern_color_dim_gray(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_DimGray].color;
}

CernColor const *
cern_color_dodger_blue(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_DodgerBlue].color;
}

CernColor const *
cern_color_firebrick(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_Firebrick].color;
}

CernColor const *
cern_color_floral_white(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_FloralWhite].color;
}

CernColor const *
cern_color_forest_green(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_ForestGreen].color;
}

CernColor const *
cern_color_fuchsia(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_Fuchsia].color;
}

CernColor const *
cern_color_gainsboro(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_Gainsboro].color;
}

CernColor const *
cern_color_ghost_white(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_GhostWhite].color;
}

CernColor const *
cern_color_gold(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_Gold].color;
}

CernColor const *
cern_color_goldenrod(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_Goldenrod].color;
}

CernColor const *
cern_color_gray(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_Gray].color;
}

CernColor const *
cern_color_green(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_Green].color;
}

CernColor const *
cern_color_green_yellow(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_GreenYellow].color;
}

CernColor const *
cern_color_honeydew(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_Honeydew].color;
}

CernColor const *
cern_color_hot_pink(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_HotPink].color;
}

CernColor const *
cern_color_indian_red(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_IndianRed].color;
}

CernColor const *
cern_color_indigo(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_Indigo].color;
}

CernColor const *
cern_color_ivory(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_Ivory].color;
}

CernColor const *
cern_color_khaki(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_Khaki].color;
}

CernColor const *
cern_color_lavender(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_Lavender].color;
}

CernColor const *
cern_color_lavender_blush(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_LavenderBlush].color;
}

CernColor const *
cern_color_lawn_green(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_LawnGreen].color;
}

CernColor const *
cern_color_lemon_chiffon(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_LemonChiffon].color;
}

CernColor const *
cern_color_light_blue(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_LightBlue].color;
}

CernColor const *
cern_color_light_coral(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_LightCoral].color;
}

CernColor const *
cern_color_light_cyan(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_LightCyan].color;
}

CernColor const *
cern_color_light_goldenrod_yellow(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_LightGoldenrodYellow].color;
}

CernColor const *
cern_color_light_green(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_LightGreen].color;
}

CernColor const *
cern_color_light_gray(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_LightGray].color;
}

CernColor const *
cern_color_light_pink(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_LightPink].color;
}

CernColor const *
cern_color_light_salmon(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_LightSalmon].color;
}

CernColor const *
cern_color_light_sea_green(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_LightSeaGreen].color;
}

CernColor const *
cern_color_light_sky_blue(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_LightSkyBlue].color;
}

CernColor const *
cern_color_light_slate_gray(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_LightSlateGray].color;
}

CernColor const *
cern_color_light_steel_blue(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_LightSteelBlue].color;
}

CernColor const *
cern_color_light_yellow(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_LightYellow].color;
}

CernColor const *
cern_color_lime(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_Lime].color;
}

CernColor const *
cern_color_lime_green(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_LimeGreen].color;
}

CernColor const *
cern_color_linen(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_Linen].color;
}

CernColor const *
cern_color_magenta(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_Magenta].color;
}

CernColor const *
cern_color_maroon(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_Maroon].color;
}

CernColor const *
cern_color_medium_aquamarine(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_MediumAquamarine].color;
}

CernColor const *
cern_color_medium_blue(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_MediumBlue].color;
}

CernColor const *
cern_color_medium_orchid(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_MediumOrchid].color;
}

CernColor const *
cern_color_medium_purple(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_MediumPurple].color;
}

CernColor const *
cern_color_medium_sea_green(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_MediumSeaGreen].color;
}

CernColor const *
cern_color_medium_slate_blue(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_MediumSlateBlue].color;
}

CernColor const *
cern_color_medium_spring_green(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_MediumSpringGreen].color;
}

CernColor const *
cern_color_medium_turquoise(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_MediumTurquoise].color;
}

CernColor const *
cern_color_medium_violet_red(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_MediumVioletRed].color;
}

CernColor const *
cern_color_midnight_blue(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_MidnightBlue].color;
}

CernColor const *
cern_color_mint_cream(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_MintCream].color;
}

CernColor const *
cern_color_misty_rose(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_MistyRose].color;
}

CernColor const *
cern_color_moccasin(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_Moccasin].color;
}

CernColor const *
cern_color_navajo_white(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_NavajoWhite].color;
}

CernColor const *
cern_color_navy(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_Navy].color;
}

CernColor const *
cern_color_old_lace(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_OldLace].color;
}

CernColor const *
cern_color_olive(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_Olive].color;
}

CernColor const *
cern_color_olive_drab(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_OliveDrab].color;
}

CernColor const *
cern_color_orange(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_Orange].color;
}

CernColor const *
cern_color_orange_red(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_OrangeRed].color;
}

CernColor const *
cern_color_orchid(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_Orchid].color;
}

CernColor const *
cern_color_pale_goldenrod(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_PaleGoldenrod].color;
}

CernColor const *
cern_color_pale_green(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_PaleGreen].color;
}

CernColor const *
cern_color_pale_turquoise(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_PaleTurquoise].color;
}

CernColor const *
cern_color_pale_violet_red(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_PaleVioletRed].color;
}

CernColor const *
cern_color_papaya_whip(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_PapayaWhip].color;
}

CernColor const *
cern_color_peach_puff(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_PapayaWhip].color;
}

CernColor const *
cern_color_peru(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_Peru].color;
}

CernColor const *
cern_color_pink(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_Pink].color;
}

CernColor const *
cern_color_plum(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_Plum].color;
}

CernColor const *
cern_color_powder_blue(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_PowderBlue].color;
}

CernColor const *
cern_color_purple(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_Purple].color;
}

CernColor const *
cern_color_red(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_Red].color;
}

CernColor const *
cern_color_rosy_brown(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_RosyBrown].color;
}

CernColor const *
cern_color_royal_blue(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_RoyalBlue].color;
}

CernColor const *
cern_color_saddle_brown(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_SaddleBrown].color;
}

CernColor const *
cern_color_salmon(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_Salmon].color;
}

CernColor const *
cern_color_sandy_brown(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_SandyBrown].color;
}

CernColor const *
cern_color_sea_green(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_SeaGreen].color;
}

CernColor const *
cern_color_sea_shell(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_SeaShell].color;
}

CernColor const *
cern_color_sienna(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_Sienna].color;
}

CernColor const *
cern_color_silver(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_Silver].color;
}

CernColor const *
cern_color_sky_blue(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_SkyBlue].color;
}

CernColor const *
cern_color_slate_blue(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_SlateBlue].color;
}

CernColor const *
cern_color_slate_gray(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_SlateGray].color;
}

CernColor const *
cern_color_snow(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_Snow].color;
}

CernColor const *
cern_color_spring_green(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_SpringGreen].color;
}

CernColor const *
cern_color_steel_blue(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_SteelBlue].color;
}

CernColor const *
cern_color_tan(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_SaddleBrown].color;
}

CernColor const *
cern_color_teal(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_Teal].color;
}

CernColor const *
cern_color_thistle(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_Thistle].color;
}

CernColor const *
cern_color_tomato(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_Tomato].color;
}

CernColor const *
cern_color_turquoise(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_Turquoise].color;
}

CernColor const *
cern_color_violet(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_Violet].color;
}

CernColor const *
cern_color_wheat(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_Wheat].color;
}

CernColor const *
cern_color_white(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_White].color;
}

CernColor const *
cern_color_white_smoke(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_WhiteSmoke].color;
}

CernColor const *
cern_color_yellow(void) {
  cern_color_table_ensure_created();
  return knwon_color_table[CernKnownColor_Yellow].color;
}

CernColor const *
cern_color_yellow_green(void) {
  cern_color_table_ensure_created();

  return knwon_color_table[CernKnownColor_YellowGreen].color;
}
