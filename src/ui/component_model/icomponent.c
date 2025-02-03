#include "cern/ui/component_model/icomponent.h"

G_DEFINE_INTERFACE(ICernComponentModelComponent, i_cern_component_model_component, G_TYPE_OBJECT);

static
void
i_cern_component_model_component_default_init(ICernComponentModelComponentInterface *iface) {
  /* Add default initialization code here if needed */
}

ICernComponentModelSite *
i_cern_component_model_component_get_site(ICernComponentModelComponent *self) {
  ICernComponentModelComponentInterface *iface;

  g_return_val_if_fail(I_CERN_IS_COMPONENT_MODEL_COMPONENT(self), NULL);

  iface = I_CERN_COMPONENT_MODEL_COMPONENT_GET_IFACE(self);

  g_return_val_if_fail(iface->get_site != NULL, NULL);

  return (iface->get_site)(self);
}

void
i_cern_component_model_component_set_site(ICernComponentModelComponent *self,
                                          ICernComponentModelSite *site){
  ICernComponentModelComponentInterface *iface;

  g_warn_if_fail(I_CERN_IS_COMPONENT_MODEL_COMPONENT(self));
  g_return_if_fail(I_CERN_IS_COMPONENT_MODEL_COMPONENT(self));

  iface = I_CERN_COMPONENT_MODEL_COMPONENT_GET_IFACE(self);

  g_warn_if_fail(iface->set_site != NULL);
  g_return_if_fail(iface->set_site != NULL);

  return (iface->set_site)(self, site);
}
