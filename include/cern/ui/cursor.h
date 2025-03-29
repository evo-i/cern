#ifndef CERN_UI_CURSOR_H
#define CERN_UI_CURSOR_H 1

#include <glib-object.h>
#include <gio/gio.h>

#include "cern/drawing/graphics.h"
#include "cern/drawing/rectangle.h"
#include "cern/drawing/point.h"
#include "cern/drawing/size.h"

G_BEGIN_DECLS

#define CERN_TYPE_CURSOR cern_cursor_get_type()

G_DECLARE_FINAL_TYPE(CernCursor, cern_cursor, CERN, CURSOR, GObject);

CernCursor *
cern_cursor_new(gpointer handle);

CernCursor *
cern_cursor_new_from_file_name(gchar const *file_name);

CernCursor *
cern_cursor_new_from_stream(GInputStream *stream);

CernRectangle
cern_cursor_rectangle_get_clip(void);

void
cern_cursor_rectangle_set_clip(CernRectangle *value);

CernCursor *
cern_cursor_get_current(void);

gpointer
cern_cursor_get_handle(CernCursor *self);

CernPoint
cern_cursor_get_hot_spot(CernCursor *self);

CernPoint
cern_cursor_get_position(void);

void
cern_cursor_set_position(CernPoint value);

CernSize
cern_cursor_get_size(CernCursor *self);

gpointer
cern_cursor_set_tag(CernCursor *self, gpointer value);

gpointer
cern_cursor_get_tag(CernCursor *self);

gpointer
cern_cursor_copy_handle(CernCursor *self);

void
cern_cursor_draw(CernCursor *self, CernGraphics *graphics,
                 CernRectangle *target_rect);

void
cern_cursor_draw_stretched(CernCursor *self, CernGraphics *graphics,
                           CernRectangle *target_rect);

void
cern_cursor_hide(void);

void
cern_cursor_show(void);

gboolean
cern_cursor_equals(CernCursor *self, CernCursor *other);

G_END_DECLS

#endif /* CERN_UI_CURSOR_H */
