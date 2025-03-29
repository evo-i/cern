#include "cern/ui/native_window.h"
#include "cern/ui/win32/window.h"

#include <Windows.h>
#include <math.h>

#define CERN_NATIVE_WINDOW_INITIALIZED_FLAGS 0x01
#define CERN_NATIVE_WINDOW_DEBUGGER_PRESENT 0x02
#define CERN_NATIVE_WINDOW_USE_DEBUGGABLE_WND_PROC 0x04
#define CERN_NATIVE_WINDOW_LOAD_CONFIG_SETTINGS 0x08
#define CERN_NATIVE_WINDOW_ASSEMBLY_IS_DEBUGGABLE 0x10

#define CERN_NATIVE_WINDOW_HASH_LOAD_FACTOR (gfloat)(0.72F)

#pragma region CernWindowClass

#define CERN_TYPE_WINDOW_CLASS cern_window_class_get_type ()

G_DECLARE_FINAL_TYPE(CernWindowClass, cern_window_class, CERN, WINDOW_CLASS, GObject)

CernWindowClass *
cern_window_class_create(gchar const *class_name, gint32 class_style);

void
cern_window_class_dispose_cache(void);

CernWindowClass *
cern_window_class_new(gchar const *class_name, gint32 class_style);

gpointer
cern_window_class_callback(gpointer h_wnd, guint32 message,
                           gpointer w_param, gpointer l_param);

gchar *
cern_window_class_get_full_class_name(CernWindowClass *self);

void
cern_window_class_register(CernWindowClass *self);

void
cern_window_class_unregister(CernWindowClass *self);

static
CernWindowClass *cern_cache;

static
GMutex
cern_internal_sync_object = { 0 };

static
GOnce
cern_once = G_ONCE_INIT;

static
gint32
cern_domain_qualifier;

static
gpointer
cern_once_init(gpointer data);

struct _CernWindowClass {
  CernWindowClass *next;
  gchar *class_name;
  gint32 class_style;
  gchar *window_class_name;
  gint32 hash_code;
  gpointer def_window_proc;
  WNDPROC window_proc;
  gboolean registered;
  CernNativeWindow *target_window;
};

G_DEFINE_FINAL_TYPE(CernWindowClass, cern_window_class, G_TYPE_OBJECT)

static
void
cern_window_class_finalize(GObject *object) {
  CernWindowClass *self = CERN_WINDOW_CLASS(object);

  g_free(self->class_name);
  g_free(self->window_class_name);

  G_OBJECT_CLASS(cern_window_class_parent_class)->finalize(object);
}

static
void
cern_window_class_class_init(CernWindowClassClass *klass) {
  GObjectClass *object_class = G_OBJECT_CLASS(klass);
  object_class->finalize = cern_window_class_finalize;
}

static
void
cern_window_class_init(CernWindowClass *self) {
  self->next = cern_cache;
  cern_cache = self;
}

static
gpointer
cern_native_window_callback(CernNativeWindow *self, gpointer h_wnd,
                            guint32 message, gpointer wparam, gpointer lparam) {
  if (self == NULL) {
    return
      (gpointer) DefWindowProcA(h_wnd, message, (WPARAM) wparam, (LPARAM) lparam);
  }

  return NULL;
}

CernWindowClass *
cern_window_class_create(gchar const *class_name, gint32 class_style) {
  g_once(&cern_once, cern_once_init, NULL);

  g_mutex_lock(&cern_internal_sync_object);

  CernWindowClass *wc = cern_cache;

  if (class_name == NULL) {
    while (wc != NULL
           && (wc->class_name != NULL || wc->class_style != class_style)) {
      wc = wc->next;
    }
  } else {
    while (wc != NULL
           && strcmp(wc->class_name, class_name) != 0) {
      wc = wc->next;
    }
  }

  if (wc == NULL) {
    wc = cern_window_class_new(class_name, class_style);
    wc->next = cern_cache;
    cern_cache = wc;
  } else {
    if (!wc->registered) {
      cern_window_class_register(wc);
    }
  }

  g_mutex_unlock(&cern_internal_sync_object);

  return wc;
}

void
cern_window_class_dispose_cache(void) {
  g_once(&cern_once, cern_once_init, NULL);
  g_mutex_lock(&cern_internal_sync_object);

  CernWindowClass *wc = cern_cache;

  while (wc != NULL) {
    if (wc->registered) {
      cern_window_class_unregister(wc);
    }
  }

  g_mutex_unlock(&cern_internal_sync_object);
}

