#include "cern/ui/component_model/isite.h"
#include "cern/ui/component_model/component.h"
#include "cern/ui/component_model/icomponent.h"
#include "cern/ui/component_model/iservice_provider.h"

static
inline
gpointer
cern_component_model_component_get_instance_private(CernComponentModelComponent *self);

typedef struct _CernComponentModelComponentPrivate {
  ICernComponentModelSite *site;
} CernComponentModelComponentPrivate;

static
ICernComponentModelSite *
i_cern_component_model_component_get_site_real(ICernComponentModelComponent *self) {
  CernComponentModelComponentPrivate *priv;
  CernComponentModelComponent *inst;

  g_warn_if_fail(CERN_IS_COMPONENT_MODEL_COMPONENT(self));
  g_return_val_if_fail(CERN_IS_COMPONENT_MODEL_COMPONENT(self), NULL);


  inst = CERN_COMPONENT_MODEL_COMPONENT(self);
  priv = cern_component_model_component_get_instance_private(inst);

  return priv->site;
}

static
gpointer
i_cern_component_model_service_provider_get_service_real(ICernComponentModelServiceProvider *self, GType service) {
  return g_object_new(service, NULL);
}

static
void
i_cern_component_model_component_set_site_real(ICernComponentModelComponent *self, ICernComponentModelSite *site) {
  CernComponentModelComponentPrivate *priv;
  CernComponentModelComponent *inst;

  g_warn_if_fail(CERN_IS_COMPONENT_MODEL_COMPONENT(self));
  g_return_if_fail(CERN_IS_COMPONENT_MODEL_COMPONENT(self));

  g_warn_if_fail(I_CERN_IS_COMPONENT_MODEL_SITE(site));
  g_return_if_fail(I_CERN_IS_COMPONENT_MODEL_SITE(site));

  inst = CERN_COMPONENT_MODEL_COMPONENT(self);
  priv = cern_component_model_component_get_instance_private(inst);

  priv->site = site;
}

static
void
cern_component_model_component_i_component_interface_init(ICernComponentModelComponentInterface *iface) {
  iface->get_site = i_cern_component_model_component_get_site_real;
  iface->set_site = i_cern_component_model_component_set_site_real;
}

static
void
cern_component_model_component_i_service_provider_interface_init(ICernComponentModelServiceProviderInterface *iface) {
  iface->get_service = i_cern_component_model_service_provider_get_service_real;
}

G_DEFINE_TYPE_WITH_CODE(CernComponentModelComponent,
  cern_component_model_component,
  G_TYPE_OBJECT,
  G_ADD_PRIVATE(CernComponentModelComponent)
  G_IMPLEMENT_INTERFACE(CERN_TYPE_I_COMPONENT_MODEL_COMPONENT,
    cern_component_model_component_i_component_interface_init)
  G_IMPLEMENT_INTERFACE(CERN_TYPE_I_COMPONENT_MODEL_SERVICE_PROVIDER,
    cern_component_model_component_i_service_provider_interface_init));

static
void
cern_component_model_component_init(CernComponentModelComponent *self) {
  // Initialization code here
}

static
gboolean
cern_component_model_component_can_raise_events_real(CernComponentModelComponent *self) {
  return TRUE;
}

static
ICernComponentModelSite *
cern_component_model_component_get_site_real(CernComponentModelComponent *self) {
  ICernComponentModelComponent *parent;
  ICernComponentModelComponentInterface *iface;

  parent = I_CERN_COMPONENT_MODEL_COMPONENT(self);
  iface = I_CERN_COMPONENT_MODEL_COMPONENT_GET_IFACE(self);

  return (iface->get_site)(parent);
}

static
void
cern_component_model_component_set_site_real(CernComponentModelComponent *self,
                                             ICernComponentModelSite *site) {
  ICernComponentModelComponent *parent;
  ICernComponentModelComponentInterface *iface;

  g_warn_if_fail(I_CERN_COMPONENT_MODEL_SITE(site));
  g_return_if_fail(I_CERN_COMPONENT_MODEL_SITE(site));

  parent = I_CERN_COMPONENT_MODEL_COMPONENT(self);
  iface = I_CERN_COMPONENT_MODEL_COMPONENT_GET_IFACE(self);

  (iface->set_site)(parent, site);
}

static
gpointer
cern_component_model_component_get_service_real(CernComponentModelComponent *self,
                                                GType service) {
  CernComponentModelComponentPrivate *priv;

  priv = cern_component_model_component_get_instance_private(self);

  return priv->site;
}

static
ICernComponentModelContainer *
cern_component_model_component_get_container_real(CernComponentModelComponent *self) {
  CernComponentModelComponentPrivate *priv;
  priv = cern_component_model_component_get_instance_private(self);
  return i_cern_component_model_site_get_container(priv->site);
}

static
void
cern_component_model_component_class_init(CernComponentModelComponentClass *klass) {
  klass->can_raise_events = cern_component_model_component_can_raise_events_real;
  klass->get_site = cern_component_model_component_get_site_real;
  klass->set_site = cern_component_model_component_set_site_real;
  klass->get_service = cern_component_model_component_get_service_real;
  klass->get_container = cern_component_model_component_get_container_real;
}

gboolean
cern_component_model_component_can_raise_events(CernComponentModelComponent *self) {
  CernComponentModelComponentClass *klass;

  klass = CERN_COMPONENT_MODEL_COMPONENT_GET_CLASS(self);

  return (klass->can_raise_events)(self);
}

ICernComponentModelSite *
cern_component_model_component_get_site(CernComponentModelComponent *self) {
  CernComponentModelComponentClass *klass;

  klass = CERN_COMPONENT_MODEL_COMPONENT_GET_CLASS(self);

  return (klass->get_site)(self);
}

void
cern_component_model_component_set_site(CernComponentModelComponent *self,
                                        ICernComponentModelSite *site){
  CernComponentModelComponentClass *klass;

  klass = CERN_COMPONENT_MODEL_COMPONENT_GET_CLASS(self);

  (klass->set_site)(self, site);
}

gpointer
cern_component_model_component_get_service(CernComponentModelComponent *self,
                                           GType service) {
  CernComponentModelComponentClass *klass;

  klass = CERN_COMPONENT_MODEL_COMPONENT_GET_CLASS(self);

  return (klass->get_service)(self, service);
}

ICernComponentModelContainer *
cern_component_model_component_get_container(CernComponentModelComponent *self) {
  CernComponentModelComponentClass *klass;

  klass = CERN_COMPONENT_MODEL_COMPONENT_GET_CLASS(self);

  return (klass->get_container)(self);
}

CernComponentModelComponent *
cern_component_model_component_new(void) {
  return g_object_new(CERN_TYPE_COMPONENT_MODEL_COMPONENT, NULL);
}
