#include "cern/ui/component_model/isite.h"

G_DEFINE_INTERFACE(ICernComponentModelSite, i_cern_component_model_site, G_TYPE_OBJECT);

void
static
i_cern_component_model_site_default_init(ICernComponentModelSiteInterface *iface) {
  // Add default initialization code here if needed
}

ICernComponentModelComponent *
i_cern_component_model_site_get_component(ICernComponentModelSite *self) {
  ICernComponentModelSiteInterface *iface;

  g_return_val_if_fail(I_CERN_IS_COMPONENT_MODEL_SITE(self), NULL);

  iface = I_CERN_COMPONENT_MODEL_SITE_GET_IFACE(self);

  g_return_val_if_fail(iface->get_component != NULL, NULL);

  return (iface->get_component)(self);
}

ICernComponentModelContainer *
i_cern_component_model_site_get_container(ICernComponentModelSite *self) {
  ICernComponentModelSiteInterface *iface;

  g_return_val_if_fail(I_CERN_IS_COMPONENT_MODEL_SITE(self), NULL);

  iface = I_CERN_COMPONENT_MODEL_SITE_GET_IFACE(self);

  g_return_val_if_fail(iface->get_container != NULL, NULL);

  return (iface->get_container)(self);
}

gboolean
i_cern_component_model_site_get_design_mode(ICernComponentModelSite *self) {
  ICernComponentModelSiteInterface *iface;

  g_return_val_if_fail(I_CERN_IS_COMPONENT_MODEL_SITE(self), FALSE);

  iface = I_CERN_COMPONENT_MODEL_SITE_GET_IFACE(self);

  g_return_val_if_fail(iface->get_gesign_mode != NULL, FALSE);

  return (iface->get_gesign_mode)(self);
}

gchar const *
i_cern_component_model_site_get_name(ICernComponentModelSite *self) {
  ICernComponentModelSiteInterface *iface;

  g_return_val_if_fail(I_CERN_IS_COMPONENT_MODEL_SITE(self), NULL);

  iface = I_CERN_COMPONENT_MODEL_SITE_GET_IFACE(self);

  g_return_val_if_fail(iface->get_name != NULL, NULL);

  return (iface->get_name)(self);
}

void
i_cern_component_model_site_set_name(ICernComponentModelSite *self,
                                     gchar const *name) {
  ICernComponentModelSiteInterface *iface;

  g_return_if_fail(I_CERN_IS_COMPONENT_MODEL_SITE(self));

  iface = I_CERN_COMPONENT_MODEL_SITE_GET_IFACE(self);

  g_return_if_fail(iface->set_name != NULL);

  (iface->set_name)(self, name);
}