CernWindowClass *
cern_window_class_new(gchar const *class_name, gint32 class_style) {
  CernWindowClass *self = g_object_new(CERN_TYPE_WINDOW_CLASS, NULL);

  self->class_name = g_strdup(class_name);
  self->class_style = class_style;
  cern_window_class_register(self);
  return self;
}

gpointer
cern_window_class_callback(gpointer h_wnd, guint32 message,
                           gpointer w_param, gpointer l_param) {
  ULONG_PTR window_class_ptr = GetClassLongPtrA(h_wnd, GCL_CBWNDEXTRA);
  CernWindowClass *wc = (CernWindowClass *) window_class_ptr;

  cern_native_window_assign_handle(wc->target_window, h_wnd);

  return
    cern_native_window_callback(wc->target_window, h_wnd, message, w_param, l_param);
}

gchar *
cern_window_class_get_full_class_name(CernWindowClass *self) {
  return
    g_strdup_printf("%s.%s.app.%d.%x", "Cern", self->class_name,
                    cern_domain_qualifier, self->hash_code);
}

void
cern_window_class_register(CernWindowClass *self) {
  WNDCLASSEX wnd_class = { 0 };

  if (cern_native_window_user_def_window_proc() == NULL) {
    gpointer proc = GetProcAddress(GetModuleHandleA(NULL), "DefWindowProcA");
    cern_native_window_set_user_def_window_proc(proc);
  }

  gchar *local_class_name = self->class_name;

  if (local_class_name == NULL) {
    wnd_class.hbrBackground = GetStockObject(HOLLOW_BRUSH);
    wnd_class.style = self->class_style;
    self->def_window_proc = cern_native_window_user_def_window_proc();
    self->hash_code = 0;
    local_class_name = g_strdup_printf("%s%x", "Window.", self->class_style);
  } else {
    WNDCLASSEX wnd_class_test = { 0 };
    wnd_class.cbSize = sizeof(WNDCLASSEX);
    wnd_class.cbWndExtra = sizeof(CernWindowClass *);
    if(GetClassInfoExA(GetModuleHandleA(NULL), self->class_name, &wnd_class_test)) {
      wnd_class.style = wnd_class_test.style;
      wnd_class.cbClsExtra = wnd_class_test.cbClsExtra;
      wnd_class.cbWndExtra = wnd_class_test.cbWndExtra;
      wnd_class.hIcon = wnd_class_test.hIcon;
      wnd_class.hCursor = wnd_class_test.hCursor;
      wnd_class.hbrBackground = wnd_class_test.hbrBackground;
      wnd_class.lpszMenuName = wnd_class_test.lpszMenuName;
      local_class_name = self->class_name;
      self->def_window_proc = wnd_class_test.lpfnWndProc;
      self->hash_code = (gint32)(0x72 ^ wnd_class_test.style);
    }
  }

  self->window_class_name = cern_window_class_get_full_class_name(self);
  self->window_proc = (WNDPROC) cern_window_class_callback;
  wnd_class.lpfnWndProc = self->window_proc;
  wnd_class.hInstance = GetModuleHandleA(NULL);
  wnd_class.lpszClassName = self->window_class_name;

  ATOM atom = RegisterClassExA(&wnd_class);

  if (atom == 0) {
    DWORD error = GetLastError();

    if (error == ERROR_CLASS_ALREADY_EXISTS) {
      WNDCLASSEXA wcls = { 0 };

      BOOL ok
        = GetClassInfoExA(GetModuleHandleA(NULL), self->window_class_name, &wcls);

      if (ok && wcls.lpfnWndProc == cern_native_window_user_def_window_proc()) {
        if (UnregisterClassA(self->window_class_name, GetModuleHandleA(NULL))) {
          atom = RegisterClassExA(&wnd_class);
        }
      } else {
        do {
          cern_domain_qualifier++;
          g_free(self->window_class_name);
          self->window_class_name = cern_window_class_get_full_class_name(self);
          wnd_class.lpszClassName = self->window_class_name;
          atom = RegisterClassExA(&wnd_class);
        } while (atom == 0 && GetLastError() == ERROR_CLASS_ALREADY_EXISTS);
      }
    }

    if (atom == 0) {
      g_critical("Failed to register window class '%s'.", self->window_class_name);
      g_object_unref(self);
      return;
    }
  }

  SetClassLongPtrA(NULL, GCL_CBWNDEXTRA, (LONG_PTR) self);

  self->registered = TRUE;
}

