#include "cern/ui/control.h"
#include "cern/ui/win32/window.h"

typedef struct _CernControlPrivate {
  gpointer a;
} CernControlPrivate;

static
void
cern_control_win32_window_interface_init(CernWin32WindowInterface *iface) {
  // Initialize the interface here
}

static
void
cern_control_init(CernControl *self) {
  // Initialize the CernControl instance here
}

static
void
cern_control_class_init(CernControlClass *klass) {
  // Initialize the CernControl class here
}

G_DEFINE_TYPE_WITH_CODE(CernControl, cern_control,
  CERN_TYPE_COMPONENT_MODEL_COMPONENT,
  G_ADD_PRIVATE(CernControl)
  G_IMPLEMENT_INTERFACE(CERN_TYPE_WIN32_WINDOW,
    cern_control_win32_window_interface_init)
  /*G_IMPLEMENT_INTERFACE()*/);
