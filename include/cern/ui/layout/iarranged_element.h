#ifndef CERN_LAYOUT_IARRANGED_ELEMENT_H
#define CERN_LAYOUT_IARRANGED_ELEMENT_H 1

#include <glib-object.h>

#include "cern/drawing/rectangle.h"
#include "../bounds_specified.h"

G_BEGIN_DECLS

#define CERN_TYPE_IARRANGED_ELEMENT (cern_iarranged_element_get_type())

G_DECLARE_INTERFACE (CernIArrangedElement, cern_iarranged_element,
                      CERN, IARRANGED_ELEMENT, GObject)

CernRectangle
cern_iarranged_element_get_bounds(CernIArrangedElement *self);

void
cern_iarranged_element_set_bounds(CernIArrangedElement *self,
                                  CernRectangle *bounds,
                                  CernBoundsSpecified bounds_specified);

CernSize
cern_iarranged_element_get_preffered_size(CernIArrangedElement *self,
                                          CernSize *proposed_size);

CernRectangle
cern_iarranged_element_get_display_rectangle(CernIArrangedElement *self);

gboolean
cern_iarranged_element_participates_in_layout(CernIArrangedElement *self);

gpointer
cern_iarranged_element_get_properties(CernIArrangedElement *self);

void
cern_iarranged_element_perform_layout(CernIArrangedElement *self,
                                      CernIArrangedElement *affected_element,
                                      gchar const *property_name);

CernIArrangedElement *
cern_iarranged_element_get_container(CernIArrangedElement *self);

GPtrArray *
cern_iarranged_element_get_children(CernIArrangedElement *self);

G_END_DECLS

#endif /* CERN_UI_LAYOUT_I_ARRANGED_ELEMENT_H */
