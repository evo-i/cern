#ifndef CERN_UI_COMPONENT_MODEL_EVENT_ARGS_H
#define CERN_UI_COMPONENT_MODEL_EVENT_ARGS_H 1

#include <glib-object.h>

G_BEGIN_DECLS

#define CERN_EVENT_ARGS_TYPE (cern_event_args_get_type())

G_DECLARE_DERIVABLE_TYPE(CernEventArgs, cern_event_args, CERN, EVENT_ARGS, GObject);

struct _CernEventArgsClass {
  GObjectClass parent_class;
};

CernEventArgs *
cern_event_args_new(void);

G_END_DECLS

#endif /* CERN_UI_COMPONENT_MODEL_EVENT_ARGS_H */
