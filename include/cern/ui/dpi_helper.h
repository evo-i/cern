#ifndef CERN_UI_DPI_HELPER_H
#define CERN_UI_DPI_HELPER_H 1

#include <glib-object.h>

G_BEGIN_DECLS

void
cern_dpi_helper_initialize(void);

gboolean
cern_dpi_helper_is_awareness_value_set(void);

void
cern_dpi_helper_initialize_helper_for_winforms(void);

void
cern_dpi_helper_initialize_helper_quirks();

gdouble
cern_dpi_helper_get_device_dpi(void);

G_END_DECLS

#endif /* CERN_UI_DPI_HELPER_H */
