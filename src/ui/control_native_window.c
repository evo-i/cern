#include "cern/ui/control_native_window.h"
#include "cern/ui/control.h"

#include <Windows.h>

static
void
cern_control_native_window_iface_init(CernWindowTargetInterface *iface);

struct _CernControlNativeWindow {
  CernNativeWindow parent_instance;

  CernControl *control;
  CernWindowTarget *window_target;
};

G_DEFINE_FINAL_TYPE_WITH_CODE(CernControlNativeWindow,
  cern_control_native_window, CERN_TYPE_NATIVE_WINDOW,
  G_IMPLEMENT_INTERFACE(CERN_TYPE_WINDOW_TARGET, cern_control_native_window_iface_init));

static
void
cern_control_native_window_finalize(GObject *object) {
  CernControlNativeWindow *self = CERN_CONTROL_NATIVE_WINDOW(object);

  g_clear_object(&self->control);
  g_clear_object(&self->window_target);

  G_OBJECT_CLASS(cern_control_native_window_parent_class)->finalize(object);
}

static
void
real_cern_control_native_window_window_proc(CernNativeWindow *target, CernMessage *message) {
  CernControlNativeWindow *self
    = CERN_CONTROL_NATIVE_WINDOW(target);

  switch(message->message) {
    case WM_MOUSELEAVE: {
      cern_control_unhook_mouse_event(CERN_CONTROL(self));
    } break;
    case WM_MOUSEMOVE: {
      if (!cern_control_get_state(CERN_CONTRL(self), CERN_CONTROL_STATE_TRACKINGMOUSEEVENT)) {
        cern_control_hook_mouse_event(CERN_CONTROL(self));
        if (cern_control_get_state(CERN_CONTRL(self), CERN_CONTROL_STATE_MOUSEENTERPENDING)) {
          cern_control_send_message(CERN_CONTROL(self), cern_control_get_message_mouse_enter(), 0, 0);
        } else {
          cern_control_set_state(CERN_CONTROL(self), CERN_CONTROL_STATE_MOUSEENTERPENDING, FALSE);
        }
      }
    } break;
    case WM_MOUSEWHEEL: {
      cern_control_reset_mouse_event_args(CERN_CONTROL(self));
    } break;
  }

  cern_control_native_window_on_message(self, message);
}

static
void
real_cern_control_native_window_handle_changed(CernNativeWindow *target, gpointer handle) {
  CernControlNativeWindow *self
    = CERN_CONTROL_NATIVE_WINDOW(target);

  CERN_WINDOW_TARGET_GET_IFACE(self->window_target)
    ->on_handle_change(self->window_target, handle);
}

static
void
cern_control_native_window_init_class(CernControlNativeWindowClass *klass) {
  CernNativeWindowClass *native_window_class;
  GObjectClass *object_class;

  native_window_class = CERN_NATIVE_WINDOW_GET_CLASS(klass);
  object_class = G_OBJECT_CLASS(klass);

  object_class->finalize
    = cern_control_native_window_finalize;

  native_window_class->handle_changed
    = real_cern_control_native_window_handle_changed;

  native_window_class->window_proc
    = real_cern_control_native_window_window_proc;
}

static
void
real_cern_control_native_window_on_handle_change(CernWindowTarget *target, gpointer handle) {
  CernControl *self = CERN_CONTROL(target);
  cern_control_set_handle(self, handle);
}

void
cern_control_native_window_iface_init(CernWindowTargetInterface *iface) {
  iface->on_handle_change = real_cern_control_native_window_on_handle_change;
}

CernControlNativeWindow *
cern_control_native_window_new(CernControl *control) {
  CernControlNativeWindow *self;

  self = g_object_new(CERN_TYPE_CONTROL_NATIVE_WINDOW, NULL);

  self->control = g_object_ref(control);
  self->window_target = CERN_WINDOW_TARGET(self);

  return self;
}

CernControl *
cern_control_native_window_get_control(CernControlNativeWindow *self) {
  return self->control;
}

void
cern_control_native_window_on_message(CernControlNativeWindow *self,
                                      CernMessage *message) {
  cern_control_window_proc(CERN_CONTROL(self), message);
}

CernWindowTarget *
cern_control_native_window_get_window_target(CernControlNativeWindow *self) {
  return self->window_target;
}

void
cern_control_native_window_window_proc(CernControlNativeWindow *self, CernMessage *message) {
  CernNativeWindowClass *klass;

  klass = CERN_NATIVE_WINDOW_GET_CLASS(self);
  klass->window_proc(CERN_NATIVE_WINDOW(self), message);
}

void
cern_control_native_window_on_handle_change(CernControlNativeWindow *self) {
  CernNativeWindowClass *native_window_class;

  native_window_class = CERN_NATIVE_WINDOW_GET_CLASS(self);

  native_window_class
    ->handle_changed(CERN_NATIVE_WINDOW(self),
                     cern_native_window_get_handle(CERN_NATIVE_WINDOW(self)));
}
