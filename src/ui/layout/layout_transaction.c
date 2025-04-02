#include "cern/ui/layout/layout_transaction.h"
#include "cern/ui/component_model/icomponent.h"
#include "cern/ui/layout/iarranged_element.h"
#include "cern/ui/common_properties.h"
#include "cern/ui/control.h"
#include "cern/ui/component_model/layout_event_args.h"
#include <glib-object.h>
#include <glibconfig.h>

struct _CernLayoutTransaction {
  GObject parent_instance;

  CernControl *control_to_layout;
  gboolean resume_layout;
};

static
void
real_cern_layout_transaction_dispose(GObject *obj) {
  CernLayoutTransaction *self
    = CERN_LAYOUT_TRANSACTION(obj);

  if (self->control_to_layout != NULL) {
    cern_control_resume_layout_ex(self->control_to_layout, self->resume_layout);
    g_clear_object(&self->control_to_layout);
  }
}

static
void
cern_layout_transaction_init(CernLayoutTransaction *self) {
  (void) self;
}

static
void
cern_layout_transaction_class_init(CernLayoutTransactionClass *klass) {
  GObjectClass *obj_class = G_OBJECT_CLASS(klass);

  obj_class->dispose = real_cern_layout_transaction_dispose;
}

CernLayoutTransaction *
cern_layout_transaction_new(CernControl *control_to_layout,
                            CernIArrangedElement *control_causing_layout,
                            gchar const *property_name) {
  return
    cern_layout_transaction_new_resume_layout(control_to_layout,
                                              control_causing_layout,
                                              property_name,
                                              TRUE);
}

CernLayoutTransaction *
cern_layout_transaction_new_resume_layout(CernControl *control_to_layout,
                                          CernIArrangedElement *control_causing_layout,
                                          gchar const *property_name,
                                          gboolean resume_layout) {
  CernLayoutTransaction *self
    = g_object_new(CERN_TYPE_LAYOUT_TRANSACTION, NULL);

  cern_common_properties_x_clear_preferred_size_cache(control_causing_layout);

  self->control_to_layout = g_object_ref(control_to_layout);
  self->resume_layout = resume_layout;

  if (self->control_to_layout != NULL) {
    cern_control_suspend_layout(self->control_to_layout);
    cern_common_properties_x_clear_preferred_size_cache(CERN_IARRANGED_ELEMENT(self->control_to_layout));

    if (resume_layout) {
      ICernComponentModelComponent *component
        = I_CERN_COMPONENT_MODEL_COMPONENT(control_causing_layout);
      CernLayoutEventArgs *args
        = cern_layout_event_args_new(component, property_name);
      cern_control_perform_layout_ea(self->control_to_layout, args);
      g_clear_object(&args);
    }
  }

  return self;
}

GObject *
cern_layout_transaction_create_transaction_if(gboolean condition,
                                              CernControl *control_to_layout,
                                              CernIArrangedElement *control_causing_layout,
                                              gchar const *property_name) {
  if (condition) {
    return
      G_OBJECT(cern_layout_transaction_new(control_to_layout,
                                           control_causing_layout,
                                           property_name));
  } else {
    cern_common_properties_x_clear_preferred_size_cache(control_causing_layout);
    return g_object_new(G_TYPE_OBJECT, NULL);
  }
}

void
cern_layout_transaction_do_layout(CernIArrangedElement *element_to_layout,
                                  CernIArrangedElement *element_causing_layout,
                                  gchar const *property_name) {
  if (element_causing_layout != NULL) {
    cern_common_properties_x_clear_preferred_size_cache(element_causing_layout);

    if (element_to_layout != NULL) {
      cern_common_properties_x_clear_preferred_size_cache(element_to_layout);
      cern_iarranged_element_perform_layout(element_to_layout,
                                            element_causing_layout,
                                            property_name);
    }
  }
}

void
cern_layout_transaction_do_layout_if(gboolean condition,
                                     CernIArrangedElement *element_to_layout,
                                     CernIArrangedElement *element_causing_layout,
                                     gchar const *property_name) {
  if (!condition) {
    if (element_causing_layout != NULL) {
      cern_common_properties_x_clear_preferred_size_cache(element_causing_layout);
    }
  } else {
    cern_layout_transaction_do_layout(element_to_layout,
                                      element_causing_layout,
                                      property_name);
  }
}

