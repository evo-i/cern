#ifndef CERN_UI_COMPONENT_MODEL_I_CONTAINER_H
#define CERN_UI_COMPONENT_MODEL_I_CONTAINER_H 1

#include <glib-object.h>

G_BEGIN_DECLS

typedef struct _ICernComponentModelComponent ICernComponentModelComponent;

#define CERN_TYPE_COMPONENT_MODEL_I_CONTAINER \
i_cern_component_model_container_get_type ()

G_DECLARE_INTERFACE(ICernComponentModelContainer, i_cern_component_model_container,
                    I_CERN, COMPONENT_MODEL_CONTAINER, GObject);

struct _ICernComponentModelContainerInterface {
  GTypeInterface parent_iface;

  void
  (*add)(ICernComponentModelContainer *self, ICernComponentModelComponent *component);

  void
  (*add_named)(ICernComponentModelContainer *self, ICernComponentModelComponent *component, gchar const *name);

  GList *
  (*get_components)(ICernComponentModelContainer *self);

  void
  (*remove)(ICernComponentModelContainer *self, ICernComponentModelComponent *component);
};

void
i_cern_component_model_container_add(ICernComponentModelContainer *self,
                                     ICernComponentModelComponent *component);

void
i_cern_component_model_container_add_named(ICernComponentModelContainer *self,
                                           ICernComponentModelComponent *component,
                                           gchar const *name);

GList *
i_cern_component_model_container_get_components(ICernComponentModelContainer *self);

void
i_cern_component_model_container_remove(ICernComponentModelContainer *self,
                                        ICernComponentModelComponent *component);

G_END_DECLS

#endif /* CERN_UI_COMPONENT_MODEL_I_CONTAINER_H */
