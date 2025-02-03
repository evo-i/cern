#include "cern/ui/component_model/icontainer.h"
#include "cern/ui/component_model/icomponent.h"

G_DEFINE_INTERFACE(ICernComponentModelContainer, i_cern_component_model_container, G_TYPE_OBJECT);

void
static
i_cern_component_model_container_default_init(ICernComponentModelContainerInterface *iface) {
  // Add default initialization code here
}

void
i_cern_component_model_container_add(ICernComponentModelContainer *self,
                                     ICernComponentModelComponent *component) {
  ICernComponentModelContainerInterface *iface;

  g_return_if_fail(I_CERN_IS_COMPONENT_MODEL_CONTAINER(self));
  g_return_if_fail(I_CERN_IS_COMPONENT_MODEL_COMPONENT(component));

  iface = I_CERN_COMPONENT_MODEL_CONTAINER_GET_IFACE(self);

  g_return_if_fail(iface->add != NULL);

  (iface->add)(self, component);
}

void
i_cern_component_model_container_add_named(ICernComponentModelContainer *self,
                                           ICernComponentModelComponent *component,
                                           gchar const *name) {
  ICernComponentModelContainerInterface *iface;

  g_return_if_fail(I_CERN_IS_COMPONENT_MODEL_CONTAINER(self));
  g_return_if_fail(I_CERN_IS_COMPONENT_MODEL_COMPONENT(component));
  g_warn_if_fail(name != NULL);
  g_return_if_fail(NULL != name);

  iface = I_CERN_COMPONENT_MODEL_CONTAINER_GET_IFACE(self);

  g_return_if_fail(iface->add_named != NULL);

  (iface->add_named)(self, component, name);
}

GList *
i_cern_component_model_container_get_components(ICernComponentModelContainer *self) {
  ICernComponentModelContainerInterface *iface;

  g_return_val_if_fail(I_CERN_IS_COMPONENT_MODEL_CONTAINER(self), NULL);

  iface = I_CERN_COMPONENT_MODEL_CONTAINER_GET_IFACE(self);

  g_return_val_if_fail(iface->get_components != NULL, NULL);

  return (iface->get_components)(self);
}

void
i_cern_component_model_container_remove(ICernComponentModelContainer *self,
                                        ICernComponentModelComponent *component) {
  ICernComponentModelContainerInterface *iface;

  g_return_if_fail(I_CERN_IS_COMPONENT_MODEL_CONTAINER(self));
  g_return_if_fail(I_CERN_IS_COMPONENT_MODEL_COMPONENT(component));

  iface = I_CERN_COMPONENT_MODEL_CONTAINER_GET_IFACE(self);

  g_return_if_fail(iface->remove != NULL);

  (iface->remove)(self, component);
}
