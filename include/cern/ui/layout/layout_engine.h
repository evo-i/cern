#ifndef CERN_UI_LAYOUT_LAYOUT_ENGINE_H
#define CERN_UI_LAYOUT_LAYOUT_ENGINE_H 1

#include <glib-object.h>

#include "cern/drawing/size.h"
#include "../bounds_specified.h"
#include "iarranged_element.h"
#include "../component_model/layout_event_args.h"

G_BEGIN_DECLS

#define CERN_LAYOUT_ENGINE_TYPE (cern_layout_engine_get_type())

G_DECLARE_DERIVABLE_TYPE(CernLayoutEngine, cern_layout_engine, CERN, LAYOUT_ENGINE, GObject)

struct _CernLayoutEngineClass {
  GObjectClass parent_class;

  CernSize
  (*get_prefered_size)(CernLayoutEngine *self, CernIArrangedElement *container,
                       CernSize *proposed_constraint);

  void
  (*init_layout)(CernLayoutEngine *self, GObject *child,
                 CernBoundsSpecified specified);

  void
  (*init_layout_core)(CernLayoutEngine *self, CernIArrangedElement *element,
                      CernBoundsSpecified specified);

  void
  (*process_suspended_layout_event_args)(CernLayoutEngine *self,
                                         CernIArrangedElement *container,
                                         CernLayoutEventArgs *args);

  gboolean
  (*layout)(CernLayoutEngine *self, GObject *container, CernLayoutEventArgs *args);

  gboolean
  (*layout_core)(CernLayoutEngine *self, CernIArrangedElement *container,
                 CernLayoutEventArgs *args);
};

CernIArrangedElement *
cern_layout_engine_cast_to_arranged_element_internal(CernLayoutEngine *self,
                                                     GObject *obj);

CernSize
cern_layout_engine_get_preferred_size_internal(CernLayoutEngine *self,
                                               CernIArrangedElement *container,
                                               CernSize *proposed_constraint);

void
cern_layout_engine_init_layout(CernLayoutEngine *self, GObject *child,
                               CernBoundsSpecified specified);

void
cern_layout_engine_init_layout_core_internal(CernLayoutEngine *self,
                                             CernIArrangedElement *element,
                                             CernBoundsSpecified specified);

void
cern_layout_engine_process_suspended_layout_event_args(CernLayoutEngine *self,
                                                       CernIArrangedElement *container,
                                                       CernLayoutEventArgs *args);

gboolean
cern_layout_engine_layout(CernLayoutEngine *self, GObject *container,
                          CernLayoutEventArgs *args);

gboolean
cern_layout_engine_layout_core(CernLayoutEngine *self,
                               CernIArrangedElement *container,
                               CernLayoutEventArgs *args);

G_END_DECLS

#endif /* CERN_UI_LAYOUT_LAYOUT_ENGINE_H */
