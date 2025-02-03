#ifndef CERN_UI_COMPONENT_MODEL_COMPONENT_H
#define CERN_UI_COMPONENT_MODEL_COMPONENT_H 1

#include <glib-object.h>
#include "icomponent.h"
#include "icontainer.h"

G_BEGIN_DECLS

#define CERN_TYPE_COMPONENT_MODEL_COMPONENT \
cern_component_model_component_get_type ()

G_DECLARE_DERIVABLE_TYPE(CernComponentModelComponent,
                         cern_component_model_component, CERN,
                         COMPONENT_MODEL_COMPONENT, GObject);

struct _CernComponentModelComponentClass {
  GObjectClass parent_class;

  gboolean
  (*can_raise_events)(CernComponentModelComponent *self);

  ICernComponentModelSite *
  (*get_site)(CernComponentModelComponent *self);

  void
  (*set_site)(CernComponentModelComponent *self, ICernComponentModelSite *site);

  gpointer
  (*get_service)(CernComponentModelComponent *self, GType service);

  ICernComponentModelContainer *
  (*get_container)(CernComponentModelComponent *self);
};

CernComponentModelComponent *
cern_component_model_component_new(void);

gboolean
cern_component_model_component_can_raise_events(CernComponentModelComponent *self);

ICernComponentModelSite *
cern_component_model_component_get_site(CernComponentModelComponent *self);

void
cern_component_model_component_set_site(CernComponentModelComponent *self,
                                        ICernComponentModelSite *site);

gpointer
cern_component_model_component_get_service(CernComponentModelComponent *self,
                                           GType service);

ICernComponentModelContainer *
cern_component_model_component_get_container(CernComponentModelComponent *self);

G_END_DECLS

#endif /* CERN_UI_COMPONENT_MODEL_COMPONENT_H */
