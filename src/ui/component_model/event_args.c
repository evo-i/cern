#include "cern/ui/component_model.h"

struct _CernEventArgs {
  GObject parent_instance;
};

CernEventArgs *
cern_event_args_new(void) {
  return g_object_new(CERN_TYPE_EVENT_ARGS, NULL);
}
