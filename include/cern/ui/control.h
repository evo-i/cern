#ifndef CERN_UI_CONTROL_H
#define CERN_UI_CONTROL_H 1

#include <glib-object.h>

G_BEGIN_DECLS

#include "cern/ui/component_model/component.h"

#define CERN_TYPE_CONTROL cern_control_get_type ()

G_DECLARE_DERIVABLE_TYPE(CernControl, cern_control, CERN, CONTROL, CernComponentModelComponent);

struct _CernControlClass {
  CernComponentModelComponentClass parent_class;
};

G_END_DECLS

#endif /* CERN_UI_CONTROL_H */
