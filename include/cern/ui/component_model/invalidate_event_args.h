#ifndef CERN_UI_COMPONENT_MODEL_INVALIDATE_EVENT_ARGS_H
#define CERN_UI_COMPONENT_MODEL_INVALIDATE_EVENT_ARGS_H 1

#include <glib-object.h>
#include "event_args.h"
#include "cern/drawing/rectangle.h"

G_BEGIN_DECLS

#define CERN_TYPE_INVALIDATE_EVENT_ARGS (cern_invalidate_event_args_get_type())

G_DECLARE_DERIVABLE_TYPE(CernInvalidateEventArgs, cern_invalidate_event_args, CERN, INVALIDATE_EVENT_ARGS, CernEventArgs)

CernInvalidateEventArgs *
cern_invalidate_event_args_new(CernRectangle *rect);

CernRectangle *
cern_invalidate_event_args_get_rect(CernInvalidateEventArgs *args);

void
cern_invalidate_event_args_unref(CernInvalidateEventArgs *args);

G_END_DECLS

#endif /* CERN_UI_COMPONENT_MODEL_INVALIDATE_EVENT_ARGS_H */