void
cern_window_class_unregister(CernWindowClass *self) {
  if (self->registered) {
    UnregisterClassA(self->window_class_name, GetModuleHandleA(NULL));
    self->window_proc = NULL;
    self->registered = FALSE;
  }
}

#pragma endregion

typedef struct _CernNativeWindowPrivate {
  gpointer handle;
  gpointer(__stdcall *window_proc)(gpointer, guint32, gpointer, gpointer);
  gpointer window_proc_ptr;
  gpointer def_window_proc;
  gboolean handle_owner;
  CernNativeWindow *previous_window;
  CernNativeWindow *next_window;
  GWeakRef self_weak_ref;
  GMutex sync_object;
} CernNativeWindowPrivate;

static
gint32
_Thread_local
const primes[] = {
  11,17,23,29,37,47,59,71,89,107,131,163,197,239,293,353,431,521,631,761,919,
  1103,1327,1597,1931,2333,2801,3371,4049,4861,5839,7013,8419,10103,12143,14591,
  17519,21023,25229,30293,36353,43627,52361,62851,75431,90523, 108631, 130363,
  156437, 187751, 225307, 270371, 324449, 389357, 467237, 560689, 672827, 807403,
  968897, 1162687, 1395263, 1674319, 2009191, 2411033, 2893249, 3471899, 4166287,
  4999559, 5999471, 7199369
};

typedef struct _CernHashBucket {
  gpointer handle;
  gpointer window;
  gssize hash;
} CernHashBucket;

static
gboolean
cern_any_handle_created = FALSE;

static
gboolean
cern_any_handle_created_in_app = FALSE;

static
gsize
cern_handle_count = 0;

static
gsize
cern_hash_load_size = 0;

static
struct {
  CernHashBucket *array;
  gsize size;
} cern_hash_buckets = { 0 };

static
gpointer
cern_user_def_window_proc = NULL;

static
guint8
_Thread_local
cern_window_proc_flags = 0;

static
guint8
_Thread_local
cern_user_set_proc_flags = 0;

static
guint8
cern_user_set_proc_flags_for_app = 0;

static
gsize
cern_global_id = 1;

GMutex
cern_create_window_sync_object = { 0 };

static
GHashTable *
cern_hash_for_id_handle = NULL;

static
GHashTable *
cern_hash_for_handle_id = NULL;

GOnce
cern_native_window_init_gonce = G_ONCE_INIT;

static
guint32
cern_native_window_message_unsubclass(void) {
  static guint32 unsubclass = -1;

  if (unsubclass == -1) {
    unsubclass = RegisterWindowMessage("CERN_WINDOW_UNSUBCLASS");
  }

  return unsubclass;
}

static
void
cern_native_window_on_shutdown(GObject *sender, gpointer user_data) {

}

static
gsize
cern_get_prime(gssize min_size) {
  if (min_size < 0) {
    g_critical("CernNativeWindow hash table capacity overflow.");
    return 0;
  }

  for (gsize i = 0; i < G_N_ELEMENTS(primes); i++) {
    if (primes[i] >= min_size) {
      return primes[i];
    }
  }

  for (gsize j = ((min_size - 2) | 1); j < G_MAXSIZE; j += 2) {
    gboolean prime = TRUE;

    if (j & 1) {
      gsize target = (gsize) sqrt(j);
      for (gsize div = 3; div < target; div += 2) {
        if ((j % div) == 0) {
          prime = FALSE;
          break;
        }
      }
      if (prime) {
        return j;
      }
    } else {
      if (j == 2) {
        return j;
      }
    }
  }

  return min_size;
}

