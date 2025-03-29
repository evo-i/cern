#ifndef CERN_UI_CONTEXT_MENU_STRIP_H
#define CERN_UI_CONTEXT_MENU_STRIP_H 1

#include <glib-object.h>

#include "tool_strip_drop_down.h"

G_BEGIN_DECLS

#define CERN_TYPE_CONTEXT_MENU_STRIP cern_context_menu_strip_get_type ()

G_DECLARE_DERIVABLE_TYPE(CernContextMenuStrip, cern_context_menu_strip, CERN, CONTEXT_MENU_STRIP, CernToolStripDropDown);

G_END_DECLS

#endif /* CERN_UI_CONTEXT_MENU_STRIP_H */
