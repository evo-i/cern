#ifndef CERN_UI_MENU_H
#define CERN_UI_MENU_H 1

#include <glib-object.h>
#include "component_model/component.h"

G_BEGIN_DECLS

#define CERN_TYPE_MENU cern_menu_get_type ()

G_DECLARE_DERIVABLE_TYPE(CernMenu, cern_menu, CERN, MENU, CernComponentModelComponent);

G_END_DECLS

#endif /* CERN_UI_MENU_H */
