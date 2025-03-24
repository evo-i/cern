#ifndef CERN_UI_TOOL_STRIP_DROP_DOWN_H
#define CERN_UI_TOOL_STRIP_DROP_DOWN_H 1

#include <glib-object.h>
#include "tool_strip.h"

G_BEGIN_DECLS

#define CERN_TYPE_TOOL_STRIP_DROP_DOWN cern_tool_strip_drop_down_get_type ()

G_DECLARE_DERIVABLE_TYPE(CernToolStripDropDown, cern_tool_strip_drop_down, CERN, TOOL_STRIP_DROP_DOWN, CernToolStrip);

G_END_DECLS

#endif /* CERN_UI_TOOL_STRIP_DROP_DOWN_H */
