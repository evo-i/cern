#ifndef CERN_UI_SCROLLABLE_CONTROL_H
#define CERN_UI_SCROLLABLE_CONTROL_H 1

#include <glib-object.h>
#include "control.h"

G_BEGIN_DECLS

#define CERN_TYPE_SCROLLABLE_CONTROL cern_scrollable_control_get_type ()

G_DECLARE_DERIVABLE_TYPE(CernScrollableControl, cern_scrollable_control, CERN, SCROLLABLE_CONTROL, CernControl);

G_END_DECLS

#endif /* CERN_UI_SCROLLABLE_CONTROL_H */
