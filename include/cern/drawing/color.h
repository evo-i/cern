#ifndef CERN_COLOR_H
#define CERN_COLOR_H 1

#include <glib-object.h>
#include "known_color.h"

G_BEGIN_DECLS

#define CERN_TYPE_COLOR (cern_color_get_type())

typedef struct _CernColor {
  struct _CernColorName {
    gchar *value;
    gboolean freedable;
  } name;
  gint64 value;
  gint16 known_color;
  gint16 state;
} CernColor;

GType
cern_color_get_type(void);

CernColor *
cern_color_new(void);

CernColor *
cern_color_copy(CernColor *self);

void
cern_color_free(CernColor *self);

CernColor
cern_color_from_argb(guint32 argb);

void
cern_color_init_argb(CernColor *self, guint32 argb);

CernColor
cern_color_from_argb_uint8(guint8 a, guint8 r, guint8 g, guint8 b);

CernColor
cern_color_from_argb_other(guint8 alpha, CernColor *other);

CernColor
cern_color_from_known_color(CernKnownColor known_color);

CernColor
cern_color_from_name(gchar const *name);

guint8
cern_color_get_alpha(CernColor *self);

guint8
cern_color_get_red(CernColor *self);

guint8
cern_color_get_green(CernColor *self);

guint8
cern_color_get_blue(CernColor *self);

gchar const *
cern_color_get_name(CernColor *self);

gboolean
cern_color_is_known(CernColor *self);

gboolean
cern_color_is_empty(CernColor *self);

gboolean
cern_color_is_named(CernColor *self);

gboolean
cern_color_is_system(CernColor *self);

float
cern_color_get_brightness(CernColor *self);

float
cern_color_get_hue(CernColor *self);

float
cern_color_get_saturation(CernColor *self);

gboolean
cern_color_equals(CernColor *self, CernColor *other);

gint32
cern_color_to_argb(CernColor *self);

CernKnownColor
cern_color_to_known_color(CernColor *self);

CernColor
cern_color_empty(void);

CernColor
cern_color_transparent(void);

CernColor
cern_color_alice_blue(void);

CernColor
cern_color_antique_white(void);

CernColor
cern_color_aqua(void);

CernColor
cern_color_aquamarine(void);

CernColor
cern_color_azure(void);

CernColor
cern_color_beige(void);

CernColor
cern_color_bisque(void);

CernColor
cern_color_black(void);

CernColor
cern_color_blanched_almond(void);

CernColor
cern_color_blue(void);

CernColor
cern_color_blue_violet(void);

CernColor
cern_color_brown(void);

CernColor
cern_color_burly_wood(void);

CernColor
cern_color_cadet_blue(void);

CernColor
cern_color_chartreuse(void);

CernColor
cern_color_chocolate(void);

CernColor
cern_color_coral(void);

CernColor
cern_color_cornflower_blue(void);

CernColor
cern_color_cornsilk(void);

CernColor
cern_color_crimson(void);

CernColor
cern_color_cyan(void);

CernColor
cern_color_dark_blue(void);

CernColor
cern_color_dark_cyan(void);

CernColor
cern_color_dark_goldenrod(void);

CernColor
cern_color_dark_gray(void);

CernColor
cern_color_dark_green(void);

CernColor
cern_color_dark_khaki(void);

CernColor
cern_color_dark_magenta(void);

CernColor
cern_color_dark_olive_green(void);

CernColor
cern_color_dark_orange(void);

CernColor
cern_color_dark_orchid(void);

CernColor
cern_color_dark_red(void);

CernColor
cern_color_dark_salmon(void);

CernColor
cern_color_dark_sea_green(void);

CernColor
cern_color_dark_slate_blue(void);

CernColor
cern_color_dark_slate_gray(void);

CernColor
cern_color_dark_turquoise(void);

CernColor
cern_color_dark_violet(void);

CernColor
cern_color_deep_pink(void);

CernColor
cern_color_deep_sky_blue(void);

CernColor
cern_color_dim_gray(void);

CernColor
cern_color_dodger_blue(void);

CernColor
cern_color_firebrick(void);

CernColor
cern_color_floral_white(void);

CernColor
cern_color_forest_green(void);

CernColor
cern_color_fuchsia(void);

CernColor
cern_color_gainsboro(void);

CernColor
cern_color_ghost_white(void);

