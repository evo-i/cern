#ifndef CERN_UI_COMPONENT_MODEL_CANCEL_EVENT_ARGS_H
#define CERN_UI_COMPONENT_MODEL_CANCEL_EVENT_ARGS_H 1

#include <glib-object.h>

G_BEGIN_DECLS

#define CERN_CANCEL_EVENT_ARGS_TYPE (cern_cancel_event_args_get_type())

G_DECLARE_DERIVABLE_TYPE(CernCancelEventArgs, cern_cancel_event_args, CERN, CANCEL_EVENT_ARGS, GObject)

G_END_DECLS

#endif /* CERN_UI_COMPONENT_MODEL_CANCEL_EVENT_ARGS_H */
