#ifndef PAINT_EVENT_ARGS
#define PAINT_EVENT_ARGS

#include <glib-object.h>

#include "event_args.h"
#include "cern/drawing/graphics.h"
#include "cern/drawing/rectangle.h"

G_BEGIN_DECLS

#define CERN_TYPE_PAINT_EVENT_ARGS (cern_paint_event_args_get_type())

G_DECLARE_DERIVABLE_TYPE(CernPaintEventArgs, cern_paint_event_args, CERN, PAINT_EVENT_ARGS, CernEventArgs)

CernPaintEventArgs *
cern_paint_event_args_new(CernGraphics *graphics, CernRectangle *clip);

CernPaintEventArgs *
cern_paint_event_args_new_internal(gpointer dc, CernRectangle *clip);

CernGraphics *
cern_paint_event_args_get_graphics(CernPaintEventArgs *args);

CernRectangle
cern_paint_event_args_get_clip_rectangle(CernPaintEventArgs *args);

gpointer
cern_paint_event_args_get_hdc_internal(CernPaintEventArgs *args);

void
cern_piant_event_args_reset_graphics(CernPaintEventArgs *args);

G_END_DECLS

#endif /* PAINT_EVENT_ARGS */
