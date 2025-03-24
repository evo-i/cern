#ifndef CERN_UI_COMPONENT_MODEL_LAYOUT_EVENT_ARGS_H
#define CERN_UI_COMPONENT_MODEL_LAYOUT_EVENT_ARGS_H 1

#include <glib-object.h>
#include "event_args.h"
#include "icomponent.h"

G_BEGIN_DECLS

typedef struct _CernControl CernControl;

#define CERN_LAYOUT_EVENT_ARGS_TYPE (cern_layout_event_args_get_type())

G_DECLARE_FINAL_TYPE(CernLayoutEventArgs, cern_layout_event_args,
                     CERN, LAYOUT_EVENT_ARGS, CernEventArgs);

CernLayoutEventArgs *
cern_layout_event_args_new(ICernComponentModelComponent *affected_component,
                           gchar *affected_property);

CernLayoutEventArgs *
cern_layout_event_args_new_control(CernControl *affected_control,
                                   gchar *affected_property);

ICernComponentModelComponent *
cern_layout_event_args_get_affected_component(CernLayoutEventArgs *self);

CernControl *
cern_layout_event_args_get_affected_control(CernLayoutEventArgs *self);

gchar *
cern_layout_event_args_get_affected_property(CernLayoutEventArgs *self);

G_END_DECLS

#endif /* CERN_UI_COMPONENT_MODEL_LAYOUT_EVENT_ARGS_H */
