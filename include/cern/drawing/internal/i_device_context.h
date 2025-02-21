#ifndef I_DEVICE_CONTEXT
#define I_DEVICE_CONTEXT

#include <glib-object.h>

G_BEGIN_DECLS

#define CERN_TYPE_IDEVICE_CONTEXT (cern_i_device_context_get_type())
G_DECLARE_INTERFACE(CernIDeviceContext, cern_i_device_context, CERN, IDEVICE_CONTEXT, GObject)

struct _CernIDeviceContextInterface {
    GTypeInterface parent_iface;

    gpointer
    (*get_hdc)(CernIDeviceContext *self);
    void
    (*release_hdc)(CernIDeviceContext *self);
};

gpointer
cern_i_device_context_get_hdc(CernIDeviceContext *self);

void
cern_i_device_context_release_hdc(CernIDeviceContext *self);

G_END_DECLS

#endif /* I_DEVICE_CONTEXT */
