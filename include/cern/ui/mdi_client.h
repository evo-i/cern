#ifndef CERN_UI_MDI_CLIENT_H
#define CERN_UI_MDI_CLIENT_H 1

#include <glib-object.h>
#include "control.h"

G_BEGIN_DECLS

#define CERN_TYPE_MDI_CLIENT cern_mdi_client_get_type ()

G_DECLARE_FINAL_TYPE(CernMdiClient, cern_mdi_client, CERN, MDI_CLIENT, CernControl);

G_END_DECLS

#endif /* CERN_UI_MDI_CLIENT_H */
