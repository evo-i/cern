#ifndef CERN_UI_CONTROL_NATIVE_WINDOW_H
#define CERN_UI_CONTROL_NATIVE_WINDOW_H 1

#include <glib-object.h>

#include "native_window.h"
#include "window_target.h"

G_BEGIN_DECLS

#define CERN_TYPE_CONTROL_NATIVE_WINDOW (cern_control_native_window_get_type())

G_DECLARE_FINAL_TYPE(CernControlNativeWindow, cern_control_native_window, CERN, CONTROL_NATIVE_WINDOW, CernNativeWindow);

CernControlNativeWindow *
cern_control_native_window_new(CernControl *control);

CernControl *
cern_control_native_window_get_control(CernControlNativeWindow *self);

void
cern_control_native_window_on_message(CernControlNativeWindow *self, CernMessage *message);

CernWindowTarget *
cern_control_native_window_get_window_target(CernControlNativeWindow *self);

void
cern_control_native_window_window_proc(CernControlNativeWindow *self, CernMessage *message);

void
cern_control_native_window_on_handle_change(CernControlNativeWindow *self);

G_END_DECLS

#endif /* CERN_UI_CONTROL_NATIVE_WINDOW_H */
