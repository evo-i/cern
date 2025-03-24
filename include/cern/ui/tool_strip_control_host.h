#ifndef CERN_UI_TOOL_STRIP_CONTROL_HOST_H
#define CERN_UI_TOOL_STRIP_CONTROL_HOST_H 1

#include <glib-object.h>

G_BEGIN_DECLS

#define CERN_TYPE_TOOL_STRIP_CONTROL_HOST (cern_tool_strip_control_host_get_type())

G_DECLARE_FINAL_TYPE(CernToolStripControlHost, cern_tool_strip_control_host, CERN, TOOL_STRIP_CONTROL_HOST, GObject);

G_END_DECLS

#endif /* CERN_UI_TOOL_STRIP_CONTROL_HOST_H */
