#ifndef CERN_UI_NATIVE_WINDOW_H
#define CERN_UI_NATIVE_WINDOW_H 1

#include <glib-object.h>
#include "create_params.h"
#include "message.h"

G_BEGIN_DECLS

typedef struct _CernControl CernControl;

#define CERN_TYPE_NATIVE_WINDOW cern_native_window_get_type ()

G_DECLARE_DERIVABLE_TYPE(CernNativeWindow, cern_native_window, CERN, NATIVE_WINDOW, GObject);

struct _CernNativeWindowClass {
  GObjectClass parent_class;

  void
  (*window_proc)
  (CernNativeWindow *self, CernMessage *message);

  void
  (*create_handle)
  (CernNativeWindow *self, CernCreateParams *create_params);

  void
  (*handle_changed)(CernNativeWindow *self, gpointer handle);

  gpointer reserved[12];
};

CernNativeWindow *
cern_native_window_new(void);

// TODO: Is needed or no?
CernNativeWindow *
cern_native_window_new_with_control(CernControl *control);

CernNativeWindow *
cern_native_window_from_handle_internal(gpointer handle);

gpointer
cern_native_window_get_handle(CernNativeWindow *self);

CernNativeWindow *
cern_native_window_get_previous_window(CernNativeWindow *self);

CernNativeWindow *
cern_native_window_get_next_window(CernNativeWindow *self);

gboolean
cern_native_window_is_any_handle_created(void);

guint32
cern_native_window_get_window_proc_flags(void);

void
cern_native_window_add_window_to_table(gpointer handle, CernNativeWindow *window);

void
cern_native_window_add_window_to_id_table(GObject *wrapper, gpointer handle);

void
cern_native_window_assign_handle(CernNativeWindow *self, gpointer handle);

gpointer
cern_native_window_user_def_window_proc(void);

void
cern_native_window_set_user_def_window_proc(gpointer proc);

void
cern_native_window_wnd_proc(CernNativeWindow *self, CernMessage *message);

void
cern_native_window_create_handle(CernNativeWindow *self,
                                 CernCreateParams *create_params);

G_END_DECLS

#endif /* CERN_UI_NATIVE_WINDOW_H */
