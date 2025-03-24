#ifndef CERN_UI_COMPONENT_MODEL_CONTROL_EVENT_ARGS_H
#define CERN_UI_COMPONENT_MODEL_CONTROL_EVENT_ARGS_H 1

#include <glib-object.h>
#include "event_args.h"

G_BEGIN_DECLS

#define  CERN_TYPE_CONTROL_EVENT_ARGS (cern_control_event_args_get_type())

G_DECLARE_DERIVABLE_TYPE(CernControlEventArgs, cern_control_event_args, CERN, CONTROL_EVENT_ARGS, CernEventArgs)

struct _CernControlEventArgsClass {
  CernEventArgsClass parent_class;
};

typedef struct _CernControl CernControl;

CernControlEventArgs *
cern_control_event_args_new(CernControl *control);

CernControl *
cern_control_event_args_get_control(CernControlEventArgs *args);

void
cern_control_event_args_unref(CernControlEventArgs *args);

G_END_DECLS

#endif /* CERN_UI_COMPONENT_MODEL_CONTROL_EVENT_ARGS_H */
