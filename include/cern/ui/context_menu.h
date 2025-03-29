#ifndef CERN_UI_CONTEXT_MENU_H
#define CERN_UI_CONTEXT_MENU_H 1

#include <glib-object.h>
#include "menu.h"

G_BEGIN_DECLS

#define CERN_TYPE_CONTEXT_MENU cern_context_menu_get_type ()

G_DECLARE_DERIVABLE_TYPE(CernContextMenu, cern_context_menu, CERN, CONTEXT_MENU, CernMenu);

G_END_DECLS

#endif /* CERN_UI_CONTEXT_MENU_H */