CernColor
cern_color_gold(void);

CernColor
cern_color_goldenrod(void);

CernColor
cern_color_gray(void);

CernColor
cern_color_green(void);

CernColor
cern_color_green_yellow(void);

CernColor
cern_color_honeydew(void);

CernColor
cern_color_hot_pink(void);

CernColor
cern_color_indian_red(void);

CernColor
cern_color_indigo(void);

CernColor
cern_color_ivory(void);

CernColor
cern_color_khaki(void);

CernColor
cern_color_lavender(void);

CernColor
cern_color_lavender_blush(void);

CernColor
cern_color_lawn_green(void);

CernColor
cern_color_lemon_chiffon(void);

CernColor
cern_color_light_blue(void);

CernColor
cern_color_light_coral(void);

CernColor
cern_color_light_cyan(void);

CernColor
cern_color_light_goldenrod_yellow(void);

CernColor
cern_color_light_green(void);

CernColor
cern_color_light_gray(void);

CernColor
cern_color_light_pink(void);

CernColor
cern_color_light_salmon(void);

CernColor
cern_color_light_sea_green(void);

CernColor
cern_color_light_sky_blue(void);

CernColor
cern_color_light_slate_gray(void);

CernColor
cern_color_light_steel_blue(void);

CernColor
cern_color_light_yellow(void);

CernColor
cern_color_lime(void);

CernColor
cern_color_lime_green(void);

CernColor
cern_color_linen(void);

CernColor
cern_color_magenta(void);

CernColor
cern_color_maroon(void);

CernColor
cern_color_medium_aquamarine(void);

CernColor
cern_color_medium_blue(void);

CernColor
cern_color_medium_orchid(void);

CernColor
cern_color_medium_purple(void);

CernColor
cern_color_medium_sea_green(void);

CernColor
cern_color_medium_slate_blue(void);

CernColor
cern_color_medium_spring_green(void);

CernColor
cern_color_medium_turquoise(void);

CernColor
cern_color_medium_violet_red(void);

CernColor
cern_color_midnight_blue(void);

CernColor
cern_color_mint_cream(void);

CernColor
cern_color_misty_rose(void);

CernColor
cern_color_moccasin(void);

CernColor
cern_color_navajo_white(void);

CernColor
cern_color_navy(void);

CernColor
cern_color_old_lace(void);

CernColor
cern_color_olive(void);

CernColor
cern_color_olive_drab(void);

CernColor
cern_color_orange(void);

CernColor
cern_color_orange_red(void);

CernColor
cern_color_orchid(void);

CernColor
cern_color_pale_goldenrod(void);

CernColor
cern_color_pale_green(void);

CernColor
cern_color_pale_turquoise(void);

CernColor
cern_color_pale_violet_red(void);

CernColor
cern_color_papaya_whip(void);

CernColor
cern_color_peach_puff(void);

CernColor
cern_color_peru(void);

CernColor
cern_color_pink(void);

CernColor
cern_color_plum(void);

CernColor
cern_color_powder_blue(void);

CernColor
cern_color_purple(void);

CernColor
cern_color_red(void);

CernColor
cern_color_rosy_brown(void);

CernColor
cern_color_royal_blue(void);

CernColor
cern_color_saddle_brown(void);

CernColor
cern_color_salmon(void);

CernColor
cern_color_sandy_brown(void);

CernColor
cern_color_sea_green(void);

CernColor
cern_color_sea_shell(void);

CernColor
cern_color_sienna(void);

CernColor
cern_color_silver(void);

CernColor
cern_color_sky_blue(void);

CernColor
cern_color_slate_blue(void);

CernColor
cern_color_slate_gray(void);

CernColor
cern_color_snow(void);

CernColor
cern_color_spring_green(void);

CernColor
cern_color_steel_blue(void);

CernColor
cern_color_tan(void);

CernColor
cern_color_teal(void);

CernColor
cern_color_thistle(void);

CernColor
cern_color_tomato(void);

CernColor
cern_color_turquoise(void);

CernColor
cern_color_violet(void);

CernColor
cern_color_wheat(void);

CernColor
cern_color_white(void);

CernColor
cern_color_white_smoke(void);

CernColor
cern_color_yellow(void);

CernColor
cern_color_yellow_green(void);

G_END_DECLS

#endif /* CERN_COLOR_H */
