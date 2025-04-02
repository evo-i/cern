#ifndef CERN_UI_LAYOUT_TRANSACTION_H
#define CERN_UI_LAYOUT_TRANSACTION_H 1

#include <glib-object.h>

G_BEGIN_DECLS

typedef struct _CernIArrangedElement CernIArrangedElement;
typedef struct _CernControl CernControl;

#define CERN_TYPE_LAYOUT_TRANSACTION cern_layout_transaction_get_type ()

G_DECLARE_FINAL_TYPE(CernLayoutTransaction,
                     cern_layout_transaction,
                     CERN, LAYOUT_TRANSACTION, GObject);

CernLayoutTransaction *
cern_layout_transaction_new(CernControl *control_to_layout,
                            CernIArrangedElement *control_causing_layout,
                            gchar const *property_name);

CernLayoutTransaction *
cern_layout_transaction_new_resume_layout(CernControl *control_to_layout,
                                          CernIArrangedElement *control_causing_layout,
                                          gchar const *property_name,
                                          gboolean resume_layout);

GObject *
cern_layout_transaction_create_transaction_if(gboolean condition,
                                              CernControl *control_to_layout,
                                              CernIArrangedElement *control_causing_layout,
                                              gchar const *property_name);

void
cern_layout_transaction_do_layout(CernIArrangedElement *element_to_layout,
                                  CernIArrangedElement *element_causing_layout,
                                  gchar const *property_name);

void
cern_layout_transaction_do_layout_if(gboolean condition,
                                     CernIArrangedElement *element_to_layout,
                                     CernIArrangedElement *element_causing_layout,
                                     gchar const *property_name);

G_END_DECLS

#endif /* CERN_UI_LAYOUT_TRANSACTION_H */
