#include "cern/ui/component_model/isite.h"
#include "cern/ui/component_model/component.h"
#include "cern/ui/component_model/icomponent.h"
#include "cern/ui/component_model/iservice_provider.h"

#include <stdbool.h>

#define STATE_CREATED                             (0x00000001)
#define STATE_VISIBLE                             (0x00000002)
#define STATE_ENABLED                             (0x00000004)
#define STATE_TABSTOP                             (0x00000008)
#define STATE_RECREATE                            (0x00000010)
#define STATE_MODAL                               (0x00000020)
#define STATE_ALLOWDROP                           (0x00000040)
#define STATE_DROPTARGET                          (0x00000080)
#define STATE_NOZORDER                            (0x00000100)
#define STATE_LAYOUTDEFERRED                      (0x00000200)
#define STATE_USEWAITCURSOR                       (0x00000400)
#define STATE_DISPOSED                            (0x00000800)
#define STATE_DISPOSING                           (0x00001000)
#define STATE_MOUSEENTERPENDING                   (0x00002000)
#define STATE_TRACKINGMOUSEEVENT                  (0x00004000)
#define STATE_THREADMARSHALLPENDING               (0x00008000)
#define STATE_SIZELOCKEDBYOS                      (0x00010000)
#define STATE_CAUSESVALIDATION                    (0x00020000)
#define STATE_CREATINGHANDLE                      (0x00040000)
#define STATE_TOPLEVEL                            (0x00080000)
#define STATE_ISACCESSIBLE                        (0x00100000)
#define STATE_OWNCTLBRUSH                         (0x00200000)
#define STATE_EXCEPTIONWHILEPAINTING              (0x00400000)
#define STATE_LAYOUTISDIRTY                       (0x00800000)
#define STATE_CHECKEDHOST                         (0x01000000)
#define STATE_HOSTEDINDIALOG                      (0x02000000)
#define STATE_DOUBLECLICKFIRED                    (0x04000000)
#define STATE_MOUSEPRESSED                        (0x08000000)
#define STATE_VALIDATIONCANCELLED                 (0x10000000)
#define STATE_PARENTRECREATING                    (0x20000000)
#define STATE_MIRRORED                            (0x40000000)
#define STATE2_HAVEINVOKED                        (0x00000001)
#define STATE2_SETSCROLLPOS                       (0x00000002)
#define STATE2_LISTENINGTOUSERPREFERENCECHANGED   (0x00000004)
#define STATE2_INTERESTEDINUSERPREFERENCECHANGED  (0x00000008)
#define STATE2_MAINTAINSOWNCAPTUREMODE            (0x00000010)
#define STATE2_BECOMINGACTIVECONTROL              (0x00000020)

#define STATE2_CLEARLAYOUTARGS                    (0x00000040)
#define STATE2_INPUTKEY                           (0x00000080)
#define STATE2_INPUTCHAR                          (0x00000100)
#define STATE2_UICUES                             (0x00000200)
#define STATE2_ISACTIVEX                          (0x00000400)
#define STATE2_USEPREFERREDSIZECACHE              (0x00000800)
#define STATE2_TOPMDIWINDOWCLOSING                (0x00001000)
#define STATE2_CURRENTLYBEINGSCALED               (0x00002000)

_Thread_local
static
gboolean
in_cross_thread_safe_call = FALSE;

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
