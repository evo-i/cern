#include "cern/ui/component_model/cancel_event_args.h"
#include <glib-object.h>
#include <glib.h>
#include <glibconfig.h>

typedef struct _CernCancelEventArgsPrivate {
  gboolean value;
} CernCancelEventArgsPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(CernCancelEventArgs,
                           cern_cancel_event_args,
                           CERN_TYPE_CANCEL_EVENT_ARGS);

CernCancelEventArgs *
cern_cancel_event_args_new(void) {
  return
    cern_cancel_event_args_new_with_value(FALSE);
}

CernCancelEventArgs *
cern_cancel_event_args_new_with_value(gboolean value) {
  CernCancelEventArgs *self
    = g_object_new(CERN_TYPE_CANCEL_EVENT_ARGS, NULL);

  CernCancelEventArgsPrivate *priv
    = cern_cancel_event_args_get_instance_private(self);

  priv->value = value;

  return self;
}

gboolean
cern_cancel_event_args_get_cancel(CernCancelEventArgs *self) {
  CernCancelEventArgsPrivate *priv
    = cern_cancel_event_args_get_instance_private(self);

  return priv->value;
}

void
cern_cancel_event_args_set_cancel(CernCancelEventArgs *self, gboolean value) {
  CernCancelEventArgsPrivate *priv
    = cern_cancel_event_args_get_instance_private(self);

  priv->value = value;
}

void
cern_cancel_event_args_unref(CernCancelEventArgs *self) {
  g_object_unref(self);
}

