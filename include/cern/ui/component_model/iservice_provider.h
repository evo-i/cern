#ifndef CERN_UI_COMPONENT_MODEL_I_SERVICE_PROVIDER
#define CERN_UI_COMPONENT_MODEL_I_SERVICE_PROVIDER 1

#include <glib-object.h>

G_BEGIN_DECLS

#define CERN_TYPE_I_COMPONENT_MODEL_SERVICE_PROVIDER \
i_cern_component_model_service_provider_get_type ()

G_DECLARE_INTERFACE(ICernComponentModelServiceProvider,
                    i_cern_component_model_service_provider,
                    I_CERN, COMPONENT_MODEL_SERVICE_PROVIDER, GObject);

struct _ICernComponentModelServiceProviderInterface {
  GTypeInterface parent_iface;

  gpointer
  (*get_service)(ICernComponentModelServiceProvider *self, GType type);
};

gpointer
i_cern_component_model_service_provider_get_service(ICernComponentModelServiceProvider *self, GType type);

G_END_DECLS

#endif /* CERN_UI_COMPONENT_MODEL_I_SERVICE_PROVIDER */