static
void
cern_expand_table(void) {
  gsize old_size = cern_hash_buckets.size;
  gsize hash_size = cern_get_prime(1 + old_size * 2);

  CernHashBucket *old_buckets = cern_hash_buckets.array;
  CernHashBucket *new_buckets =  g_new0(CernHashBucket, hash_size);

  for (gsize nb = 0; nb < old_size; nb++) {
    CernHashBucket *old_b = &cern_hash_buckets.array[nb];
    if ((old_b->handle != NULL) && (old_b->handle != (gpointer) -1)) {
      gsize seed = old_b->hash & 0x7FFFFFFF;
      gsize incr = (1 + (((seed >> 5) + 1) % hash_size - 1));

      do {
        gsize bucket_number = seed % hash_size;

        if (new_buckets[bucket_number].handle == NULL
            || new_buckets[bucket_number].handle == (gpointer) -1) {
          new_buckets[bucket_number].handle = old_b->handle;
          new_buckets[bucket_number].window = old_b->window;
          new_buckets[bucket_number].hash |= old_b->hash & 0x7FFFFFFF;
          break;
        }
        new_buckets[bucket_number].hash = 0x80000000;
        seed += incr;
      } while(TRUE);
    }
  }

  cern_hash_buckets.array = new_buckets;
  cern_hash_buckets.size = hash_size;
  cern_hash_load_size = hash_size * CERN_NATIVE_WINDOW_HASH_LOAD_FACTOR;
  if (cern_hash_load_size >= hash_size) {
    cern_hash_load_size = hash_size - 1;
  }

  g_free(old_buckets);
}

static
gpointer
cern_once_init(gpointer data) {
  const gsize hash_size = primes[4];
  cern_hash_buckets.array = g_new0(CernHashBucket, hash_size);
  cern_hash_load_size = CERN_NATIVE_WINDOW_HASH_LOAD_FACTOR * hash_size;

  if (cern_hash_load_size >= hash_size) {
    cern_hash_load_size = hash_size - 1;
  }

  cern_hash_for_id_handle = g_hash_table_new(g_direct_hash, g_direct_equal);
  cern_hash_for_handle_id = g_hash_table_new(g_direct_hash, g_direct_equal);

  g_mutex_init(&cern_internal_sync_object);
  g_mutex_init(&cern_create_window_sync_object);
  return NULL;
}

static
void
cern_native_window_interface_init_win32_window(CernWin32WindowInterface *iface) {
  iface->get_native
    = (gpointer (*)(CernWin32Window *)) cern_native_window_get_handle;
}

G_DEFINE_TYPE_WITH_CODE(CernNativeWindow, cern_native_window, G_TYPE_OBJECT,
  G_ADD_PRIVATE(CernNativeWindow)
  G_IMPLEMENT_INTERFACE(CERN_TYPE_WIN32_WINDOW, cern_native_window_interface_init_win32_window))

static
LRESULT
cern_native_window_proc_redirect(HWND h_wnd, UINT msg,
                                 WPARAM w_param, LPARAM l_param) {
  CernNativeWindow *window = (CernNativeWindow *) GetWindowLongPtr(h_wnd, GWLP_USERDATA);

  if (window != NULL) {
    CernNativeWindowPrivate *priv = cern_native_window_get_instance_private(window);
    return (LRESULT) priv->window_proc(window, msg, (gpointer) w_param, (gpointer) l_param);
  }

  return DefWindowProcA(h_wnd, msg, w_param, l_param);
}

static
gint32
cern_native_window_wnd_proc_flags(void) {
  gint32 window_proc_flags = cern_window_proc_flags;

  if (window_proc_flags == 0) {
    if (cern_user_set_proc_flags != 0) {
      window_proc_flags = cern_user_set_proc_flags;
    } else if (cern_user_set_proc_flags_for_app != 0) {
      window_proc_flags = cern_user_set_proc_flags_for_app;
    } // TODO: Application.CustomThreadExceptionHandlerAttached ???

    window_proc_flags |= CERN_NATIVE_WINDOW_INITIALIZED_FLAGS;
    cern_window_proc_flags = (guint8) window_proc_flags;
  }

  return cern_window_proc_flags;
}

static
void
cern_native_window_force_exit_message_loop(CernNativeWindow *self) {
  CernNativeWindowPrivate *priv = cern_native_window_get_instance_private(self);

  gpointer h = priv->handle;
  gboolean owned_handle = priv->handle_owner;

  if (h != NULL) {
    if (IsWindow(priv->handle)) {
      // TODO: CernThreadContext is needed...
      DWORD proc_id = 0;
      guint32 thread_id = GetWindowThreadProcessId(priv->handle, &proc_id);
      HANDLE thread_handle = OpenThread(THREAD_ALL_ACCESS, TRUE, thread_id);
      if (thread_handle != NULL) {
        DWORD exit_code = 0;
        guint32 unsubclass = cern_native_window_message_unsubclass();
        GetExitCodeThread(thread_handle, &exit_code);
        CloseHandle(thread_handle);

        if (exit_code == STATUS_PENDING) {
          DWORD_PTR result;
          if (SendMessageTimeoutA(priv->handle, unsubclass, 0, 0,
                                  SMTO_ABORTIFHUNG, 100, &result) != 0) {
            if (result != 0) {
              exit_code = GPOINTER_TO_UINT(result);
            }
          } else {
            g_warning("Failed to ping window: %u, thread: %u",
                      GPOINTER_TO_UINT(h), thread_id);
          }
        }
      }
    }

    if (priv->handle != NULL) {
      //cern_native_window_release_handle(self, TRUE);
    }
  }

  if (h != NULL && priv->handle_owner) {
    PostMessage(h, WM_CLOSE, 0, 0);
  }
}

