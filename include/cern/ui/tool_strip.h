#ifndef CERN_UI_TOOL_STRIP_H
#define CERN_UI_TOOL_STRIP_H 1

#include <glib-object.h>
#include "scrollable_control.h"

G_BEGIN_DECLS

#define CERN_TYPE_TOOL_STRIP cern_tool_strip_get_type ()

G_DECLARE_DERIVABLE_TYPE(CernToolStrip, cern_tool_strip, CERN, TOOL_STRIP, CernScrollableControl);

G_END_DECLS

#endif /* CERN_UI_TOOL_STRIP_H */
