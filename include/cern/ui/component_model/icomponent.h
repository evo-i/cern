#ifndef CERN_UI_ICOMPONENT_H
#define CERN_UI_ICOMPONENT_H 1

#include <glib-object.h>

G_BEGIN_DECLS

typedef struct _ICernComponentModelSite ICernComponentModelSite;

#define CERN_TYPE_I_COMPONENT_MODEL_COMPONENT \
i_cern_component_model_component_get_type ()

G_DECLARE_INTERFACE(ICernComponentModelComponent,
                    i_cern_component_model_component, I_CERN,
                    COMPONENT_MODEL_COMPONENT, GObject);

struct _ICernComponentModelComponentInterface {
  GTypeInterface parent_iface;

  ICernComponentModelSite *
  (*get_site)(ICernComponentModelComponent *self);

  void
  (*set_site)(ICernComponentModelComponent *self, ICernComponentModelSite *site);
};

ICernComponentModelSite *
i_cern_component_model_component_get_site(ICernComponentModelComponent *self);

void
i_cern_component_model_component_set_site(ICernComponentModelComponent *self,
                                          ICernComponentModelSite *site);

G_END_DECLS

#endif /* CERN_UI_ICOMPONENT_H */