static
void
real_cern_native_window_finalize(GObject *object) {
  cern_native_window_force_exit_message_loop(CERN_NATIVE_WINDOW(object));
  G_OBJECT_CLASS(cern_native_window_parent_class)->finalize(object);
}

static
void
real_cern_native_window_create_handle(CernNativeWindow *self,
                                      CernCreateParams *params) {
  CernNativeWindowPrivate *priv = cern_native_window_get_instance_private(self);

  if (priv->handle != NULL) {
    g_error("Handle already exists");
  }
}

static
void
real_cern_native_window_wnd_proc(CernNativeWindow *self, CernMessage *message) {

}

static
void
real_cern_native_window_handle_changed(CernNativeWindow *self, gpointer handle) { }

static
void
cern_native_window_class_init(CernNativeWindowClass *klass) {
  GObjectClass *object_class = G_OBJECT_CLASS(klass);
  object_class->finalize = real_cern_native_window_finalize;
  klass->window_proc = real_cern_native_window_wnd_proc;
  klass->create_handle = real_cern_native_window_create_handle;
  klass->handle_changed = real_cern_native_window_handle_changed;
}

static
void
cern_native_window_init(CernNativeWindow *self) {
  CernNativeWindowPrivate *priv = cern_native_window_get_instance_private(self);
  g_mutex_init(&priv->sync_object);
};

CernNativeWindow *
cern_native_window_new(void) {
  g_once(&cern_once, cern_once_init, NULL);

  CernNativeWindow *self = g_object_new(CERN_TYPE_NATIVE_WINDOW, NULL);
  CernNativeWindowPrivate *priv = cern_native_window_get_instance_private(self);
  g_weak_ref_set(&priv->self_weak_ref, self);
  return self;
}

gboolean
cern_native_window_is_any_handle_created(void) {
  return cern_any_handle_created;
}

gpointer
cern_native_window_get_handle(CernNativeWindow *self) {
  CernNativeWindowPrivate *priv
    = cern_native_window_get_instance_private(self);
  return priv->handle;
}

CernNativeWindow *
cern_native_window_get_previous_window(CernNativeWindow *self) {
  CernNativeWindowPrivate *priv = cern_native_window_get_instance_private(self);
  return priv->previous_window;
}

CernNativeWindow *
cern_native_window_get_next_window(CernNativeWindow *self) {
  CernNativeWindowPrivate *priv = cern_native_window_get_instance_private(self);
  return priv->next_window;
}

gpointer
cern_native_window_user_def_window_proc(void) {
  return cern_user_def_window_proc;
}

void
cern_native_window_set_user_def_window_proc(gpointer proc) {
  cern_user_def_window_proc = proc;
}

static
gsize
cern_init_hash(gpointer handle, gsize hash_size, gsize *seed, gsize *incr) {
  gsize hash_code = ((gsize) handle) & 0x7FFFFFFF;
  (*seed) = hash_code;
  (*incr) = (1 + ((((*seed) >> 5) + 1) % (hash_size - 1)));
  return hash_code;
}

