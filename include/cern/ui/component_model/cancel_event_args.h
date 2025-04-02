#ifndef CERN_UI_COMPONENT_MODEL_CANCEL_EVENT_ARGS_H
#define CERN_UI_COMPONENT_MODEL_CANCEL_EVENT_ARGS_H 1

#include <glib-object.h>
#include "event_args.h"

G_BEGIN_DECLS

#define CERN_TYPE_CANCEL_EVENT_ARGS (cern_cancel_event_args_get_type())

G_DECLARE_DERIVABLE_TYPE(CernCancelEventArgs, cern_cancel_event_args,
                         CERN, CANCEL_EVENT_ARGS, CernEventArgs);

struct _CernCancelEventArgsClass {
  GObjectClass parent_class;

  gpointer reserved[5];
};

CernCancelEventArgs *
cern_cancel_event_args_new(void);

CernCancelEventArgs *
cern_cancel_event_args_new_with_value(gboolean cancel);

gboolean
cern_cancel_event_args_get_cancel(CernCancelEventArgs *self);

void
cern_cancel_event_args_unref(CernCancelEventArgs *self);

G_END_DECLS

#endif /* CERN_UI_COMPONENT_MODEL_CANCEL_EVENT_ARGS_H */
