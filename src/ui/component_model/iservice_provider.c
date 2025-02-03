#include "cern/ui/component_model/iservice_provider.h"

G_DEFINE_INTERFACE(ICernComponentModelServiceProvider, i_cern_component_model_service_provider, G_TYPE_OBJECT)

void
static
i_cern_component_model_service_provider_default_init(ICernComponentModelServiceProviderInterface *iface) {
  /* Default initialization code here */
}

gpointer
i_cern_component_model_service_provider_get_service(ICernComponentModelServiceProvider *self, GType type) {
  ICernComponentModelServiceProviderInterface *iface;

  g_return_val_if_fail(I_CERN_IS_COMPONENT_MODEL_SERVICE_PROVIDER(self), NULL);

  iface = I_CERN_COMPONENT_MODEL_SERVICE_PROVIDER_GET_IFACE(self);

  g_return_val_if_fail(iface->get_service != NULL, NULL);

  return (iface->get_service)(self, type);
}
