#ifndef CERN_UI_LAYOUT_DEFAULT_LAYOUT_H
#define CERN_UI_LAYOUT_DEFAULT_LAYOUT_H 1

#include <glib-object.h>
#include "layout_engine.h"
#include "iarranged_element.h"
#include "cern/drawing/size_f.h"
#include "../anchor_styles.h"
#include "../dock_style.h"

G_BEGIN_DECLS

#define CERN_TYPE_DEFAULT_LAYOUT cern_default_layout_get_type ()

G_DECLARE_DERIVABLE_TYPE(CernDefaultLayout, cern_default_layout, CERN, DEFAULT_LAYOUT, CernLayoutEngine);

struct _CernDefaultLayoutClass {
  CernLayoutEngineClass parent_class;
};

CernDefaultLayout *
cern_default_layout_get_instance(void);

CernAnchorStyles
cern_default_layout_get_anchor(CernIArrangedElement *elment);

void
cern_default_layout_set_anchor(CernIArrangedElement *element, CernAnchorStyles value);

CernDockStyle
cern_default_layout_get_dock(CernIArrangedElement *element);

void
cern_default_layout_set_dock(CernIArrangedElement *element, CernDockStyle value);

void
cern_default_layout_scale_anchor_info(CernIArrangedElement *element, CernSizeF factor);

gboolean
cern_default_layout_get_is_anchored(CernAnchorStyles anchor, CernAnchorStyles desired_anchor);

G_END_DECLS

#endif /* CERN_UI_LAYOUT_DEFAULT_LAYOUT_H */