void
cern_native_window_add_window_to_table(gpointer handle, CernNativeWindow *window) {
  g_once(&cern_once, cern_once_init, NULL);

  g_mutex_lock(&cern_internal_sync_object);

  if (cern_handle_count >= cern_hash_load_size) {
    cern_expand_table();
  }

  cern_any_handle_created = TRUE;
  cern_any_handle_created_in_app = TRUE;

  gsize seed = 0;
  gsize incr = 0;

  gsize hash_code = cern_init_hash(handle, cern_hash_buckets.size, &seed, &incr);

  gsize ntry = 0;
  gssize empty_slot = -1;

  do {
    gsize bucket_number = seed % cern_hash_buckets.size;
    if (empty_slot == -1
        && cern_hash_buckets.array[bucket_number].handle == (gpointer) -1
        && cern_hash_buckets.array[bucket_number].hash < 0) {
      empty_slot = bucket_number;
    }

    if (cern_hash_buckets.array[bucket_number].handle == NULL
        || cern_hash_buckets.array[bucket_number].handle == (gpointer) -1
        && (cern_hash_buckets.array[bucket_number].hash & 0x80000000)) {
      if (empty_slot != -1) {
        bucket_number = empty_slot;
      }

      cern_hash_buckets.array[bucket_number].handle = handle;
      cern_hash_buckets.array[bucket_number].window = window;
      cern_hash_buckets.array[bucket_number].hash |= hash_code;
      cern_handle_count++;
      g_mutex_unlock(&cern_internal_sync_object);
      return;
    }

    if ((cern_hash_buckets.array[bucket_number].hash & 0x7FFFFFFF) == hash_code
        && handle == cern_hash_buckets.array[bucket_number].handle) {
      gpointer prev_window = cern_hash_buckets.array[bucket_number].window;
      if (prev_window != NULL) {
        CernNativeWindowPrivate *priv
          = cern_native_window_get_instance_private(window);
        priv->previous_window = prev_window;

        CernNativeWindowPrivate *priv_next
          = cern_native_window_get_instance_private(priv->previous_window);
        priv_next->next_window = window;
      }

      cern_hash_buckets.array[bucket_number].window = window;
      g_mutex_unlock(&cern_internal_sync_object);
      return;
    }

    if (empty_slot == -1) {
      cern_hash_buckets.array[bucket_number].hash |= 0x80000000;
    }

    seed += incr;
  } while (++ntry < cern_hash_buckets.size);

  if (empty_slot != -1) {
    cern_hash_buckets.array[empty_slot].handle = handle;
    cern_hash_buckets.array[empty_slot].window = window;
    cern_hash_buckets.array[empty_slot].hash |= hash_code;
    cern_handle_count++;
    g_mutex_unlock(&cern_internal_sync_object);
    return;
  }

  g_mutex_unlock(&cern_internal_sync_object);

  g_critical("native window hash table insert failed! "
             "Load factor too high, or our double hashing function "
             "is incorrect.");
}

void
cern_native_window_add_window_to_id_table(GObject *wrapper, gpointer handle) {
  g_once(&cern_once, cern_once_init, NULL);

  g_hash_table_insert(cern_hash_for_id_handle, (gpointer) cern_global_id, handle);
  g_hash_table_insert(cern_hash_for_handle_id, handle, (gpointer) cern_global_id);
  SetWindowLong(handle, GWL_ID, cern_global_id);
  cern_global_id++;
}

void
cern_native_window_assign_handle(CernNativeWindow *self, gpointer handle) {
  CernNativeWindowPrivate *priv = cern_native_window_get_instance_private(self);

  g_mutex_lock(&priv->sync_object);

  g_assert(handle == NULL);

  priv->handle = handle;

  if (cern_user_def_window_proc == NULL) {
    cern_user_def_window_proc
      = GetProcAddress(GetModuleHandle(NULL), "DefWindowProcA");
  }

  priv->def_window_proc = (gpointer) GetWindowLongPtrA(handle, GWLP_WNDPROC);

  priv->window_proc
    = (gpointer(*)(gpointer, guint32, gpointer, gpointer)) cern_native_window_proc_redirect;

  priv->window_proc_ptr
    = (gpointer) GetWindowLongPtrA(handle, GWLP_WNDPROC);

  g_assert(priv->window_proc == priv->window_proc_ptr
           && "Uh oh! Subclassed ourselves!!!");

  SetWindowLongPtrA(handle, GWLP_ID, (LONG_PTR) handle);

  g_mutex_unlock(&priv->sync_object);
}

static
void
cern_native_window_def_wnd_proc(CernNativeWindow *self, CernMessage *message) {
  CernNativeWindowPrivate *priv = cern_native_window_get_instance_private(self);
}

void
cern_native_window_wnd_proc(CernNativeWindow *self, CernMessage *message) {
  CernNativeWindowClass *klass = CERN_NATIVE_WINDOW_GET_CLASS(self);
  klass->window_proc(self, message);
}

void
cern_native_window_create_handle(CernNativeWindow *self,
                                 CernCreateParams *create_params) {
  CernNativeWindowClass *klass = CERN_NATIVE_WINDOW_GET_CLASS(self);
  klass->create_handle(self, create_params);
}
