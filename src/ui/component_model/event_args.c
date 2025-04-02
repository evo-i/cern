#include "cern/ui/component_model/event_args.h"

CernEventArgs *
cern_event_args_new(void) {
  return g_object_new(CERN_TYPE_EVENT_ARGS, NULL);
}
