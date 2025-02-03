#ifndef CERN_UI_COMPONENT_MODEL_SITE_H
#define CERN_UI_COMPONENT_MODEL_SITE_H 1

#include <glib-object.h>

G_BEGIN_DECLS

typedef struct _ICernComponentModelComponent ICernComponentModelComponent;
typedef struct _ICernComponentModelContainer ICernComponentModelContainer;

#define I_CERN_TYPE_COMPONENT_MODEL_SITE i_cern_component_model_site_get_type()

G_DECLARE_INTERFACE(ICernComponentModelSite, i_cern_component_model_site,
                    I_CERN, COMPONENT_MODEL_SITE, GObject);

struct _ICernComponentModelSiteInterface {
  GTypeInterface parent_iface;

  ICernComponentModelComponent *
  (*get_component)(ICernComponentModelSite *self);

  ICernComponentModelContainer *
  (*get_container)(ICernComponentModelSite *self);

  gboolean
  (*get_gesign_mode)(ICernComponentModelSite *self);

  gchar const *
  (*get_name)(ICernComponentModelSite *self);

  void
  (*set_name)(ICernComponentModelSite *self, gchar const *name);
};

ICernComponentModelComponent *
i_cern_component_model_site_get_component(ICernComponentModelSite *self);

ICernComponentModelContainer *
i_cern_component_model_site_get_container(ICernComponentModelSite *self);

gboolean
i_cern_component_model_site_get_design_mode(ICernComponentModelSite *self);

gchar const *
i_cern_component_model_site_get_name(ICernComponentModelSite *self);

void
i_cern_component_model_site_set_name(ICernComponentModelSite *self,
                                     gchar const *name);

G_END_DECLS

#endif /* CERN_UI_COMPONENT_MODEL_SITE_H */
