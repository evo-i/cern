#ifndef CERN_UI_CONTROL_TAB_ORDER_HOLDER_H
#define CERN_UI_CONTROL_TAB_ORDER_HOLDER_H 1

#include <glib-object.h>
#include "control.h"

G_BEGIN_DECLS

#define CERN_TYPE_CONTROL_TAB_ORDER_HOLDER cern_control_tab_order_holder_get_type()

G_DECLARE_FINAL_TYPE(CernControlTabOrderHolder,
                     cern_control_tab_order_holder,
                     CERN, CONTROL_TAB_ORDER_HOLDER, GObject);

CernControlTabOrderHolder *
cern_control_tab_order_holder_new(gint32 old_order, gint32 new_order, CernControl *ctl);

gint32
cern_control_tab_order_holder_get_old_order(CernControlTabOrderHolder *self);

gint32
cern_control_tab_order_holder_get_new_order(CernControlTabOrderHolder *self);

CernControl *
cern_control_tab_order_holder_get_control(CernControlTabOrderHolder *self);

G_END_DECLS

#endif /* CERN_UI_CONTROL_TAB_ORDER_HOLDER_H */
