#include "cern/ui/control_tab_order_holder.h"
#include "cern/ui/control.h"

struct _CernControlTabOrderHolder {
  GObject parent_instance;

  gint32 old_order;
  gint32 new_order;
  CernControl *control;
};

G_DEFINE_FINAL_TYPE(CernControlTabOrderHolder, cern_control_tab_order_hodler, G_TYPE_OBJECT);

static
void
cern_control_tab_order_holder_dispose(GObject *obj) {
  CernControlTabOrderHolder *self
    = CERN_TYPE_CONTROL_TAB_ORDER_HOLDER(obj);

  g_clear_object(&self->control);

  G_OBJECT_CLASS(cern_control_tab_order_holder_parent_class)->dispose(obj);
}

static
void
cern_control_tab_order_holder_init(CernControlTabOrderHolder *self) { }

static
void
cern_control_tab_order_holder_class_init(CernControlTabOrderHolderClass *klass) {
  GObjectClass *g_class = G_OBECT_CLASS(klass);

  g_class->dispose = cern_control_tab_order_holder_dispose;
}

CernControlTabOrderHolder *
cern_control_tab_order_holder_new(gint32 old_order, gint32 new_order, CernControl *control) {
  CernControlTabOrderHolder *self
    = g_object_new(CERN_TYPE_CONTROL_TAB_ORDER_HOLDER, NULL);

  self->old_order = old_order;
  self->new_order = new_order;
  self->control = g_object_ref(control);

  return self;
}

gint32
cern_control_tab_order_holder_get_old_order(CernControlTabOrderHolder *self) {
  return self->old_order;
}

gint32
cern_control_tab_order_holder_get_new_order(CernControlTabOrderHolder *self) {
  return self->new_order;
}

CernControlTabOrderHolder *
cern_control_tab_order_holder_get_control(CernControlTabOrderHolder *self) {
  return self->control;
}

