#ifndef CERN_AMBIENT_PROPERTIES_H
#define CERN_AMBIENT_PROPERTIES_H 1

#include <glib-object.h>
#include "cern/drawing/color.h"
#include "cern/drawing/font.h"
#include "cursor.h"

G_BEGIN_DECLS

#define CERN_TYPE_AMBIENT_PROPERTIES cern_ambient_properties_get_type ()

G_DECLARE_FINAL_TYPE(CernAmbientProperties, cern_ambient_properties, CERN, AMBIENT_PROPERTIES, GObject);

CernAmbientProperties *
cern_ambient_properties_new(void);

CernFont *
cern_ambient_properties_get_font(CernAmbientProperties *self);

void
cern_ambient_properties_set_font(CernAmbientProperties *self, CernFont *value);

CernCursor *
cern_ambient_properties_get_cursor(CernAmbientProperties *self);

void
cern_ambient_properties_set_cursor(CernAmbientProperties *self, CernCursor *value);

CernColor
cern_ambient_properties_get_back_color(CernAmbientProperties *self);

void
cern_ambient_properties_set_back_color(CernAmbientProperties *self, CernColor *value);

CernColor
cern_ambient_properties_get_fore_color(CernAmbientProperties *self);

void
cern_ambient_properties_set_fore_color(CernAmbientProperties *self, CernColor *value);

G_END_DECLS

#endif /* CERN_AMBIENT_PROPERTIES_H */
