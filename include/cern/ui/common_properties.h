#ifndef CERN_UI_COMMON_PROPERTIES_H
#define CERN_UI_COMMON_PROPERTIES_H 1

#include <glib-object.h>
#include "padding.h"
#include "layout/iarranged_element.h"

G_BEGIN_DECLS

#define CERN_COMMON_PROPERTIES_DEFAULT_MARGIN (CernPadding) { 3, 3, 3, 3 }

void
cern_common_properties_set_auto_size_mode(CernIArrangedElement *element,
                                          CernAutoSizeMode mode);

CernAutoSizeMode
cern_common_properties_get_auto_size_mode(CernIArrangedElement *element);

G_END_DECLS

#endif /* CERN_UI_COMMON_PROPERTIES_H */
