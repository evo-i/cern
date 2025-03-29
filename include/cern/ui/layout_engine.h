#ifndef CERN_UI_LAYOUT_ENGINE_H
#define CERN_UI_LAYOUT_ENGINE_H 1

#include <glib-object.h>
#include "bounds_specified.h"
#include "cern/drawing/size.h"
#include "layout/iarranged_element.h"

G_BEGIN_DECLS

#define CERN_TYPE_LAYOUT_ENGINE cern_layout_engine_get_type ()

G_DECLARE_DERIVABLE_TYPE(CernLayoutEngine, cern_layout_engine, CERN, LAYOUT_ENGINE, GObject);

struct _CernLayoutEngineClass {
  GObjectClass parent_class;

  CernIArrangedElement *
  (*cast_to_arranged_element)(CernLayoutEngine *self, GObject *obj);

  CernSize
  (*get_preffered_size)(CernLayoutEngine *self, CernIArrangedElement *element, CernSize *proposed_size);

  void
  (*init_layout)(CernLayoutEngine *self, GObject *child, CernBoundsSpecified bounds_specified);

  void
  (*init_layout_core)(CernLayoutEngine *self, CernIArrangedElement *element, CernBoundsSpecified bounds_specified);
  /* FIXME: Add methods here... */
};

G_END_DECLS

#endif /* CERN_UI_LAYOUT_ENGINE_H */
