#ifndef CERN_COLOR_TRANSLATOR_H
#define CERN_COLOR_TRANSLATOR_H 1

#include <glib-object.h>

G_BEGIN_DECLS

typedef struct _CernColor CernColor;

gint32
cern_color_translator_to_win32(CernColor *self);

CernColor
cern_color_translator_from_win32(gint32 argb);

gint32
cern_color_translator_to_ole(CernColor *self);

CernColor
cern_color_translator_from_ole(gint32 ole);

G_END_DECLS

#endif /* CERN_COLOR_TRANSLATOR_H */
