#include "cern/ui/control.h"
#include "cern/drawing/color.h"
#include "cern/drawing/font.h"
#include "cern/drawing/rectangle.h"
#include "cern/drawing/region.h"
#include "cern/drawing/size.h"
#include "cern/ui/ambient_properties.h"
#include "cern/ui/common_properties.h"
#include "cern/ui/bounds_specified.h"
#include "cern/ui/padding.h"
#include "cern/drawing/internal/font_handle_wrapper.h"
#include "cern/ui/component_model/cancel_event_args.h"
#include "cern/ui/component_model/control_event_args.h"
#include "cern/ui/component_model/event_args.h"
#include "cern/ui/component_model/icomponent.h"
#include "cern/ui/component_model/invalidate_event_args.h"
#include "cern/ui/context_menu.h"
#include "cern/ui/control_collection.h"
#include "cern/ui/control_styles.h"
#include "cern/ui/create_params.h"
#include "cern/ui/cursor.h"
#include "cern/ui/image_layout.h"
#include "cern/ui/layout/iarranged_element.h"
#include "cern/ui/message.h"
#include "cern/ui/native_window.h"
#include "cern/ui/padding.h"
#include "cern/ui/property_store.h"
#include "cern/ui/right_to_left.h"
#include "cern/ui/control_native_window.h"
#include <Windows.h>
#include <glib-object.h>
#include <glib.h>
#include <glibconfig.h>
#include <gobject/gmarshal.h>
#include <minwindef.h>
#include <windef.h>
#include <wingdi.h>
#include <winver.h>

/*
 * @brief Signal with end _CHANGED 
 * */
#define SC(X) SIGNAL_ ## X ## _CHANGED

/*
 * @brief Signal normal, not like SC signal changed.
 * */
#define SN(X) SIGNAL_ ## X

enum {
  SN(0),
  SC(AUTOSIZE),
  SC(BACK_COLOR),
  SC(BACKGROUND_IMAGE),
  SC(BACKGROUND_IMAGE_LAYOUT),
  SC(BINDING_CONTEXT),
  SC(CAUSES_VALIDATION),
  SC(CLIENT_SIZE),
  SC(CONTEXT_MENU),
  SC(CONTEXT_MENU_STRIP),
  SC(CURSOR),
  SC(DOCK),
  SC(ENABLED),
  SC(FONT),
  SC(FORE_COLOR),
  SC(LOCATION),
  SC(MARGIN),
  SC(REGION),
  SC(RIGHT_TO_LEFT),
  SC(SIZE),
  SC(TAB_INDEX),
  SC(TAB_STOP),
  SC(TEXT),
  SC(VISIBLE),
  SN(CLICK),
  SN(CONTROL_ADDED),
  SN(CONTROL_REMOVED),
  SN(DRAG_DROP),
  SN(DRAG_ENTER),
  SN(DRAG_OVER),
  SN(DRAG_LEAVE),
  SN(GIVE_FEEDBACK),
  SN(HANDLE_CREATED),
  SN(HANDLE_DESTROYED),
  SN(HELP_REQUESTED),
  SN(INVALIDATED),
  SC(PADDING),
  SN(PAINT),
  SN(QUERY_CONTINUE_DRAG),
  SN(QUERY_ACCESSIBILITY_HELP),
  SN(DOUBLE_CLICK),
  SN(ENTER),
  SN(GOT_FOCUS),
  SN(KEY_DOWN),
  SN(KEY_PRESS),
  SN(KEY_UP),
  SN(LAYOUT),
  SN(LEAVE),
  SN(LOST_FOCUS),
  SN(MOUSE_CLICK),
  SN(MOUSE_DOUBLE_CLICK),
  SN(MOUSE_CAPTURE_CHANGED),
  SN(MOUSE_DOWN),
  SN(MOUSE_ENTER),
  SN(MOUSE_LEAVE),
  SN(DPI_CHANGED_BEFORE_PARENT),
  SN(DPI_CHANGED_AFTER_PARENT),
  SN(MOUSE_OVER),
  SN(MOUSE_MOVE),
  SN(MOUSE_UP),
  SN(MOUSE_WHEEL),
  SN(MOVE),
  SN(PREVIEW_KEY_DOWN),
  SN(RESIZE),
  SN(CHANGE_UI_CUES),
  SC(STYLE),
  SC(SYSTEM_COLORS),
  SN(VALIDATING),
  SN(VALIDATED),
  SC(PARENT),
  SN(N)
};

static
guint32
signals[SIGNAL_N] = { 0 };

#define CERN_UISTATE_FOCUS_CUES_MASK      (0x000F)
#define CERN_UISTATE_FOCUS_CUES_HIDDES    (0x0001)
#define CERN_UISTATE_FOCUS_CUES_SHOW      (0x0002)
#define CERN_UISTATE_KEYBOARD_CUES_MASK   (0x00F0)
#define CERN_UISTATE_KEYBOARD_CUES_HIDDEN (0x0010)
#define CERN_UISTATE_KEYBOARD_CUES_SHOW   (0x0020)

static gint32 CernPropName = 0;
static gint32 CernPropBackBrush = 0;
static gint32 CernPropFontHeight = 0;
static gint32 CernPropCurrentAmbientFont = 0;

static gint32 CernPropControlsCollection = 0;
static gint32 CernPropBackColor = 0;
static gint32 CernPropForeColor = 0;
static gint32 CernPropFont = 0;

static gint32 CernPropBackgroundImage = 0;
static gint32 CernPropFontHandleWrapper = 0;
static gint32 CernPropUserData = 0;
static gint32 CernPropContextMenu = 0;

static gint32 CernPropCursor = 0;
static gint32 CernPropRegion = 0;
static gint32 CernPropRightToLeft = 0;

static gint32 CernPropBindings = 0;
static gint32 CernPropBindingManager = 0;
static gint32 CernPropAccessibleDefaultActionDescription = 0;
static gint32 CernPropAccessibleDescription = 0;

static gint32 CernPropAccessibility = 0;
static gint32 CernPropNcAccessibility = 0;
static gint32 CernPropAccessibleName = 0;
static gint32 CernPropAccessibleRole = 0;

static gint32 CernPropPaintingException = 0;
static gint32 CernPropActiveXImpl = 0;
static gint32 CernPropControlVersionInfo = 0;
static gint32 CernPropBackgroundImageLayout = 0;

static gint32 CernPropAccessibleHelpProvider = 0;
static gint32 CernPropContextMenuStrip = 0;
static gint32 CernPropAutoScrollOffset = 0;
static gint32 CernPropUseCompatibleTextRendering = 0;

static gint32 CernPropImeWmCharsToIgnore = 0;
static gint32 CernPropImeMode = 0;
static gint32 CernPropDisableImeModeChangedCount = 0;
static gint32 CernPropLastCanEnableIme = 0;

static gint32 CernPropCacheTextCount = 0;
static gint32 CernPropCacheTextField = 0;
static gint32 CernPropAmbientPropertiesService = 0;

static gboolean CernNeedToLoadComCtl = TRUE;
static gboolean UseCompatibleTextRenderingDefault = TRUE;

static
guint32 CernMessageThreadCallback = 0;

static
void
cern_control_win32_window_interface_init(CernWin32WindowInterface *iface);

static
void
cern_control_required_scaling_set(CernControl *self, gboolean enabled);

static
gboolean
cern_control_required_scaling_get(CernControl *self);

static
gboolean
cern_control_get_resize_redraw_real(CernControl *self);

static
void
cern_control_set_resize_redrad_real(CernControl *self, gboolean value);

static
void *
cern_control_prop_init(void *prop) {
  CernPropName = cern_property_store_create_key();
  CernPropBackBrush = cern_property_store_create_key();
  CernPropFontHeight = cern_property_store_create_key();
  CernPropCurrentAmbientFont = cern_property_store_create_key();

  CernPropControlsCollection = cern_property_store_create_key();
  CernPropBackColor = cern_property_store_create_key();
  CernPropForeColor = cern_property_store_create_key();
  CernPropFont = cern_property_store_create_key();

  CernPropBackgroundImage = cern_property_store_create_key();
  CernPropFontHandleWrapper = cern_property_store_create_key();
  CernPropUserData = cern_property_store_create_key();
  CernPropContextMenu = cern_property_store_create_key();

  CernPropCursor = cern_property_store_create_key();
  CernPropRegion = cern_property_store_create_key();
  CernPropRightToLeft = cern_property_store_create_key();

  CernPropBindings = cern_property_store_create_key();
  CernPropBindingManager = cern_property_store_create_key();
  CernPropAccessibleDefaultActionDescription = cern_property_store_create_key();
  CernPropAccessibleDescription = cern_property_store_create_key();

  CernPropAccessibility = cern_property_store_create_key();
  CernPropNcAccessibility = cern_property_store_create_key();
  CernPropAccessibleName = cern_property_store_create_key();
  CernPropAccessibleRole = cern_property_store_create_key();

  CernPropPaintingException = cern_property_store_create_key();
  CernPropActiveXImpl = cern_property_store_create_key();
  CernPropControlVersionInfo = cern_property_store_create_key();
  CernPropBackgroundImageLayout = cern_property_store_create_key();

  CernPropAccessibleHelpProvider = cern_property_store_create_key();
  CernPropContextMenuStrip = cern_property_store_create_key();
  CernPropAutoScrollOffset = cern_property_store_create_key();
  CernPropUseCompatibleTextRendering = cern_property_store_create_key();

  CernPropImeWmCharsToIgnore = cern_property_store_create_key();
  CernPropImeMode = cern_property_store_create_key();
  CernPropDisableImeModeChangedCount = cern_property_store_create_key();
  CernPropLastCanEnableIme = cern_property_store_create_key();

  CernPropCacheTextCount = cern_property_store_create_key();
  CernPropCacheTextField = cern_property_store_create_key();
  CernPropAmbientPropertiesService = cern_property_store_create_key();
  return NULL;
}

static GOnce prop_init_once = G_ONCE_INIT;

typedef struct _CernControlPrivate {
  CernNativeWindow *window;

  CernControl *parent;
  CernControl *reflect_parent;
  CernCreateParams *create_params;

  gint32 x;
  gint32 y;
  gint32 width;
  gint32 height;
  gint32 client_width;
  gint32 client_height;
  gint32 state;
  gint32 state2;
  CernControlStyles control_style;
  gint32 tab_index;
  gchar *text;
  gchar *window_text;
  guint8 layout_suspend_count;
  guint8 required_scaling;
  CernPropertyStore *property_store;
  TRACKMOUSEEVENT *track_mouse_event;
  guint16 update_count;
  GQueue *thread_callback_list;
  gint32 device_dpi;
  gint32 ui_cues_state;
} CernControlPrivate;

G_DEFINE_TYPE_WITH_CODE(CernControl, cern_control,
  CERN_TYPE_COMPONENT_MODEL_COMPONENT,
  G_ADD_PRIVATE(CernControl)
  G_IMPLEMENT_INTERFACE(CERN_TYPE_WIN32_WINDOW,
    cern_control_win32_window_interface_init));

static
gpointer
cern_control_load_library_from_system_path_if_available(char const *library_name) {
  HANDLE module = NULL;
  HANDLE kernel32 = GetModuleHandle("kernel32.dll");

  if (kernel32 != NULL) {
    if (GetProcAddress(kernel32, "AddDllDirectory") != NULL) {
      module = LoadLibraryEx(library_name, NULL, LOAD_LIBRARY_SEARCH_SYSTEM32);
    } else {
      module = LoadLibrary(library_name);
    }
  }

  return module;
}

static
void
real_cern_control_set_allow_drop(CernControl *self, gboolean value) {
  if (cern_control_get_state(self, CERN_CONTROL_STATE_ALLOWDROP) != value) {
    cern_control_set_state(self, CERN_CONTROL_STATE_ALLOWDROP, value);
  }

  if (cern_control_get_is_handle_created(self)) {
    if (!cern_control_get_accept_drops(self, value)) {
      cern_control_set_state(self, CERN_CONTROL_STATE_ALLOWDROP, !value);
    }
  }
}

static
gboolean
real_cern_control_get_allow_drop(CernControl *self) {
  return
    cern_control_get_state(self, CERN_CONTROL_STATE_ALLOWDROP);
}

static
CernAnchorStyles
real_cern_control_get_anchor(CernControl *self) {
  return cern_default_layout_get_anchor(self);
}

static
void
real_cern_control_set_anchor(CernControl *self, CernAnchorStyles value) {
  cern_default_layout_set_anchor(self, value);
}

static
gboolean
real_cern_control_get_auto_size(CernControl *self) {
  CernIArrangedElement *arranged_element
    = CERN_IARRANGED_ELEMENT(self);

  return
    cern_common_properties_get_auto_size_mode(arranged_element);
}

static
void
real_cern_control_set_auto_size(CernControl *self, gboolean value) {
  CernIArrangedElement *arranged_element
    = CERN_IARRANGED_ELEMENT(self);

  if (value != cern_control_get_auto_size(self)) {
    cern_common_properties_set_auto_size_mode(arranged_element, value);

    if (cern_control_get_parent_internal(self) != NULL) {
      CernControl *parent = cern_control_get_parent_internal(self);
      CernLayoutEngine *layout_engine = cern_control_get_layout_engine(parent);
      if (value && layout_engine == cern_default_layout_get_instance()) {
        cern_layout_engine_init_layout(layout_engine, G_OBJECT(parent),
                                       CernBoundsSpecified_Size);
      }

      cern_layout_transaction_do_layout(CERN_IARRANGED_ELEMENT(parent),
                                        CERN_IARRANGED_ELEMENT(self),
                                        cern_property_names_auto_size());
    }

    CernEventArgs *args = cern_event_args_new();
    cern_control_on_autosize_changed(self, args);
    g_clear_object(&args);
  }
}

static
CernPoint
real_cern_control_get_auto_scroll_offset(CernControl *self) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);
  CernPropertyStore *property_store = priv->property_store;
  gboolean contains =
    cern_property_store_contains_object(property_store,
                                        CernPropAutoScrollOffset);
  if (contains) {

    GObject *obj
      = cern_property_store_get_object(property_store, CernPropAutoScrollOffset);
    CernPoint *pt = g_object_get_data(obj, "CernPoint");
    return (*pt);
  }

  return (CernPoint) { 0 };
}

static
void
real_cern_control_set_auto_scroll_offset(CernControl *self, CernPoint *value) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);
  CernPropertyStore *property_store = priv->property_store;
  CernPoint pt = cern_control_get_auto_scroll_offset(self);
  if (!cern_point_is_equals(&pt, value)) {
    GObject *obj
      = cern_property_store_get_object(property_store, CernPropAutoScrollOffset);
    CernPoint *pt = g_object_get_data(obj, "CernPoint");
    cern_point_free(pt);
    g_object_set_data(obj, "CernPoint", cern_point_copy(value));
  }
}

static
CernLayoutEngine *
real_cern_control_get_layout_engine(CernControl *self) {
  return cern_default_layout_get_instance();
}

static
void
cern_control_win32_window_interface_init(CernWin32WindowInterface *iface) {
  // iface->get_native = cern_control_get_native;
}

static
void
cern_control_init(CernControl *self) {
  CernControlPrivate *priv;

  g_once((&prop_init_once), cern_control_prop_init, NULL);

  priv = cern_control_get_instance_private(self);

  priv->property_store = cern_property_store_new();
  priv->device_dpi = cern_dpi_helper_get_device_dpi();
  priv->window = cern_native_window_new_with_control(self);
  cern_control_required_scaling_set(self, TRUE);
  priv->state
    = CERN_CONTROL_STATE_VISIBLE
        | CERN_CONTROL_STATE_ENABLED
        | CERN_CONTROL_STATE_TABSTOP
        | CERN_CONTROL_STATE_CAUSESVALIDATION;

  priv->state2 = CERN_CONTROL_STATE2_INTERESTEDINUSERPREFERENCECHANGED;

  CernControlStyles styles
    = CernControlStyles_AllPaintingInWmPaint
        | CernControlStyles_UserPaint
        | CernControlStyles_StandardClick
        | CernControlStyles_StandardDoubleClick
        | CernControlStyles_UseTextForAccessibility
        | CernControlStyles_Selectable;

  cern_control_set_style(self, styles, TRUE);
  CernPadding default_margin = cern_control_get_default_margin(self);
  CernPadding margin = cern_control_get_margin(self);

  if (!cern_padding_equals(&default_margin, &margin)) {
    cern_control_set_margin(self, &default_margin);
  }
  
  CernPadding def_padding = cern_common_properties_get_default_margin();

  cern_control_set_default_margin(CERN_IARRANGED_ELEMENT(self), &def_padding);
}

static
gboolean
cern_control_is_valid_back_color(CernControl *self, CernColor *color) {
  if (!cern_color_is_empty(color)
      && !cern_control_get_style(self, CernControlStyles_SupportsTransparentBackColor)
      && (cern_color_get_alpha(color) < 255)) {
    return FALSE;
  }

  return TRUE;
}

static
CernColor
real_cern_control_get_back_color(CernControl *self) {
  CernColor color = cern_control_get_raw_back_color(self);
  if (cern_color_is_empty(&color)) {
    return color;
  }

  CernControl *parent = cern_control_get_parent_internal(self);

  if (parent != NULL
      && cern_control_get_can_access_properties(parent)) {
    color = cern_control_get_back_color(parent);
    g_clear_object(&parent);

    if (cern_control_is_valid_back_color(self, &color)) {
      return color;
    }
  }

  if (cern_color_is_empty(&color)) {
    CernAmbientProperties *ambient
      = cern_control_get_ambient_properties_service(self);

    if (ambient != NULL) {
      color = cern_ambient_properties_get_back_color(ambient);
    }
  }

  if (!cern_color_is_empty(&color)
      && cern_control_is_valid_back_color(self, &color)) {
    return color;
  }

  return cern_control_get_default_back_color();
}

static
void
real_cern_control_set_back_color(CernControl *self, CernColor *value) {
  CernColor empty = { 0 };
  CernColor color = { 0 };
  CernColor tmp = { 0 };
  CernPropertyStore *store = NULL;
  CernControlPrivate *priv = cern_control_get_instance_private(self);

  if (!cern_color_equals(&empty, value)
      && !cern_control_get_style(self, CernControlStyles_SupportsTransparentBackColor)
      && cern_color_get_alpha(value) < 255) {
    g_warning("BackColor cannot be transparent when SupportsTransparentBackColor is set.");
    return;
  }

  color = cern_control_get_back_color(self);
  store = priv->property_store;

  if (!cern_color_is_empty(value)
      || cern_property_store_contains_object(store, CernPropBackColor)) {
    cern_property_store_set_color(store, CernPropBackColor, value);
  }

  tmp = cern_control_get_back_color(self);
  if (!cern_color_equals(&color, &tmp)) {
    CernEventArgs *args = cern_event_args_new();
    cern_control_on_background_color_changed(self, args);
    g_clear_object(&args);
  }
}

static
CernImage *
real_cern_control_get_background_image(CernControl *self) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);
  CernPropertyStore *store = priv->property_store;

  CernImage *image
    = cern_property_store_get_object(store, CernPropBackgroundImage);

  if (image != NULL) {
    return g_object_ref(image);
  }

  return image;
}

static
void
real_cern_control_set_background_image(CernControl *self, CernImage *value) {
  CernImage *image = cern_control_get_background_image(self);
  CernControlPrivate *priv = cern_control_get_instance_private(self);
  CernPropertyStore *store = priv->property_store;
  CernEventArgs *args = NULL;

  if (image != NULL) {
    if (value != image) {
      cern_property_store_set_object(store, CernPropBackgroundImage, value);
      g_clear_object(&image);
      args = cern_event_args_new();
      cern_control_on_background_image_changed(self, args);
      g_clear_object(&args);
    }
  }
}

static
CernImageLayout
real_cern_control_get_background_image_layout(CernControl *self) {
  CernPropertyStore *store = cern_control_get_properties(self);
  gboolean found = cern_property_store_contains_object(store, CernPropBackgroundImageLayout);

  if (!found) {
    return CernImageLayout_Tile;
  } else {
    return 
      (CernImageLayout) cern_property_store_get_integer(store, CernPropBackgroundImageLayout);
  }
}

static
void
real_cern_control_set_background_image_layout(CernControl *self, CernImageLayout value) {
  if (cern_control_get_background_image_layout(self) != value) {
    if (!cern_client_utils_is_valid_enum(value, (gint32) CernImageLayout_None, (gint32) CernImageLayout_Zoom)) {
      g_critical("%s(...): Imvalid enum value", __func__);
      return;
    }

    if (value == CernImageLayout_Center
        || value == CernImageLayout_Zoom
        || value == CernImageLayout_Stretch) {
      cern_control_set_style(self, CernControlStyles_ResizeRedraw, TRUE);
      if (cern_control_paint_get_is_image_transparent(cern_control_get_background_image(self))) {
        cern_control_set_double_buffered(self, TRUE);
      }
    }

    CernPropertyStore *store = cern_control_get_properties(self);
    cern_property_store_set_integer(store, CernPropBackgroundImageLayout, (gint32) value);

    CernEventArgs *args = cern_event_args_new();
    cern_control_on_background_image_layout_changed(self, args);
    g_clear_object(&args);
  }
}

static
CernBindingContext *
real_cern_control_get_binding_context(CernControl *self) {
  return cern_control_get_binding_context_internal(self);
}

static
void
real_cern_control_set_binding_context(CernControl *self, CernBindingContext *value) {
  CernBindingContext *old_context
    = cern_control_get_binding_context_internal(self);
  cern_control_set_binding_context_internal(self, value);
  g_clear_object(&old_context);
}

static
gboolean
real_cern_control_get_can_access_properties(CernControl *self) {
  (void) self; /* calm down. */
  return TRUE;
}

static
CernContextMenu *
real_cern_control_get_context_menu(CernControl *self) {
  CernPropertyStore *store = cern_control_get_properties(self);
  CernContextMenu *context_menu
    = CERN_CONTEXT_MENU(cern_property_store_get_object(store, CernPropContextMenu));

  return g_object_ref(context_menu);
}

static
void
real_cern_control_set_context_menu(CernControl *self, CernContextMenu *value) {
  CernPropertyStore *store = cern_control_get_properties(self);
  CernContextMenu *old_value = cern_property_store_get_object(store, CernPropContextMenu);

  if (old_value != value) {
    cern_property_store_set_object(store, CernPropContextMenu, value);
    CernEventArgs *args = cern_event_args_new();
    cern_control_on_context_menu_changed(self, args);
    g_clear_object(&args);
  }
}

static
CernContextMenuStrip *
real_cern_control_get_context_menu_strip(CernControl *self) {
  CernPropertyStore *store
    = cern_control_get_properties(self);

  CernContextMenuStrip *menu_strip
    = cern_property_store_get_object(store, CernPropContextMenuStrip);

  return g_object_ref(menu_strip);
}

static
void
real_cern_control_set_context_menu_strip(CernControl *self, CernContextMenuStrip *value) {
  CernPropertyStore *store
    = cern_control_get_properties(self);

  CernContextMenuStrip *old_value
    = cern_property_store_get_object(store, CernPropContextMenuStrip);

  if (old_value != NULL) {
    cern_property_store_set_object(store, CernPropContextMenuStrip, value);
    g_clear_object(&old_value);

    CernEventArgs *args = cern_event_args_new();
    cern_control_on_context_menu_strip_changed(self, args);
    g_clear_object(&args);
  }
}

static
CernCreateParams *
real_cern_control_get_create_params(CernControl *self) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);

  if (CernNeedToLoadComCtl) {
    if (GetModuleHandleA("comctl32.dll")
        || cern_control_load_library_from_system_path_if_available("comctl32.dll")) {
      CernNeedToLoadComCtl = FALSE;
    }
  }

  if (priv->create_params == NULL) {
    priv->create_params = cern_create_params_new_empty();
  }

  CernCreateParams *cp = priv->create_params;
  cern_create_params_set_style(cp, 0);
  cern_create_params_set_ex_style(cp, 0);
  cern_create_params_set_title(cp, priv->text);

  cern_create_params_set_x(cp, priv->x);
  cern_create_params_set_y(cp, priv->y);
  cern_create_params_set_width(cp, priv->width);
  cern_create_params_set_height(cp, priv->height);

  cern_create_params_set_style(cp, WS_CLIPCHILDREN);

  if (cern_control_get_style(self, CernControlStyles_ContainerControl)) {
    gint32 style = cern_create_params_get_ex_style(cp);
    style |= WS_EX_CONTROLPARENT;
    cern_create_params_set_ex_style(cp, style);
  }

  cern_create_params_set_class_style(cp, CS_DBLCLKS);

  if ((priv->state & CERN_CONTROL_STATE_TOPLEVEL) == 0) {
    CernControl *parent = cern_control_get_parent_internal(self);
    cern_create_params_set_parent(cp, !priv->parent ? NULL : cern_control_get_internal_handle(parent));
    cern_create_params_set_style(cp, cern_create_params_get_style(cp) | WS_CHILD | WS_CLIPSIBLINGS);
  } else {
    cern_create_params_set_parent(cp, NULL);
  }

  if (priv->state & CERN_CONTROL_STATE_TABSTOP) {
    cern_create_params_set_style(cp, cern_create_params_get_style(cp) | WS_TABSTOP);
  }

  if (priv->state & CERN_CONTROL_STATE_VISIBLE) {
    cern_create_params_set_style(cp, cern_create_params_get_style(cp) | WS_VISIBLE);
  }

  if (!cern_control_get_enabled(self)) {
    cern_create_params_set_style(cp, cern_create_params_get_style(cp) | WS_DISABLED);
  }

  if (cern_control_get_right_to_left(self) == CernRightToLeft_Yes) {
    gint32 ex_style = cern_create_params_get_ex_style(cp);
    ex_style |= (WS_EX_RTLREADING | WS_EX_RIGHT | WS_EX_LEFTSCROLLBAR);
    cern_create_params_set_ex_style(cp, ex_style);
  }

  return cp;
}

static
void
real_cern_control_notify_validation_result(CernControl *self, CernCancelEventArgs *args) {
  cern_control_set_validation_cancelled(self, cern_cancel_event_args_get_cancel(args));
}

static
CernCursor *
real_cern_control_get_cursor(CernControl *self) {
  if (cern_control_get_state(self, CERN_CONTROL_STATE_USEWAITCURSOR)) {
    return cern_cursors_get_wait_cursor();
  }

  CernPropertyStore *store = cern_control_get_properties(self);

  CernCursor *cursor = cern_property_store_get_object(store, CernPropCursor);

  if (cursor != NULL) {
    return cursor;
  }


}

static
gboolean
real_cern_control_can_raise_events(CernComponentModelComponent *self) {
  (void) self; /* calm down. */
  return TRUE;
}

static
gboolean
real_cern_control_get_double_buffered(CernControl *self) {
  return cern_control_get_style(self, CernControlStyles_OptimizedDoubleBuffer);
}

static
void
real_cern_control_set_double_buffered(CernControl *self, gboolean value) {
  if (value != cern_control_get_double_buffered(self)) {
    if (TRUE == value) {
      cern_control_set_style(self,
                             CernControlStyles_OptimizedDoubleBuffer
                              | CernControlStyles_AllPaintingInWmPaint,
                             value);
    } else {
      cern_control_set_style(self, CernControlStyles_OptimizedDoubleBuffer, value);
    }
  }
}

static
CernFont *
cern_control_get_parent_font(CernControl *self) {
  if (cern_control_get_parent_internal(self) != NULL
      && cern_control_get_can_access_properties(cern_control_get_parent_internal(self))) {
    return cern_control_get_font(cern_control_get_parent_internal(self));
  }

  return NULL;
}

static
gboolean
real_cern_control_get_focused(CernControl *self) {
  return
    cern_control_get_is_handle_created(self)
      && GetFocus() == (HWND) cern_control_get_handle(self);
}

static
CernFont *
real_cern_control_get_font(CernControl *self) {
  CernPropertyStore *store = cern_control_get_properties(self);
  CernFont *font = cern_property_store_get_object(store, CernPropFont);

  if (font != NULL) {
    return font;
  }

  CernFont *f = cern_control_get_parent_font(self);

  if (f != NULL) {
    return f;
  }

  CernAmbientProperties *ambient
    = cern_control_get_ambient_properties_service(self);

  if (ambient != NULL && cern_ambient_properties_get_font(ambient) != NULL) {
    return cern_ambient_properties_get_font(ambient);
  }

  return cern_control_get_default_font();
}

static
void
real_cern_control_set_font(CernControl *self, CernFont *value) {
  CernPropertyStore *store
    = cern_control_get_properties(self);
  CernFont *local
    = cern_property_store_get_object(store, CernPropFont);
  CernFont *resolved = local;

  gboolean local_changed = FALSE;

  if (value == NULL) {
    if (local != NULL) {
      local_changed = TRUE;
    }
  } else {
    if (local == NULL) {
      local_changed = TRUE;
    } else {
      local_changed = cern_font_is_equals(value, local);
    }
  }

  if (local_changed) {
    cern_property_store_set_object(store, CernPropFont, value);

    if (!cern_font_is_equals(resolved, value)) {
      cern_control_dispose_font_handle(self);

      if (cern_property_store_contains_integer(store, CernPropFontHeight)) {
        cern_property_store_set_integer(store, CernPropFontHeight,
                                        (value == NULL) ? -1 ? cern_font_get_height(value));
      }

      CernLayoutTransation *lt
        = cern_layout_transacation_new(cern_control_get_parent_internal(self),
                                       CERN_IARRANGED_ELEMENT(sefl),
                                       cern_property_names_font());
      CernEventArgs *args = cern_event_args_new();
      cern_control_on_font_changed(self, args);

      g_clear_object(&args);
      g_clear_object(&lt);
    } else if (cern_control_get_is_handle_created(self)
               && !cern_control_get_style(self, CernControlStyles_UserPaint)) {
      cern_control_dispose_font_handle(self);
      cern_control_set_window_font(self);
    }
  }
}

static
CernColor
real_cern_control_get_fore_color(CernControl *self) {
  CernPropertyStore *store = cern_control_get_properties(self);

  CernColor color = cern_property_store_get_color(store, CernPropForeColor);

  if (!cern_color_is_empty(&color)) {
    return color;
  }

  CernControl *p = cern_control_get_parent_internal(self);

  if (p != NULL && cern_control_get_can_access_properties(p)) {
    CernPropertyStore *parent_store = cern_control_get_properties(p);
    return cern_control_get_fore_color(p);
  }

  CernAmbientProperties *ambient
    = cern_control_get_ambient_properties_service(self);

  if (ambient != NULL) {
    color = cern_ambient_properties_get_fore_color(ambient);
  }

  if (!cern_color_is_empty(&color)) {
    return color;
  }

  return cern_control_get_default_fore_color();
}

static
void
real_cern_control_set_fore_color(CernControl *self, CernColor *value) {
  CernPropertyStore *store = cern_control_get_properties(self);
  CernColor c = cern_control_get_fore_color(self);

  if (!cern_color_is_empty(value)
      || cern_property_store_contains_object(store, CernPropForeColor)) {
    cern_property_store_set_color(store, CernPropForeColor, value);
  }

  if (!cern_color_equals(&c, value)) {
    CernEventArgs *args = cern_event_args_new();
    cern_control_on_fore_color_changed(self, args);
    g_clear_object(&args);
  }
}

static
CernSize
real_cern_control_get_preferred_size(CernControl *self,
                                     CernSize *proposed_size) {
  CernSize pref_size = { 0 };
  CernSize prop_size = *proposed_size;

  if (cern_control_get_state(self, CERN_CONTROL_STATE_DISPOSED
                                    | CERN_CONTROL_STATE_DISPOSING)) {
    pref_size = cern_common_properties_x_get_preferred_size_cached(self);
  } else {
    prop_size = cern_layout_utils_convert_zero_to_unbounded(proposed_size);

    prop_size = cern_control_apply_size_constraints_size(self, &prop_size);

    CernSize cached_size
        = cern_common_properties_x_get_preferred_size_cached(self);

      CernSize max_size = cern_layout_utils_max_size();

    if (cern_control_get_state2(self,
                                CERN_CONTROL_STATE2_USEPREFERREDSIZECACHE)) {

      if ((cached_size.height != 0
           && cached_size.width != 0)
          && cern_size_equals(proposed_size, &max_size)) {
        return cached_size;
      }
    }

    cern_control_set_cache_text_internal(self, TRUE);

    pref_size = cern_control_get_preferred_size_core(self, proposed_size);

    cern_control_set_cache_text_internal(self, FALSE);

    pref_size = cern_control_apply_size_constraints_size(self, &pref_size);

    if (cern_control_get_state2(self, CERN_CONTROL_STATE2_USEPREFERREDSIZECACHE)
        && cern_size_equals(proposed_size, &max_size)) {
      cern_common_properties_x_get_preferred_size_cached(self, &pref_size);
    }
  }

  return pref_size;
}

static
CernSize
real_cern_control_get_preferred_size_core(CernControl *self,
                                          CernSize *proposed_size) {
  CernRectangle rect
    = cern_common_properties_get_specified_bounds(self);
  return cern_rectangle_get_size(&rect);
}

static
gboolean
real_cern_control_get_has_children(CernControl *self) {
  CernPropertyStore *store = cern_control_get_properties(self);

  CernControlCollection *controls
    = cern_property_store_get_object(store, CernPropControlsCollection);

  return
    controls != NULL && cern_control_collection_get_count(controls) > 0;
}

static
gboolean
real_cern_control_get_is_container_control(CernControl *self) {
  (void) self;
  return FALSE;
}

static
CernSize
real_cern_control_get_maximum_size(CernControl *self) {
  CernSize def_max_size
    = cern_control_get_default_maximum_size(self);
  return
    cern_common_properties_get_maximum_size(CERN_IARRANGED_ELEMENT(self),
                                            &def_max_size);
}

static
void
real_cern_control_set_maximum_size(CernControl *self, CernSize *value) {
  CernSize empty = { 0 };
  CernSize maximum = cern_control_get_maximum_size(self);
  if (cern_size_equals(value, &empty)) {
    cern_common_properties_clear_maximum_size(CERN_IARRANGED_ELEMENT(self));
  } else if (!cern_size_equals(value, &maximum)) {
    cern_common_properties_set_maximum_size(CERN_IARRANGED_ELEMENT(self), value);
  }
}

static
CernSize
real_cern_control_get_minimum_size(CernControl *self) {
  CernSize min_size = cern_control_get_default_minimum_size(self);
  return
    cern_common_properties_get_minimum_size(CERN_IARRANGED_ELEMENT(self),
                                            &min_size);
}

static
void
real_cern_control_set_minimum_size(CernControl *self, CernSize *value) {
  CernSize min_size = cern_control_get_minimum_size(self);

  if (!cern_size_equals(value, &min_size)) {
    cern_common_properties_set_minimum_size(CERN_IARRANGED_ELEMENT(self), value);
  }
}

static
CernControl *
real_cern_control_get_parent_internal(CernControl *self) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);
  return priv->parent;
}

static
void
real_cern_control_set_parent_internal(CernControl *self, CernControl *value) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);

  if (priv->parent != value) {
    CernControlCollection *controls = cern_control_get_controls(value);
    if (value != NULL) {
      cern_control_collection_add(controls, self);
    } else {
      cern_control_collection_remove(controls, self);
    }
  }
}

static
void
real_cern_control_add_reflect_child(CernControl *self) {
  (void) self;
}

static
void
real_cern_control_remove_reflect_child(CernControl *self) {
  (void) self;
}

static
gboolean
real_cern_control_get_render_transparency_with_visual_styles(CernControl *self) {
  (void) self;
  return FALSE;
}

static
CernRightToLeft
real_cern_control_get_right_to_left(CernControl *self) {
  CernPropertyStore *store = cern_control_get_properties(self);
  gboolean found = FALSE;

  gint32 right_to_left
    = cern_property_store_try_get_integer(store, CernPropRightToLeft, &found);

  if (!found) {
    right_to_left = (gint32) CernRightToLeft_Inherit;
  }

  if ((CernRightToLeft) right_to_left == CernRightToLeft_Inherit) {
    CernControl *ctl = cern_control_get_parent(self);

    if (ctl != NULL) {
      right_to_left = (gint32) cern_control_get_right(ctl);
    } else {
      right_to_left = cern_control_get_default_right_to_left();
    }
  }

  return (CernRightToLeft) right_to_left;
}

static
void
real_cern_control_set_right_to_left(CernControl *self, CernRightToLeft value) {
  if (!cern_client_utils_is_valid_enum(value, CernRightToLeft_No, CernRightToLeft_Inherit)) {
    g_critical("%s(...): Invalid CernRightToLeft value.");
    return;
  }

  CernRightToLeft old_value = cern_control_get_right_to_left(self);
  CernPropertyStore *store = cern_control_get_properties(self);

  if (cern_property_store_contains_integer(store, CernPropRightToLeft)
      || value != CernRightToLeft_Inherit) {
    cern_property_store_set_integer(store, CernPropRightToLeft, (gint32) value);
  }

  if (old_value !=  value) {
    CernLayoutTransaction *ltr
      = cern_layout_transacation_new(self, CERN_IARRANGED_ELEMENT(self),
                                     cern_property_names_right_to_left());
    CernEventArgs *args = cern_event_args_new();

    cern_control_on_parent_right_to_left_changed(self, args);

    g_clear_object(&args);
    g_clear_object(&ltr);
  }
}

static
gboolean
real_cern_control_get_scale_children(CernControl *self) {
  return TRUE;
}

static
gchar *
real_cern_control_get_text(CernControl *self) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);
  if (cern_control_get_cache_text_internal(self)) {
    return priv->text == NULL ? "" : priv->text;
  } else {
    return cern_control_get_window_text(self);
  }
}

static
void
real_cern_control_set_text(CernControl *self, gchar *value) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);

  if (value == NULL) {
    value = "";
  }

  if (g_strcmp0(value, priv->text) == 0) {
    return;
  }

  if (cern_control_get_cache_text_internal(self)) {
    g_free(priv->text);
    priv->text = g_strdup(value);
  }

  cern_control_set_window_text(self, priv->text);

  CernEventArgs *args = cern_event_args_new();
  cern_control_on_text_changed(self, args);
  g_clear_object(&args);
}

static
gboolean
real_cern_control_get_show_keyboard_cues(CernControl *self) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);
  if (!cern_control_get_is_handle_created(self)
      || cern_component_get_design_mode(self)) {
    return TRUE;
  }

  if ((priv->ui_cues_state & CERN_UISTATE_KEYBOARD_CUES_MASK) == 0) {
    if (cern_control_system_information_menu_access_keys_underlined()) {
      priv->ui_cues_state |= CERN_UISTATE_KEYBOARD_CUES_SHOW;
    } else {
      gint32 action_mask
        = (UISF_HIDEACCEL
            | cern_accessibility_improvements_level1() ? 0 : UISF_HIDEFOCUS) << 16;
      priv->ui_cues_state |= CERN_UISTATE_KEYBOARD_CUES_HIDDEN;

      HWND handle
        = (HWND) cern_control_get_handle(cern_control_get_top_most_parent(self));

      SendMessage(handle, WM_CHANGEUISTATE, (WPARAM) (action_mask | UIS_SET), (LPARAM) 0);
    }
  }

  return
    (priv->ui_cues_state
      & CERN_UISTATE_KEYBOARD_CUES_MASK) == CERN_UISTATE_KEYBOARD_CUES_SHOW;
}

static
gboolean
real_cern_control_get_show_focus_cues(CernControl *self) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);

  if (!cern_control_get_is_handle_created(self)) {
    return TRUE;
  }

  if ((priv.ui_cues_state & CERN_UISTATE_FOCUS_CUES_MASK) == 0) {
    if (cern_control_system_information_menu_access_keys_underlined()) {
      priv->ui_cues_state |= CERN_UISTATE_FOCUS_CUES_SHOW;
    } else {
      priv->ui_cues_state |= CERN_UISTATE_FOCUS_CUES_HIDDES;

      gint32 action_mask = (UISF_HIDEACCEL | UISF_HIDEFOCUS) << 16;

      HWND handle
        = (HWND) cern_control_get_handle(cern_control_get_top_most_parent(self));

      SendMessage(handle, WM_CHANGEUISTATE,
                  (WPARAM) action_mask | UIS_SET, (LPARAM) 0);
    }
  }

  return
    (priv->ui_cues_state & CERN_UISTATE_FOCUS_CUES_MASK) == CERN_UISTATE_FOCUS_CUES_SHOW;
}

static
gint32
real_cern_control_get_show_params(CernControl *self) {
  return SW_SHOW;
}

static
gchar *
real_cern_control_get_window_text(CernControl *self) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);

  if (!cern_control_get_is_handle_created(self)) {
    if (priv->text == NULL) {
      return "";
    } else {
      return priv->text;
    }
  }

  gsize text_len = GetWindowTextLengthA((HWND) cern_control_get_handle(self));

  if (cern_system_information_dbcs_enabled()) {
    text_len = (text_len * 2) + 1;
  }

  if (priv->window_text != NULL) {
    g_free(priv->window_text);
    priv->window_text = NULL;
  }

  priv->window_text = g_new(gchar, text_len + 1);

  GetWindowTextA((HWND) cern_control_get_handle(self), priv->window_text, text_len);

  return priv->window_text;
}

static
void
real_cern_control_set_window_text(CernControl *self, gchar *value) {
  if (value == NULL) {
    value = "";
  }

  gchar *window_text = cern_control_get_window_text(self);

  if(g_strcmp0(value, window_text) != 0) {
    if (cern_control_get_is_handle_created(self) {
      SetWindowTextA(cern_control_get_handle(self), value);
    } else {
      if (g_utf8_strlen(value) == 0) {
        priv->text = NULL;
      } else {
        priv->text = g_strdup(value);
      }
    }
  }
}

static
void
real_cern_control_assign_parent(CernControl *self, CernControl *parent) {
  if (parent != NULL) {
    cern_control_set_required_scaling_enabled(self, cern_control_get_required_scaling_enabled(parent));
  }

  if (cern_control_get_can_access_properties(self)) {
    CernFont *old_font = cern_control_get_font(self);
    CernColor old_fore_color = cern_control_get_fore_color(self);
    CernColor old_back_color = cern_control_get_back_color(self);
    CernRightToLeft old_rtl = cern_control_get_right_to_left(self);
    gboolean old_enabled = cern_control_get_enabled(self);
    gboolean old_visible = cern_control_get_visible(self);

    CernControlPrivate *priv = cern_control_get_instance_private(self);
    priv->parent = parent;

    CernEventArgs *args = cern_event_args_new();
    cern_control_on_parent_changed(self, args);
    g_clear_object(&args);

    if (cern_control_get_any_disposing_in_hierarchy()) {
      return;
    }

    if (old_enabled != cern_control_get_enabled(self)) {
      CernEventArgs *args = cern_event_args_new();
      cern_control_on_enabled_changed(self, args);
      g_clear_object(&args);
    }

    gboolean new_visible = cern_control_get_visible(self);

    if (old_visible != new_visible
        && !(!old_visible && new_visible
              && cern_control_get_parent(self) != NULL
              && !cern_control_get_top_level(self))) {
      CernEventArgs *args = cern_event_args_new();
      cern_control_on_visible_changed(self, args);
      g_clear_object(&args);
    }

    if (!cern_font_is_equals(old_font, cern_control_get_font(self))) {
      CernEventArgs *args = cern_event_args_new();
      cern_control_on_font_changed(self, args);
      g_clear_object(&args);
    }

    CernColor new_fore_color = cern_control_get_fore_color(self);
    if (!cern_color_equals(&old_fore_color, &new_fore_color)) {
      CernEventArgs *args = cern_event_args_new();
      cern_control_on_fore_color_changed(self, args);
      g_clear_object(&args)
    }

    CernColor new_back_color = cern_control_get_back_color(self);

    if (!cern_color_equals(&old_back_color, &new_back_color)) {
      CernEventArgs *args = cern_event_args_new();
      cern_control_on_background_color_changed(self, args);
      g_clear_object(&args);
    }

    if (old_rtl != cern_control_get_right_to_left(self)) {
      CernEventArgs *args = cern_event_args_new();
      cern_control_on_right_to_left_changed(self, args);
      g_clear_object(&args);
    }

    CernPropertyStore *store = cern_control_get_properties(self);
    if (cern_property_store_get_object(store, CernPropBindingManager) == NULL
        && cern_control_get_created(self)) {
      CernEventArgs *args = cern_event_args_new();
      cern_control_on_binding_context_changed(self, args);
      g_clear_object(&args);
    }
  } else {
    priv->parent = parent;
    CernEventArgs *args = cern_event_args_new();
    cern_control_on_parent_changed(self, args);
    g_clear_object(&args);
  }

  cern_control_set_state(self, CERN_CONTROL_STATE_CHECKEDHOST, FALSE);
  if (cern_control_get_parent_internal(self) != NULL) {
    cern_layout_engine_init_layout(
      cern_control_get_layout_engine(cern_control_get_parent_internal(self)),
      G_OBJECT(self), CernBoundsSpecified_All);
  }
}

static
gboolean
real_cern_control_can_process_mnemonic(CernControl *self) {
  if (!cern_control_get_enabled(self) || !cern_control_get_visible(self)) {
    return FALSE;
  }

  if (cern_control_get_parent(self) != NULL) {
    return cern_control_can_process_mnemonic(cern_control_get_parent(self));
  }

  return TRUE;
}

static
gboolean
real_cern_control_can_select_core(CernControl *self) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);

  if ((priv->control_style & CernControlStyles_Selectable)
        != CernControlStyles_Selectable) {
    return FALSE;
  }

  for (CernControl *c = self; c != NULL; c = cern_control_get_parent(c)) {
    if (!cern_control_get_enabled(c) || !cern_control_get_visible(c)) {
      return FALSE;
    }
  }

  return TRUE;
}

static
void
real_cern_control_create_handle(CernControl *self) {
  gpointer user_cookie = NULL;
  CernControlPrivate *priv = cern_control_get_instance_private(self);

  if (cern_control_get_state(self, CERN_CONTROL_STATE_DISPOSED)) {
    g_critical("Object diaposed...");
    return;
  }

  if (cern_control_get_state(self, CERN_CONTROL_STATE_CREATINGHANDLE)) {
    return;
  }

  CernRectangle original_bounds = { 0 };

  cern_control_set_state(self, CERN_CONTROL_STATE_CREATINGHANDLE, TRUE);

  original_bounds = cern_control_get_bounds(self);

  /* FIXME: Activate Theming scope */

  CernCreateParams *cp = cern_control_get_create_params(self);

  cern_control_set_state(self, CERN_CONTROL_STATE_MIRRORED,
                         cern_create_params_get_ex_style(cp) & WS_EX_LAYOUTRTL);

  if (cern_control_get_parent(self) != NULL) {
    CernRectangle parent_rect
      = cern_control_get_client_rectangle(cern_control_get_parent(self));

    if (!cern_rectangle_is_empty(&parent_rect)) {
      if (cern_create_params_get_x(cp) != CW_USEDEFAULT) {
        cern_create_params_set_x(cp, cern_create_params_get_x(cp) - parent_rect.x);
      }

      if (cern_create_params_get_y(cp) != CW_USEDEFAULT) {
        cern_create_params_set_y(cp, cern_create_params_get_y(cp) - parent_rect.y);
      }
    }
  }

  if (cern_create_params_get_parent(cp) == NULL
      && (cern_create_params_get_style(cp) & WS_CHILD) != 0) {
    /* Park the Handle... */
  }

  cern_native_window_create_handle(priv->window, cp);
  cern_control_update_reflect_parent(self, TRUE);

  cern_control_set_state(self, CERN_CONTROL_STATE_CREATINGHANDLE, FALSE);

  /* FIXME: deactivate theaming scope. */

  CernRectangle self_bounds = cern_control_get_bounds(self);

  if (!cern_rectangle_equals(&original_bounds, &self_bounds)) {
    cern_layout_transaction_do_layout(
      CERN_IARRANGED_ELEMENT(cern_control_get_parent_internal(self)),
      CERN_IARRANGED_ELEMENT(self),
      cern_property_names_bounds());
  }
}


static
void
real_cern_control_def_wnd_proc(CernControl *self, CernMessage *message) {
  CernControlPrivate *priv = cern_control_get_parent_internal(self);
  CernNativeWindowClass *nw_class = CERN_NATIVE_WINDOW_GET_CLASS(priv->window);
  nw_class->window_proc(priv->window, message);
}

static
void
real_cern_control_destroy_handle(CernControl *self) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);

  if (cern_control_get_state(self, CERN_CONTROL_STATE_CREATINGHANDLE)) {
    if (priv->thread_callback_list != NULL) {
      if (CernMessageThreadCallback != 0) {
        MSG msg = { 0 };
        if (PeekMessageA(&msg, (HWND) cern_control_get_handle(self),
                         CernMessageThreadCallback, CernMessageThreadCallback,
                         PM_NOREMOVE)) {
          cern_control_set_state(self, CERN_CONTROL_STATE_THREADMARSHALLPENDING, TRUE);
        }
      }
    }
  }

  if (!cern_control_get_recreating_handle(self)) {
    if (priv->thread_callback_list != NULL) {
      /* FIXME: Thread Method Entry... */
    }
  }

  if (0 != (WS_EX_MDICHILD & GetWindowLongA((HWND) cern_control_get_internal_handle(self), GWL_EXSTYLE))) {
    DefMDIChildProcA(cern_control_get_internal_handle(self), WM_CLOSE, (WPARAM) 0, (LPARAM) 0);
  } else {
    cern_native_window_destroy_handle(priv->window);
  }

  g_free(priv->track_mouse_event);
  priv->track_mouse_event = NULL;
}

static
gboolean
real_cern_control_focus_internal(CernControl *self) {
  if (cern_control_get_can_focus(self)) {
    SetFocus((HWND) cern_control_get_handle(self));
  }

  if (cern_control_get_focused(self)
      && cern_control_get_parent_internal(self) != NULL) {
    CernIContainerControl *cctl
      = cern_control_get_container_control_internal(cern_control_get_parent_internal(self));
    if (cctl != NULL) {
      if (CERN_IS_CONRAINER_CONTROL(cctl)) {
        cern_container_control_set_active_control_internal(CERN_CONTAINER_CONTROL(cctl), self);
      } else {
        cern_i_container_control_set_active_control(cctl, self);
      }
    }
  }

  return cern_control_get_focused(self);
}

static
CernRectangle
real_cern_control_apply_bounds_constraints(CernControl *self, gint32 suggested_x,
                                           gint32 suggested_y, gint32 proposed_width,
                                           gint32 proposed_height) {
  CernSize empty = { 0 };

  {
    CernSize min_size = cern_control_get_minimum_size(self);
    CernSize max_size = cern_control_get_maximum_size(self);

    if (!cern_size_equals(&min_size, &empty)
        || cern_size_equals(&max_size, &empty)) {
      CernSize maximum_size = cern_layout_utils_convert_zero_to_unbounded(max_size);
      CernRectangle new_bounds = {
        .x = suggested_x,
        .y = suggested_y,
        .width = 0,
        .height = 0
      };

      CernSize tmp = {
        .width = proposed_width,
        .height = proposed_height
      };

      CernSize intersect_size
        = cern_layout_utils_intersect_sizes(&tmp, maximum_size);
      cern_rectangle_set_size(&new_bounds, &intersect_size);
      tmp = cern_rectangle_get_size(&new_bounds);
      tmp = cern_layout_utils_intersect_sizes(&temp, &maximum_size);
      cern_rectangle_set_size(&new_bounds, &tmp);
      return new_bounds;
    }
  }
  return (CernRectangle) {
    .x = suggested_x,
    .y = suggested_y,
    .width = proposed_width,
    .height = proposed_height
  };
}

static
CernRectangle
real_cern_control_get_scaled_bounds(CernControl *self,
                                    CernRectangle *bounds,
                                    CernSizeF *factor,
                                    CernBoundsSpecified specified) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);
  RECT adornments = { 0 };

  CernCreateParams *cp = cern_control_get_create_params(self);
  AdjustWindowRectEx(&adornments,
                     cern_create_params_get_style(cp),
                     cern_control_get_has_menu(self),
                     cern_create_params_get_ex_style(cp));
  gfloat dx = factor->width;
  gfloat dy = factor->height;

  gint32 sx = bounds->x;
  gint32 sy = bounds->y;

  gboolean scale_loc = !cern_control_get_state(self, CERN_CONTROL_STATE_TOPLEVEL);

  if (scale_loc) {
    ICernComponentModelSite *site
      = i_cern_component_model_component_get_site(I_CERN_COMPONENT_MODEL_COMPONENT(self));
    if (site != NULL) {
      /* FIXME: IDK What can do here right now... */
    }
  }

  if (scale_loc) {
    if (specified & CernBoundsSpecified_X) {
      sx = (gint32) roundf(bounds->x * dx);
    }
    if (specified & CernBoundsSpecified_Y) {
      sy = (gint32) roundf(bounds->y * dy);
    }
  }

  gint32 sw = bounds->width;
  gint32 sh = bounds->height;

  if ((priv->control_style & CernControlStyles_FixedWidth) != CernControlStyles_FixedWidth
       && (specified & CernBoundsSpecified_Width) != 0) {
    gint32 adornment_width = (adornments.right - adornments.left);
    gint32 local_width = bounds->width - adornment_width;
    sw = (gint32) roundf(local_width * dx) + adornment_width;
  }

  if ((priv->control_style & CernControlStyles_FixedHeight) != CernControlStyles_FixedHeight
      && (specified & CernBoundsSpecified_Height) != 0) {
    gint32 adornment_height = (adornments.bottom - adornments.top);
    gint32 local_height = bounds->height - adornment_height;
    sh = (gint32) roundf(local_height * dy) + adornment_height;
  }

  return (CernRectangle) {
    .x = sx,
    .y = sy,
    .width = sw,
    .height = sh
  };
}

static
gboolean
real_cern_control_get_visible_core(CernControl *self) {
  if (!cern_control_get_state(self, CERN_CONTROL_STATE_VISIBLE)) {
    return FALSE;
  } else if (cern_control_get_parent_internal(self) == NULL) {
    return TRUE;
  } else {
    return cern_control_get_visible_core(cern_control_get_parent_internal(self));
  }
}

static
CernControl *
real_cern_control_get_first_child_control_in_tab_order(CernControl *self, gboolean forward) {
  CernPropertyStore *store = cern_control_get_properties(self);

  CernControlCollection *ctl_controls
    = cern_property_store_get_object(store, CernPropControlsCollection);

  CernControl *found = NULL;

  if (ctl_controls != NULL) {
    if (forward) {
      for (gsize c = cern_control_collection_get_count(ctl_controls); c != NULL; c++) {
        if (found == NULL
            || cern_control_get_tab_index(found)
                > cern_control_get_tab_index(cern_control_collection_get_by_index(ctl_controls, c))) {
          found = cern_control_collection_get_by_index(ctl_controls, c);
        }
      }
    } else {
      for (gsize c = cern_control_collection_get_count(ctl_controls) - 1; c >= 0; c--) {
        if (found == NULL
            || cern_control_get_tab_index(found)
                < cern_control_get_tab_index(cern_control_collection_get_by_index(ctl_controls, c))) {
          found = cern_control_collection_get_by_index(ctl_controls, c);
        }
      }
    }
  }

  return found;
}

static
void
real_cern_control_init_layout(CernControl *self) {
  CernLayoutEngine *le = cern_control_get_layout_engine(self);
  cern_layout_engine_init_layout(le, self, CernBoundsSpecified_All);
}

static
void
real_cern_control_on_create_control(CernControl *self) {
  (void) self;
  /* nothing here */
}

static
void
cern_control_set_window_font(CernControl *self) {
  cern_control_send_message(self, WM_SETFONT, cern_control_get_font_handle(self), NULL);
}

static
void
real_cern_control_on_handle_created(CernControl *self, CernEventArgs *args) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);

  if (cern_control_get_is_handle_created(self)) {
    if (!cern_control_get_style(self, CernControlStyles_UserPaint)) {
      cern_control_set_window_font(self);
    }

    if (cern_dpi_helper_enable_dpi_changed_message_handling()
        && !CERN_IS_WINDOW(self)) {
      gint32 old = priv->device_dpi;
      priv->device_dpi = (gint32) GetDpiForWindow((HWND) cern_control_get_handle(self));
      if (old != priv->device_dpi) {
        cern_control_rescale_constants_for_dpi(self, old, priv->device_dpi);
      }
    }

    cern_control_set_accept_drops(self, cern_control_get_allow_drop(self));

    CernPropertyStore *store
      = cern_control_get_properties(self);

    CernRegion *region
      = cern_property_store_get_object(self, CernPropRegion);

    if (region != NULL) {
      gpointer region_handle
        = cern_control_get_h_rgn_internal(self, region);

      SetWindowRgn((HWND) cern_control_get_handle(self),
                   (HRGN) region_handle,
                   IsWindowVisible((HWND) cern_control_get_handle(self)));

      if (region_handle != NULL) {
        DeleteObject((HGDIOB) region_handle);
      }
    }

    gpointer handle = cern_control_get_handle(self);

    if (priv->text != NULL
        && g_utf8_strlen(priv->text, 0) != 0) {
      SetWindowTextA((HWND) handle, priv->text);
    }

    if (!CERN_IS_SCROLLABLE_CONTROL(self) 
        && !cern_control_get_is_mirrored(self)
        && cern_control_get_state2(self, CERN_CONTROL_STATE2_SETSCROLLPOS)
        && !cern_control_get_state2(self, CERN_CONTROL_STATE2_HAVEINVOKED)) {
      /*cern_control_begin_invoke(cern_control_on_set_scroll_position,)*/

      cern_control_set_state2(self, CERN_CONTROL_STATE2_HAVEINVOKED, TRUE);
      cern_control_set_state2(self, CERN_CONTROL_STATE2_SETSCROLLPOS, FALSE);
    }

    g_signal_emit(self, CernControlSignals[SIGNAL_HANDLE_CREATED], 0, args);


    if (cern_control_get_is_handle_created(self)) {
      if (cern_control_get_state(self, CERN_CONTROL_STATE_THREADMARSHALLPENDING)) {
        PostMessageA((HWND) cern_control_get_handle(self),
                     CernMessageThreadCallback,
                     (WPARAM) 0, (LPARAM) 0);
        cern_control_set_state(self, CERN_CONTROL_STATE_THREADMARSHALLPENDING, FALSE);
      }
    }
  }
  g_signal_emit(self, CernControlSignals[SIGNAL_HANDLE_CREATED], 0, args);


  if (cern_control_get_is_handle_created(self)) {
    if (cern_control_get_state(self, CERN_CONTROL_STATE_THREADMARSHALLPENDING)) {
      PostMessageA((HWND) cern_control_get_handle(self),
                   CernMessageThreadCallback,
                   (WPARAM) 0, (LPARAM) 0);
      cern_control_set_state(self, CERN_CONTROL_STATE_THREADMARSHALLPENDING, FALSE);
    }
  }
}

static
void
cern_control_finalize(GObject *object) {
  CernControl *self = CERN_CONTROL(object);
  CernControlPrivate *priv;

  priv = cern_control_get_instance_private(self);

  /* FIXME: Destroy our window... */
  // cern_native_window_destroy(priv->window);

  G_OBJECT_CLASS(cern_control_parent_class)->finalize(object);
}

#define svf_ex(klass, fn) klass##->##fn = real_cern_control_##fn

/*
 * set virtual function, the object klass must be named as klass
 * */
#define svf(fn) svf_ex(klass, fn) 

static
void
cern_control_class_init(CernControlClass *klass) {
  GObjectClass *o_klass;
  CernComponentModelComponentClass *cm_klass;

  cm_klass = CERN_COMPONENT_MODEL_COMPONENT_CLASS(klass);
  o_klass = G_OBJECT_CLASS(klass);

  svf(set_allow_drop);
  svf(get_allow_drop);

  svf(get_anchor);
  svf(set_anchor);

  svf(get_auto_size);
  svf(set_auto_size);

  svf(get_layout_engine);

  svf(get_back_color);
  svf(set_back_color);

  svf(get_background_image);
  svf(set_background_image);

  svf(get_background_image_layout);
  svf(set_background_image_layout);

  svf(get_binding_context);
  svf(set_binding_context);

  svf(get_can_access_properties);

  svf(get_context_menu);
  svf(set_context_menu);

  svf(get_context_menu_strip);
  svf(set_context_menu_strip);
  
  svf(get_create_params);

  svf(notify_validation_result);

  svf(get_cursor);
  svf(set_cursor);

  svf(get_default_cursor);
  svf(get_default_margins);
  svf(get_default_maximum_size);
  svf(get_default_minimum_size);
  svf(get_default_padding);
  svf(get_default_size);
  
  svf(get_display_rectangle);

  svf(get_dock);
  svf(set_dock);

  svf(get_double_buffered);
  svf(set_double_buffered);

  /* TODO: Not implemented!!! */

  svf(get_focused);

  svf(get_font);
  svf(set_font);

  svf(get_fore_color);
  svf(set_fore_color);

  svf(get_preferred_size);
  svf(get_preferred_size_core);

  svf(get_has_children);

  svf(get_is_container_control);

  svf(get_maximum_size);
  svf(set_maximum_size);
  
  svf(get_minimum_size);
  svf(set_minimum_size);

  svf(get_parent_internal);
  svf(set_parent_internal);

  svf(add_reflect_child);
  svf(remove_reflect_child);

  svf(get_render_transparency_with_visual_styles);

  svf(get_right_to_left);
  svf(set_right_to_left);

  svf(get_scale_children);

  svf(get_text);
  svf(set_text);

  svf(get_show_keyboard_cues);
  svf(get_show_focus_cues);

  svf(get_show_params);

  svf(get_window_text);
  svf(set_window_text);

  svf(assign_parent);
  svf(can_process_mnemonic);
  svf(can_select_core);
  svf(create_handle);
  svf(def_wnd_proc);
  svf(destroy_handle);
  svf(focus_internal);
  svf(apply_bounds_constraints);
  svf(get_scaled_bounds);
  svf(get_visible_core);
  svf(get_first_child_control_in_tab_order);
  svf(init_layout);
  svf(initialize_dc_for_wm_ctl_color);
  svf(is_input_char);
  svf(is_input_key);
  svf(notify_invalidate);
  svf(notify_validating);
  svf(notify_validated);
  svf(on_background_color_changed);
  svf(on_background_image_changed);
  svf(on_background_image_layout_changed);
  svf(on_binding_context_changed); 
  svf(on_causes_validation_changed);
  svf(on_child_layout_resuming);
  svf(on_context_menu_strip_changed);
  svf(on_cursor_changed);
  svf(on_dock_changed);
  svf(on_enabled_changed);
  svf(on_font_changed);
  svf(on_fore_color_changed);
  svf(on_right_to_left_changed);
  svf(on_notify_message);
  svf(on_parent_back_color_changed);
  svf(on_parent_background_image_changed);
  svf(on_parent_binding_context_changed);
  svf(on_parent_cursor_changed);
  svf(on_parent_enabled_changed);
  svf(on_parent_font_changed);
  svf(on_parent_handle_recreated);
  svf(on_parent_handle_recreating);
  svf(on_parent_fore_color_changed);
  svf(on_parent_right_to_left_changed);
  svf(on_parent_visible_changed);
  svf(on_parent_became_invisible);
  svf(on_paint);
  svf(on_tab_index_changed);
  svf(on_tab_stop_changed);
  svf(on_text_changed);
  svf(on_visible_changed);
  svf(on_parent_changed);
  svf(on_click);
  svf(on_client_size_changed);
  svf(on_control_added);
  svf(on_control_removed);
  svf(on_create_control);
  svf(on_handle_created);
  svf(auto_size_changed);
  svf(back_color_changed);
  svf(background_image_changed);
  svf(background_image_layout_changed);
  svf(binding_context_changed);
  svf(cause_validation_changed);
  svf(client_size_changed);
  svf(on_context_menu_changed);
  svf(context_menu_strip_changed);
  svf(validation_result);
  svf(cursor_changed);
  svf(dock_changed);
  svf(enabled_changed);
  svf(font_changed);
  svf(fore_color_changed);
  svf(location_changed);
  svf(margin_changed);
  svf(region_changed);
  svf(right_to_left_changed);
  svf(size_changed);
  svf(tab_index_changed);
  svf(tab_stop_changed);
  svf(text_changed);
  svf(visible_changed);
  svf(click);
  svf(control_added);
  svf(control_removed);
  svf(handle_created);
  svf(handle_destroyed);
  svf(invalidated);
  svf(padding_changed);
  svf(paint);
  svf(double_click);
  svf(enter);
  svf(got_focus);
  svf(key_down);
  svf(key_press);
  svf(key_up);
  svf(layout);
  svf(leave);
  svf(lost_focus);
  svf(mouse_click);
  svf(mouse_double_click);
  svf(mouse_capture_chanched);
  svf(mouse_down);
  svf(mouse_enter);
  svf(mouse_leave);
  svf(dpi_changed_before_parent);
  svf(dpi_changed_after_parent);
  svf(mouse_hover);
  svf(mouse_move);
  svf(mouse_up);
  svf(mouse_wheel);
  svf(move);
  svf(preview_key_down);
  svf(resize);
  svf(change_ui_cues);
  svf(style_changed);
  svf(system_color_changed);
  svf(validating);
  svf(validated);
  svf(parent_changed);

  /* CLASS */
  /* implemented */
  svf_ex(cm_klass, can_raise_events);
  svf_ex(o_klass, finalize);
}

CernControl *
cern_control_new_internal(gboolean install_sync_context) {
  CernControl *self;
  CernControlPrivate *priv;

  self = g_object_new(CERN_TYPE_CONTROL, NULL);
  priv = cern_control_get_instance_private(self);

  priv->property_store = cern_property_store_new();
  priv->device_dpi = 96;
  priv->thread_callback_list = g_queue_new();
  priv->ui_cues_state = 0;
}

CernControl *
cern_control_new(void) {
  CernControl *self;

  self = g_object_new(CERN_TYPE_CONTROL, NULL);

  return self;
}

CernControl *
cern_control_new_2(gchar const *text) {
  return cern_control_new_4(NULL, text);
}

CernControl *
cern_control_new_3(gchar const *text, gint32 left,
                   gint32 top, gint32 width, gint32 height) {
  return cern_control_new_5(NULL, text, left, top, width, height);
}

CernControl *
cern_control_new_4(CernControl *parent, gchar const *text) {
  CernControl *self = cern_control_new();
  CernControlPrivate *priv = cern_control_get_instance_private(self);

  priv->parent = g_object_ref(parent);
  priv->text = g_strdup(text);

  return self;
}

CernControl *
cern_control_new_5(CernControl *parent, gchar const *text,
                   gint32 left, gint32 top, gint32 width, gint32 height) {
  CernControl *self = cern_control_new_4(parent, text);
  CernControlPrivate *priv = cern_control_get_instance_private(self);
  CernPoint location = { .x = left, .y = top };
  CernSize size = { .width = width, .height = height };

  cern_control_set_location(self, &location);
  cern_control_set_size(self, &size);

  return self;
}

gboolean
cern_control_get_allow_drop(CernControl *self) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  return klass->get_allow_drop(self);
}

void
cern_control_set_allow_drop(CernControl *self, gboolean allow_drop) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  klass->set_allow_drop(self, allow_drop);
}

CernAmbientProperties *
cern_control_get_ambient_properties_service(CernControl *self) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);
  gboolean contains = FALSE;
  CernComponentModelComponent *self_site;
  CernAmbientProperties *props
    = cern_property_store_try_get_object(priv->property_store, CernPropAmbientPropertiesService, &contains);

  /* if (!contains) {*/
  /*   self_site = CERN_COMPONENT_MODEL_COMPONENT(self);*/
  /*   if (cern_component_model_component_get_site(self_site)) {*/
  /*     ICernComponentModelSite *site*/
  /*       = cern_component_model_component_get_site(self_site);*/
  /*     props*/
  /*       = cern_component_model_component_get_service(self_site, CERN_TYPE_AMBIENT_PROPERTIES);*/
  /*   } else {*/
  /*     props*/
  /*       = cern_component_model_component_get_service(self_site, CERN_TYPE_AMBIENT_PROPERTIES);*/
  /*   }*/
  /* }*/

  return NULL;
}

CernAnchorStyles
cern_control_get_anchor(CernControl *self) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  return klass->get_anchor(self);
}

void
cern_control_set_anchor(CernControl *self, CernAnchorStyles anchor) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  klass->set_anchor(self, anchor);
}

gboolean
cern_control_get_auto_size(CernControl *self) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  return klass->get_auto_size(self);
}

void
cern_control_set_auto_size(CernControl *self, gboolean auto_size) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  klass->set_auto_size(self, auto_size);
}

CernPoint
cern_control_get_auto_scroll_offset(CernControl *self) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  return klass->get_auto_scroll_offset(self);
}

void
cern_control_set_auto_scroll_offset(CernControl *self, CernPoint *offset) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  klass->set_auto_scroll_offset(self, offset);
}

void
cern_control_set_auto_size_mode(CernControl *self, CernAutoSizeMode mode) {
  cern_common_properties_set_auto_size_mode(CERN_IARRANGED_ELEMENT(self), mode);
}

CernAutoSizeMode
cern_control_get_auto_size_mode(CernControl *self) {
  return
    cern_common_properties_get_auto_size_mode(CERN_IARRANGED_ELEMENT(self));
}

CernLayoutEngine *
cern_control_get_layout_engine(CernControl *self) {
  CernControlClass *klass;

  if (!CERN_IS_CONTROL(self)) {
    return NULL;
  }

  klass = CERN_CONTROL_GET_CLASS(self);

  return klass->get_layout_engine(self);
}

gpointer
cern_control_back_color_brush(CernControl *self) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);
  CernControlPrivate *parent_priv = NULL;

  gpointer obj
    = cern_property_store_get_object(priv->property_store, CernPropBackBrush);

  if (obj != NULL) {
    return obj;
  }

  if (!cern_property_store_contains_object(priv->property_store, CernPropBackColor)) {
    if (priv->parent != NULL) {
      parent_priv = cern_control_get_instance_private(priv->parent);
      CernColor parent_color = cern_control_get_back_color(priv->parent);
      CernColor self_color = cern_control_get_back_color(self);

      if (cern_color_equals(&self_color, &parent_color)) {
        cern_control_back_color_brush(priv->parent);
      }
    }
  }

  CernColor color = cern_control_get_back_color(self);
  gpointer back_brush;
  if(cern_color_translator_to_ole(&color) < 0) {
    back_brush = GetSysColorBrush(cern_color_translator_to_ole(&color) & 0xFF);
    cern_control_set_state(self, CERN_CONTROL_STATE_OWNCTLBRUSH, FALSE);
  } else {
    back_brush = CreateSolidBrush(cern_color_translator_to_win32(&color));
    cern_control_set_state(self, CERN_CONTROL_STATE_OWNCTLBRUSH, TRUE);
  }

  cern_property_store_set_object(priv->property_store, CernPropBackBrush, back_brush);

  return back_brush;
}

CernColor
cern_control_get_back_color(CernControl *self) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  return klass->get_back_color(self);
}

void
cern_control_set_back_color(CernControl *self, CernColor *value) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  klass->set_back_color(self, value);
}

CernImage *
cern_control_get_background_image(CernControl *self) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  return klass->get_background_image(self);
}

void
cern_control_set_background_image(CernControl *self, CernImage *value) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  klass->set_background_image(self, value);
}

CernImageLayout
cern_control_get_background_image_layout(CernControl *self) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  return klass->get_background_image_layout(self);
}

void
cern_control_set_background_image_layout(CernControl *self, CernImageLayout value) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  klass->set_background_image_layout(self, value);
}

gboolean
cern_control_get_becoming_active_control(CernControl *self) {
  return
    cern_control_get_state2(self, CERN_CONTROL_STATE2_BECOMINGACTIVECONTROL);
}

void
cern_control_set_becoming_active_control(CernControl *self, gboolean value) {
  if (value != cern_control_get_becoming_active_control(self)) {
    cern_control_set_state2(self, CERN_CONTROL_STATE2_BECOMINGACTIVECONTROL, value);
  }
}

void
cern_control_reset_bindings(CernControl *self) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);
  CernControlBindingsCollection *bindings
    = cern_property_store_get_object(priv->property_store, CernPropBindings);
  if (bindings != NULL) {
    cern_control_bindings_clear(bindings);
  }
}

CernBindingContext *
cern_control_get_binding_context_internal(CernControl *self) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);
  CernBindingContext *context
    = cern_property_store_get_object(priv->property_store, CernPropBindingManager);
  if (context != NULL) {
    return g_object_ref(context);
  }

  CernControl *parent = cern_control_get_parent_internal(self);

  if (parent != NULL && cern_control_get_can_access_properties(parent)) {
    return cern_control_get_binding_context(parent);
  }

  return NULL;
}

void
cern_control_set_binding_context_internal(CernControl *self, CernBindingContext *value) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);
  CernBindingContext *old_context
    = cern_property_store_get_object(priv->property_store, CernPropBindingManager);

  CernBindingContext *new_context = value;
  if (old_context != new_context) {
    cern_property_store_set_object(priv->property_store,
                                   CernPropBindingManager,
                                   g_object_ref(new_context));
    CernEventArgs *args = cern_event_args_new();
    cern_control_on_binding_context_changed(self, args);
    g_clear_object(&args);
  }
}

CernBindingContext *
cern_control_get_binding_context(CernControl *self) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  return klass->get_binding_context(self);
}

void
cern_control_set_binding_context(CernControl *self, CernBindingContext *value) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  klass->set_binding_context(self, value);
}

gint32
cern_control_get_bottom(CernControl *self) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);
  return priv->x + priv->height;
}

CernRectangle
cern_control_get_bounds(CernControl *self) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);
  return (CernRectangle) {
    .x = priv->x,
    .y = priv->y,
    .width = priv->width,
    .height = priv->height
  };
}

static
void
cern_control_init_scaling(CernControl *self, CernBoundsSpecified specified) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);
  priv->required_scaling
    |= (guint8)((gint32)(specified & CERN_CONTROL_REQUIRED_SCALING_MASK));
}

static
void
real_cern_control_set_bounds_core(CernControl *self, gint32 x, gint32 y,
                                  gint32 width, gint32 height,
                                  CernBoundsSpecified specified) {
  if (cern_control_get_parent_internal(self) != NULL) {
    cern_control_suspend_layout(cern_control_get_parent_internal(self));
  }
}

static
void
cern_control_set_bounds_private(CernControl *self, gint32 x, gint32 y,
                                gint32 width, gint32 height,
                                CernBoundsSpecified specified) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  if ((specified & CernBoundsSpecified_X) == CernBoundsSpecified_None) {
    priv->x = x;
  }
  if ((specified & CernBoundsSpecified_Y) == CernBoundsSpecified_None) {
    priv->y = y;
  }
  if ((specified & CernBoundsSpecified_Width) == CernBoundsSpecified_None) {
    priv->width = width;
  }
  if ((specified & CernBoundsSpecified_Height) == CernBoundsSpecified_None) {
    priv->height = height;
  }

  if (priv->x != x || priv->y != y || priv->width != width || priv->height != height) {
    klass->set_bounds_core(self, x, y, width, height, specified);
    CernControl *parent = cern_control_get_parent_internal(self);
    cern_layout_translation_do_layout(parent, self, cern_property_names_bounds());
  } else {
    cern_control_init_scaling(self, specified);
  }
}

void
cern_control_set_bounds(CernControl *self, CernRectangle *bounds) {
  cern_control_set_bounds_private(self, bounds->x, bounds->y,
                                  bounds->width, bounds->height,
                                  CernBoundsSpecified_All);
}

gboolean
cern_control_get_can_access_properties(CernControl *self) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  return klass->get_can_access_properties(self);
}

gboolean
cern_control_get_can_focus(CernControl *self) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);
  gpointer handle = cern_native_window_get_handle(priv->window);
  if (!cern_control_get_is_handle_created(self)) {
    return FALSE;
  }

  return (IsWindowVisible((HWND) handle) && IsWindowEnabled((HWND) handle));
}

gboolean
cern_control_get_can_raise_events(CernControl *self) {
  CernComponentModelComponentClass *cm_class
     = CERN_COMPONENT_MODEL_COMPONENT_GET_CLASS(self);

  CernComponentModelComponent *component
    = CERN_COMPONENT_MODEL_COMPONENT(self);
  return cm_class->can_raise_events(component);
}

gboolean
cern_control_get_can_select(CernControl *self) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  return klass->get_can_select_core(self);
}

gboolean
cern_control_get_capture(CernControl *self) {
  return cern_control_get_capture_internal(self);
}

void
cern_control_set_capture(CernControl *self, gboolean value) {
  cern_control_set_capture_internal(self, value);
}

gboolean
cern_control_get_capture_internal(CernControl *self) {
  CernControlPrivate *priv
    = cern_control_get_instance_private(self);
  gpointer handle = cern_native_window_get_handle(priv->window);

  return
    cern_control_get_is_handle_created(self) && GetCapture() == (HWND) handle;
}

void
cern_control_set_capture_internal(CernControl *self, gboolean value) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);
  gpointer handle = cern_native_window_get_handle(priv->window);

  if (value != cern_control_get_capture_internal(self)) {
    if (value) {
      SetCapture((HWND) handle);
    } else {
      ReleaseCapture();
    }
  }
}

gboolean
cern_control_get_causes_validation(CernControl *self) {
  return
    cern_control_get_state(self, CERN_CONTROL_STATE_CAUSESVALIDATION);
}

void
cern_control_set_causes_validation(CernControl *self, gboolean value) {
  if (value != cern_control_get_causes_validation(self)) {
    cern_control_set_state(self, CERN_CONTROL_STATE_CAUSESVALIDATION, value);
    CernEventArgs *args = cern_event_args_new();
    cern_control_on_causes_validation_changed(self, args);
    g_clear_object(&args);
  }
}

gboolean
cern_control_get_cache_text_internal(CernControl *self) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);
  CernPropertyStore *store = priv->property_store;
  gboolean found = FALSE;
  gint32 cache_text_counter
    = cern_property_store_try_get_integer(store, CernPropCacheTextCount, &found);
  return
    cache_text_counter > 0
      || cern_control_get_style(self, CernControlStyles_CacheText);
}

void
cern_control_set_cache_text_internal(CernControl *self, gboolean value) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);
  CernPropertyStore *store = priv->property_store;
  gboolean found = FALSE;

  if (cern_control_get_style(self, CernControlStyles_CacheText)
      || !cern_control_get_is_handle_created(self)) {
    return;
  }

  gint32 cache_text_counter
    = cern_property_store_try_get_integer(store, CernPropCacheTextCount, &found);

  if (value) {
    if (cache_text_counter == 0) {
      GObject *obj;

      if (!cern_property_store_try_get_object(store, CernPropCacheTextField, found)) {
        obj = g_object_new(G_TYPE_OBJECT, NULL);
      } else {
        obj = cern_property_store_get_object(store, CernPropCacheTextField);
      }

      g_object_set_data(obj, "Control::text", priv->text);

      cern_property_store_set_object(store, CernPropCacheTextField, obj);
      if (priv->text == NULL) {
        priv->text = cern_control_get_window_text(self);
      }
      cache_text_counter++;
    }
  } else {
    cache_text_counter--;
    if (cache_text_counter == 0) {
      GObject *obj
        = cern_property_store_try_get_object(store, CernPropCacheTextField, found);

      priv->text = g_object_get_data(obj, "Control::text");
    }
  }
  cern_property_store_set_integer(store, CernPropCacheTextCount, cache_text_counter);
}

gboolean
cern_control_get_check_for_illegal_cross_thread_calls(void) {
  g_critical("Not implemented yet...");
  return TRUE;
}

void
cern_control_set_check_for_illegal_cross_thread_calls(gboolean value) {
  g_critical("Not implemented yet...");
  (void) value;
}

CernRectangle
cern_control_get_client_rectangle(CernControl *self) {
  CernControlPrivate *priv
    = cern_control_get_instance_private(self);

  return (CernRectangle) {
    .x = 0,
    .y = 0,
    .width = priv->client_width,
    .height = priv->client_height
  };
}

CernSize
cern_control_get_client_size(CernControl *self) {
  CernControlPrivate *priv
    = cern_control_get_instance_private(self);

  return (CernSize) {
    .width = priv->client_width,
    .height = priv->client_height
  };
}

void
cern_control_set_client_size(CernControl *self, CernSize *value) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  klass->set_client_size_core(self, value->width, value->height);
}

gchar *
cern_control_get_company_name(CernControl *self) {
  CernControlVersionInfo *info = cern_control_get_verison_info();
  return cern_control_version_info_get_company_name(info);
}

gboolean
cern_control_get_contains_focus(CernControl *self) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);
  gpointer handle = cern_native_window_get_handle(priv->window);

  if (!cern_control_get_is_handle_created(self)) {
    return FALSE;
  }

  HWND focus_window = GetFocus();

  if (focus_window == NULL) {
    return FALSE;
  }

  if (focus_window == handle) {
    return TRUE;
  }

  if (IsChild(handle, focus_window)) {
    return TRUE;
  }

  return FALSE;
}

CernContextMenu *
cern_control_get_context_menu(CernControl *self) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  return klass->get_context_menu(self);
}

void
cern_control_set_context_menu(CernControl *self, CernContextMenu *value) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  klass->set_context_menu(self, value);
}

CernContextMenuStrip *
cern_control_get_context_menu_strip(CernControl *self) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  klass->get_context_menu_strip(self);
}

void
cern_control_set_context_menu_strip(CernControl *self, CernContextMenuStrip *value) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  klass->set_context_menu_strip(self, value);
}

CernControlCollection *
cern_control_get_controls(CernControl *self) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);
  CernControlCollection *controls;

  CernPropertyStore *store = priv->property_store;

  controls
    = CERN_CONTROL_COLLECTION(cern_property_store_get_object(store, CernPropControlsCollection));

  if (controls == NULL) {
    controls = cern_control_create_controls_instance(self);
    cern_property_store_set_object(store, CernPropControlsCollection, controls);
  }

  return controls;
}

gboolean
cern_control_get_created(CernControl *self) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);
  return (priv->state & CERN_CONTROL_STATE_CREATED) != 0;
}

CernCreateParams *
cern_control_get_create_params(CernControl *self) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  return klass->get_create_params(self);
}


gboolean
cern_control_validate_active_control(CernControl *self,
                                     gboolean *validated_control_allows_focus_change) {
  gboolean valid = TRUE;
  (*validated_control_allows_focus_change) = FALSE;
  CernIContainerControl *c = cern_control_get_conrainer_control_internal(self);
  if (c != NULL && cern_control_get_causes_validation(self)) {
    if (CERN_IS_CONTAINER_CONTROL(c)) {
      CernContainerControl *container = CERN_CONTAINER_CONTROL(c);
      while (cern_container_control_get_active_control(container) == NULL) {
        CernContainerControl *cc;
        CernControl *parent = cern_control_get_parent_internal(self);
        if (parent != NULL) {
          if (CERN_IS_CONTAINER_CONTROL(cern_control_get_container_control_internal(parent))) {
            cc = CERN_CONTAINER_CONTROL(cern_control_get_container_control_internal(parent));
            if (cc != NULL) {
              container = cc;
            } else {
              break;
            }
          } else {
            break;
          }
        }
      }

      valid = cern_container_control_validate_internal(container, TRUE, validated_control_allows_focus_change);
    }

  }

  return valid;
}

gboolean
cern_control_get_validation_cancelled(CernControl *self) {
  if (cern_control_get_state(self, CERN_CONTROL_STATE_VALIDATIONCANCELLED)) {
    return TRUE;
  } else {
    CernControl *parent = cern_control_get_parent_internal(self);
    if (parent != NULL) {
      return cern_control_get_validation_cancelled(parent);
    }

    return FALSE;
  }
}

void
cern_control_set_validation_cancelled(CernControl *self, gboolean value) {
  cern_control_set_state(self, CERN_CONTROL_STATE_VALIDATIONCANCELLED, value);
}

gboolean
cern_control_get_is_top_mdi_window_closing(CernControl *self) {
  return cern_control_get_state2(self, CERN_CONTROL_STATE2_TOPMDIWINDOWCLOSING);
}

void
cern_control_set_is_top_mdi_window_closing(CernControl *self, gboolean value) {
  cern_control_set_state2(self, CERN_CONTROL_STATE2_TOPMDIWINDOWCLOSING, value);
}

gboolean
cern_control_get_is_curently_being_scaled(CernControl *self) {
  return cern_control_get_state2(self, CERN_CONTROL_STATE2_CURRENTLYBEINGSCALED);
}

void
cern_control_set_is_curently_being_scaled(CernControl *self, gboolean value) {
  cern_control_set_state2(self, CERN_CONTROL_STATE2_CURRENTLYBEINGSCALED, value);
}

gint32
cern_control_get_create_thread_id(CernControl *self) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);
  if (cern_control_get_is_handle_created(self)) {
    DWORD pid;
    return (gint32) GetWindowThreadProcessId((HWND) cern_control_get_handle(self), &pid);
  } else {
    return GetCurrentThreadId();
  }
}

CernCursor *
cern_control_get_cursor(CernControl *self) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  return klass->get_cursor(self);
}

void
cern_control_set_cursor(CernControl *self, CernCursor *value) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  klass->set_cursor(self, value);
}

CernControlBindingsCollection *
cern_control_get_data_bindings(CernControl *self) {
  CernControlPrivate *priv
    = cern_control_get_instance_private(self);
  CernPropertyStore *store = priv->property_store;

  CernControlBindingsCollection *bindings
    = CERN_CONTROL_BINDINGS_COLLECTION(cern_property_store_get_object(store, CernPropBindings));

  if (bindings == NULL) {
    bindings = cern_control_bindings_collection_new(self);
    cern_property_store_set_object(store, CernPropBindings, bindings);
  }

  return bindings;
}

CernColor
cern_control_get_default_back_color(void) {
  return cern_system_colors_control();
}

CernCursor *
cern_control_get_default_cursor(CernControl *self) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  klass->get_default_cursor(self);
}

CernFont *
cern_control_get_default_font(void) {
  if (CernDefaultFont == NULL) {
    CernDefaultFont = cern_system_fonts_get_default_font();
  }
  return CernDefaultFont;
}

CernColor
cern_control_get_default_fore_color(void) {
  return cern_system_colors_control_text();
}

CernPadding
cern_control_get_default_margin(CernControl *self) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  return klass->get_default_margin(self);
}

CernSize
cern_control_get_default_maximum_size(CernControl *self) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  klass->get_default_maximum_margin(self);
}

CernSize
cern_control_get_default_minimum_size(CernControl *self) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  klass->get_deifault_minimum_size(self);
}

CernPadding
cern_control_get_default_padding(CernControl *self) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  klass->get_default_padding(self);
}

CernRightToLeft
cern_control_get_default_right_to_left(void) {
  return CernRightToLeft_No;
}

CernSize
cern_control_get_default_size(CernControl *self) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  klass->get_default_size(self);
}

gint32
cern_control_get_device_dpi(CernControl *self) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);
  if (cern_dpi_helper_enable_dpi_changed_message_handling()) {
    return priv->device_dpi;
  }
  return cern_dpi_helper_device_dpi();
}

gboolean
cern_control_get_double_buffered(CernControl *self) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  klass->get_double_buffered(self);
}

void
cern_control_set_double_buffered(CernControl *self, gboolean value) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  klass->set_double_buffered(self, value);
}

gboolean
cern_control_get_enabled(CernControl *self) {
  if (!cern_control_get_state(self, CERN_CONTROL_STATE_ENABLED)) {
    return FALSE;
  } else if (cern_control_get_parent_internal(self) == NULL) {
    return TRUE;
  } else {
    return cern_conrol_get_enabled(cern_control_get_parent_internal(self));
  }
}

void
cern_control_set_enabled(CernControl *self, gboolean value) {
  gboolean old_value = cern_control_get_enabled(self);
  cern_control_set_state(CERN_CONTROL_STATE_ENABLED, value);

  if (old_value != value) {
    if (!value) {
      cern_control_select_next_if_focused(self);
    }

    CernEventArgs *args = cern_event_args_new();
    cern_control_on_enabled_changed(self, args);
    g_clear_object(&args);
  }
}

gboolean
cern_control_get_focused(CernControl *self) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  return klass->get_focused(self);
}

void
cern_control_scale_font(CernControl *self, gfloat factor) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);
  CernPropertyStore *store = priv->property_store;
  CernFont *local_font = cern_property_store_get_object(store, CernPropFont);
  CernFont *resolved = cern_control_get_font(self);
  CernFont *new_font
    = cern_dpi_helper_enable_dpi_changed_high_dpi_improvements()
      ? cern_font_new_4(cern_font_get_font_family(resolved),
                        cern_font_get_size(resolved) * factor,
                        cern_font_get_style(resolved),
                        cern_font_get_unit(resolved),
                        cern_font_get_gdi_charset(resolved),
                        cern_font_get_gdi_vertical_font(resolved))
      : cern_font_new_7(cern_font_get_font_family(resolved),
                        cern_font_get_size(resolved) * factor,
                        cern_font_get_style(resolved);

  if (local_font == NULL || !cern_font_is_equals(local_font, new_font)) {
    cern_property_store_set_object(store, CernPropFont, new_font);
    g_clear_object(&local_font);

    if (cern_font_is_equals(resolved, new_font)) {
      cern_control_dispose_font_handle(self);
      if (cern_property_store_contains_integer(store, CernPropFontHeight)) {
        cern_property_store_set_integer(store, CernPropFontHeight, cern_font_get_height(new_font));
      }
    }
  }
}

gpointer
cern_control_get_font_handle(CernControl *self) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);
  CernPropertyStore *store = priv->property_store;

  CernFont *font
    = cern_property_store_get_object(store, CernPropFont);

  if (font != NULL) {
    CernFontHandleWrapper *wrapper
      = cern_property_store_get_object(store, CernPropFontHandleWrapper);
    if (wrapper == NULL) {
      wrapper = cern_font_handle_wrapper_new(font);
      cern_property_store_set_object(store, CernPropFontHandleWrapper, wrapper);
    }

    return cern_font_handle_wrapper_get_font(wrapper);
  }

  if (cern_control_get_parent(self) != NULL) {
    cern_control_get_font_handle(cern_control_get_parent(self));
  }

  CernAmbientProperties *properties
    = cern_control_get_ambient_properties_service(self);

  if (properties != NULL
      && cern_ambient_properties_get_font(properties) != NULL) {
    CernFontHandleWrapper *font_handle = NULL;

    CernFont *current_ambient
      = cern_property_store_get_object(store, CernPropCurrentAmbientFont);

    if (current_ambient != NULL
        && cern_font_is_equals(current_ambient,
                               cern_ambient_properties_get_font(properties)) {
      font_handle = cern_property_store_get_object(store, CernPropFontHandleWrapper);
    } else {
      cern_property_store_set_object(store, CernPropCurrentAmbientFont,
                                     cern_ambient_properties_get_font(properties));
    }

    if (font_handle == NULL) {
      font = cern_ambient_properties_get_font(properties);
      font_handle = cern_font_handle_wrapper_new(font);


      cern_property_store_set_object(store, CernPropFontHandleWrapper, font_handle);
    }

    return cern_font_handle_wrapper_get_font(font_handle);
  }

  CernFontHandleWrapper *default_font_handle
    = cern_control_get_default_font_handle_wrapper();

  return cern_font_handle_wrapper_get_font(default_font_handle);
}

gint32
cern_control_get_font_height(CernControl *self) {
  CernControlPrivate *priv
    = cern_control_get_instance_private(self);
  gboolean found = FALSE;

  CernPropertyStore *store = priv->property_store;

  gint32 font_height
    = cern_property_store_try_get_integer(store, CernPropFontHeight, &found);

  if (found && font_height != -1) {
    return font_height;
  } else {
    CernFont *font = cern_property_store_get_object(store, CernPropFont);

    if (font != NULL) {
      font_height = cern_font_get_height(font);

      cern_property_store_set_integer(store, CernPropFontHeight, font_height);

      return font_height;
    }
  }

  gint32 local_font_height = -1;

  CernControl *parent = cern_control_get_parent_internal(self);

  if (parent != NULL && cern_control_get_can_access_properties(parent)) {
    local_font_height = cern_control_get_font_height(parent);
  }

  if (local_font_height == -1) {
    local_font_height = cern_font_get_height(font);

    cern_property_store_set_integer(store, CernPropFontHeight, local_font_height);
  }

  return local_font_height;
}


void
cern_control_set_font_height(CernControl *self, gint32 value) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);
  CernPropertyStore *store = priv->property_store;

  cern_property_store_set_integer(store, CernPropFontHeight, value);
}

CernColor
cern_control_get_fore_color(CernControl *self) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  return klass->get_fore_color(self);
}

void
cern_control_set_fore_color(CernControl *self, CernColor *value) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  klass->set_fore_color(self, value);
}

CernSize
cern_control_get_preferred_size_with_proposed(CernControl *self, CernSize *proposed_size) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  klass->get_preferred_size_with_proposed(self, proposed_size);
}

CernSize
cern_control_get_preferred_size_core(CernControl *self, CernSize *proposed_size) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  return klass->get_preferred_size_core(self, proposed_size);
}

gpointer
cern_control_get_handle(CernControl *self) {
  if (!cern_control_get_is_handle_created(self)) {
    cern_control_create_handle(self);
  }

  return cern_control_get_handle_internal(self);
}

gpointer
cern_control_get_handle_internal(CernControl *self) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);
  return cern_native_window_get_handle(priv->window);
}

gboolean
cern_control_get_has_children(CernControl *self) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);
  CernPropertyStore *store = priv->property_store;

  CernControlCollection *controls
    = cern_property_store_try_get_object(store, CernPropControlsCollection);

  return controls != NULL
          && (cern_control_collection_get_count(controls) > 0);
}

gboolean
cern_control_get_has_menu(CernControl *self) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  return klass->get_has_menu(self);
}

gint32
cern_control_get_height(CernControl *self) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);
  return priv->height;
}

void
cern_control_set_height(CernControl *self, gint32 value) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);

  cern_control_set_bounds(self, priv->x, priv->y, priv->width, value, CernBoundsSpecified_Height);
}

gboolean
cern_control_get_hosted_in_win32_dialog_manager(CernControl *self) {
  if (!cern_control_get_state(self, CERN_CONTROL_STATE_CHECKEDHOST)) {
    CernControl *top_most = cern_control_get_top_most_parent(self);

    if (self != top_most) {
      cern_control_set_state(self, CERN_CONTROL_STATE_HOSTEDINDIALOG,
                             cern_control_get_hosted_in_win32_dialog_manager(top_most));
    } else {
      HWND parent_handle = GetParent((HWND) cern_control_get_handle(self));
      HWND last_parent_handle = parent_handle;

      struct buffer {
        gchar *sb;
        gsize capacity;
      } buf;

      buf.sb = g_new(gchar, 32);
      buf.capacity = 32;

      cern_control_set_state(self, CERN_CONTROL_STATE_HOSTEDINDIALOG, FALSE);

      while (parent_handle != NULL) {
        gint32 len = GetClassName(last_parent_handle, NULL, 0);

        if (len > sb.capacity) {
          buf.sb = g_renew(buf.sb, gchar, len + 5);
          buf.capacity = len + 5;
        }

        GetClassName(last_parent_handle, buf.sb, buf.capacity);

        if (g_strcmp0(buf.sb, "#32770") == 0) {
          cern_control_set_state(self, CERN_CONTROL_STATE_HOSTEDINDIALOG, TRUE);
          break;
        }

        last_parent_handle = parent_handle;
        parent = GetParent(last_parent_handle);
      }

      g_free(buf.sb);
    }

    cern_control_set_state(self, CERN_CONTROL_STATE_CHECKEDHOST, TRUE);
  }

  return cern_control_get_state(self, CERN_CONTROL_STATE_HOSTEDINDIALOG);
}

gboolean
cern_control_get_is_handle_created(CernControl *self) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);
  return (cern_native_window_get_handle(priv->window) != NULL);
}

gboolean
cern_control_get_is_layout_suspended(CernControl *self) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);
  return priv->layout_suspend_count > 0;
}

gboolean
cern_control_get_is_window_obscured(CernControl *self) {
  if (!cern_control_get_is_handle_created(self)
       && !cern_control_get_visible(self)) {
    return FALSE;
  }

  gboolean empty_region = FALSE;

  RECT temp = { 0 };

  CernRegion *working;
  CernControl *parent = cern_control_get_parent_internal(self);

  if (parent != NULL) {
    while (cern_control_get_parent_internal(parent) != NULL) {
      parent = cern_control_get_parent_internal(parent);
    }
  }

  GetWindowRect(cern_control_get_handle(self), &temp);

  CernRectangle self_rect
    = cern_rectangle_create_with_ltrb(temp.left, temp.top, temp.right, temp.bottom);

  working = cern_region_new_rect(self_rect);

  {
    HWND prev;
    HWND next;
    HWND start;

    if (parent != NULL) {
      start = (HWND) cern_control_get_handle(parent);
    } else {
      start = (HWND) cern_control_get_handle(self);
    }

    for (prev = start;
         (next = GetWindow(prev, GW_HWNDPREV)) != NULL;
         prev = next) {
      GetWindowRect(next, &temp);
      CernRectangle current
        = cern_rectangle_create_with_ltrb(temp.left, temp.top, temp.right, temp.bottom);

      if (IsWindowVisible(next)) {
        cern_region_exclude_with_rectangle(working, &current);
      }
    }

    CernGraphics *g = cern_control_create_graphics(self);

    empty_region = cern_region_is_empty(working, g);

    g_clear_object(&g);
  }

  g_clear_object(&working);

  return empty_region;
}

gpointer
cern_control_get_internal_handle(CernControl *self) {
  if (!cern_control_get_is_handle_created(self)) {
    return NULL;
  }

  return cern_control_get_handle(self);
}

gboolean
cern_control_get_invoke_required(CernControl *self) {
  HWND hwnd;

  if (cern_control_get_is_handle_created(self)) {
    hwnd = (HWND) cern_control_get_handle(self);
  } else {
    CernControl *marshaling_control = cern_control_find_marshaling_control(self);

    if (!cern_control_get_is_handle_created(marshaling_control)) {
      return FALSE;
    }

    hwnd = (HWND) cern_control_get_handle(marshaling_control);
  }

  DWORD pid;

  DWORD hwnd_thread = GetWindowThreadProcessId(hwnd, &pid);
  DWORD current_thread = GetCurrentThreadId();

  return hwnd_thread == current_thread;
}

gboolean
cern_control_get_is_accessible(CernControl *self) {
  return cern_control_get_state(self, CERN_CONTROL_STATE_ISACCESSIBLE);
}

void
cern_control_set_is_accessible(CernControl *self, gboolean value) {
  cern_control_set_state(self, CERN_CONTROL_STATE_ISACCESSIBLE, value);
}

gboolean
cern_control_get_is_container_control(CernControl *self) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  return klass->get_is_container_control(self);
}

gboolean
cern_control_get_is_mirrored(CernControl *self) {
  if (!cern_control_get_is_handle_created(self)) {
    CernCreateParams *cp = cern_control_get_create_params(self);
    gint32 ex_styles = cern_create_params_get_ex_style(cp);

    cern_control_set_state(self, CERN_CONTROL_STATE_MIRRORED, (ex_styles & WS_EX_LAYOUTRTL) != 0);
  }

  return cern_control_get_state(self, CERN_CONTROL_STATE_MIRRORED);
}

gint32
cern_control_get_left(CernControl *self) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);
  return priv->x;
}

void
cern_control_set_left(CernControl *self, gint32 value) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);
  cern_control_set_bounds(self, value, priv->y, priv->width priv->height, CernBoundsSpecified_X);
}

CernPoint
cern_control_get_location(CernControl *self) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);
  return (CernPoint) {
    .x = priv->x,
    .y = priv->y
  };
}

void
cern_control_set_location(CernControl *self, CernPoint *value) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);
  cern_control_set_bounds(self, value->x, value->y, priv->width, priv->height, CernBoundsSpecified_Location);
}

CernPadding
cern_control_get_margin(CernControl *self) {
  return cern_common_properties_get_margin(CERN_IARRANGED_ELEMENT(self));
}

void
cern_control_set_margin(CernControl *self, CernPadding *value) {
  CernPadding normalized_margin
    = cern_layout_utils_clamp_negation_padding_to_zero(value);

  CernPadding current_margin = cern_control_get_margin(self);

  if (!cern_padding_equals(&current_margin, &normalized_margin)) {
    cern_common_properties_set_margin(CERN_IARRANGED_ELEMENT(self), value);
    CernEventArgs *args = cern_event_args_new();
    cern_control_on_margin_changed(self, args);
    g_clear_object(&args);
  }
}

CernSize
cern_control_get_maximum_size(CernControl *self) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  return klass->get_maximum_size(self);
}

void
cern_control_set_maximum_size(CernControl *self, CernSize *value) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  klass->set_maximum_size(self, value);
}

CernSize
cern_control_get_minimum_size(CernControl *self) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  return klass->get_minimum_size(self);
}

void
cern_control_set_minimum_size(CernControl *self, CernSize *value) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  klass->set_minimum_size(self, value);
}

CernKeys
cern_control_get_modifier_keys(void) {
  CernKeys modifiers = 0;

  if (GetKeyState((gint32) CernKeys_ShiftKey) < 0) {
    modifiers |= CernKeys_ShiftKey;
  }

  if (GetKeyState((gint32) CernKeys_ControlKey) < 0) {
    modifiers |= CernKeys_ControlKey;
  }

  if (GetKeyState((gint32) CernKeys_Menu) < 0) {
    modifiers |= CernKeys_Alt;
  }

  return modifiers;
}

CernMouseButtons
cern_control_get_mouse_buttons(void) {
  CernMouseButtons buttons = 0;

  if (GetKeyState((gint32) CernKeys_LButton) < 0) {
    buttons |= CernMouseButtons_Left;
  }

  if (GetKeyState((gint32) CernKeys_RButton) < 0) {
    buttons |= CernMouseButtons_Right;
  }

  if (GetKeyState((gint32) CernKeys_MButton) < 0) {
    buttons |= CernMouseButtons_Middle;
  }

  if (GetKeyState((gint32) CernKeys_XButton1) < 0) {
    buttons |= CernMouseButtons_XButton1;
  }

  if (GetKeyState((gint32) CernKeys_XButton2) < 0) {
    buttons |= CernMouseButtons_XButton2;
  }

  return buttons;
}

CernPoint
cern_control_get_mouse_position(void) {
  POINT pt = { 0 };
  GetCursorPos(&pt);
  return (CernPoint) {
    .x = pt.x,
    .y = pt.y
  };
}

gchar *
cern_control_get_name(CernControl *self) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);
  ICernComponentModelSite *site = I_CERN_COMPONENT_MODEL_SITE(self);
  CernPropertyStore *store = priv->property_store;

  GObject *obj = cern_property_store_get_object(store, CernPropName);

  gchar *name = g_object_get_data(obj, "CernControl::name");

  if (name == NULL
      || g_strcmp0(name, "") == 0) {
    // FIXME: Need to find  out to get ComponentModelSite...
  }

  g_critical("%s(...): Not implemented", __func__);

  return NULL;
}

void
cern_control_set_name(CernControl *self, gchar *value) {
  (void) self;
  (void) value;

  g_critical("%s(...): Not implemented", __func__);
}

CernControl *
cern_control_get_parent(CernControl *self) {
  return cern_control_get_parent_internal(self);
}

void
cern_control_set_parent(CernControl *self, CernControl *parent) {
  cern_control_set_parent_internal(self, parent);
}

CernControl *
cern_control_get_parent_internal(CernControl *self) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  return klass->get_parent_internal(self);
}

void
cern_control_set_parent_internal(CernControl *self, CernControl *parent) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  klass->set_parent_internal(self, parent);
}

gchar *
cern_control_get_product_name(CernControl *self) {
  g_critical("%s(...): Not implemented");
  return NULL;
}

gchar *
cern_control_get_product_version(CernControl *self) {
  g_critical("%s(...): Not implemented");
  return NULL;
}

CernPropertyStore *
cern_control_get_properties(CernControl *self) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);
  return priv->property_store;
}

CernColor
cern_control_get_raw_back_color(CernControl *self) {
  CernPropertyStore *store = cern_control_get_properties(self);
  return cern_property_store_get_color(CernPropBackColor);
}

gboolean
cern_control_get_recreating_handle(CernControl *self) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);

  return (priv->state & CERN_CONTROL_STATE_RECREATE) != 0;
}

CernRegion *
cern_control_get_region(CernControl *self) {
  CernPropertyStore *store = cern_control_get_properties(self);
  return CERN_REGION(cern_property_store_get_object(store, CernPropRegion));
}

void
cern_control_set_region(CernControl *self, CernRegion *value) {
  CernPropertyStore *store = cern_control_get_properties(self);

  CernRegion *old_region = cern_control_get_region(self);

  if (old_region != value) {
    cern_property_store_set_object(store, CernPropRegion, value);

    if (old_region != NULL) {
      g_clear_object(&old_region);
    }

    if (cern_control_get_is_handle_created(self)) {
      gpointer region_handle = NULL;

      if (value != NULL) {
        region_handle = cern_control_get_h_rgn(value);
      }

      if (SetWindowRgn(cern_control_get_handle(self),
                       (HRGN) region_handle,
                       cern_control_is_window_visible(self)) != 0) {
        region_handle = NULL;
      }

      if (region_handle != NULL) {
        DeleteObject((HGDIOBJ) region_handle);
      }
    }
    CernEventArgs *args = cern_event_args_new();
    cern_control_on_region_changed(self, args);
    g_clear_object(&args);
  }
}

gboolean
cern_control_get_render_transparent(CernControl *self) {
  CernColor back_color = cern_control_get_back_color(self);
  return
    cern_control_get_style(self, CernControlStyles_SupportsTransparentBackColor)
      && cern_color_get_alpha(&back_color) < 255;
}

gboolean
cern_control_get_render_transparency_with_visual_styles(CernControl *self) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  return klass->get_render_transparency_with_visual_styles(self);
}

CernBoundsSpecified
cern_control_get_required_scaling(CernControl *self) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);

  if (priv->required_scaling & CERN_CONTROL_REQUIRED_SCALING_ENABLED_MASK) {
    return (CernBoundsSpecified) (priv->required_scaling & CERN_CONTROL_REQUIRED_SCALING_MASK);
  }

  return CernBoundsSpecified_None;
}

void
cern_control_set_required_scaling(CernControl *self, CernBoundsSpecified value) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);
  guint8 enable_bit
    = (guint8) (priv->required_scaling & CERN_CONTROL_REQUIRED_SCALING_ENABLED_MASK);

  priv->required_scaling = (guint8) (((gint32) value & CERN_CONTROL_REQUIRED_SCALING_MASK) | enable_bit));
}

gboolean
cern_control_get_required_scaling_enabled(CernControl *self) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);

  return (priv->required_scaling & CERN_CONTROL_REQUIRED_SCALING_ENABLED_MASK);
}

void
cern_control_set_required_scaling_enabled(CernControl *self, gboolean value) {
  CernControlPrivate *priv
  guint8 scaling
    = (guint8)(priv->required_scaling & CERN_CONTROL_REQUIRED_SCALING_MASK);

  priv->required_scaling = scaling;
  if (value) {
    priv->required_scaling |= CERN_CONTROL_REQUIRED_SCALING_ENABLED_MASK;
  }
}

gboolean
cern_control_get_resize_redraw(CernControl *self) {
  return cern_control_get_style(self, CernControlStyles_ResizeRedraw);
}

void
cern_control_set_resize_redraw(CernControl *self, gboolean value) {
  cern_control_set_style(self, CernControlStyles_ResizeRedraw, value);
}

gint32
cern_control_get_right(CernControl *self) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);
  return priv->x + priv->width;
}

CernRightToLeft
cern_control_get_right_to_left(CernControl *self) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  return klass->get_right_to_left(self);
}

void
cern_control_set_right_to_left(CernControl *self, CernRightToLeft value) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  klass->set_right_to_left(self, value);
}

gboolean
cern_control_get_scale_children(CernControl *self) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  return klass->get_scale_childred(self);
}

CernSize
cern_control_get_size(CernControl *self) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);

  return (CernSize) {
    .width = priv->width,
    .heigth = priv->height
  };
}

void
cern_control_set_size(CernControl *self, CernSize *value) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);
  cern_control_set_bounds(self, priv->x, priv->y, value->width, value->height, CernBoundsSpecified_Size);
}

gint32
cern_control_get_tab_index(CernControl *self) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);

  return priv->tab_index == -1 ? 0 : priv->tab_index;
}

void
cern_control_set_tab_index(CernControl *self, gint32 index) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);

  if (index < 0) {
    g_warning("Tab index cannot be less than 0.");
    return;
  }

  if (index != priv->tab_index) {
    priv->tab_index = index;
    CernEventArgs *args = cern_event_args_new();
    cern_control_on_tab_index_changed(self, args);
    g_clear_object(&args);
  }
}

gboolean
cern_control_get_tab_stop(CernControl *self) {
  return cern_control_get_tab_stop_internal(self);
}

void
cern_control_set_tab_stop(CernControl *self, gboolean value) {
  if (cern_control_get_tab_stop(self) != value) {
    cern_control_set_tab_stop_internal(self, value);

    if (cern_control_get_is_handle_created(self)) {
      cern_control_set_window_style(self, WS_TABSTOP, value);

      CernEventArgs *args = cern_event_args_new();
      cern_control_on_tab_stop_changed(self, args);
      g_clear_object(&args);
    }
  }
}

gboolean
cern_control_get_tab_stop_internal(CernControl *self) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);

  return priv->state & CERN_CONTROL_STATE_TABSTOP;
}

void
cern_control_set_tab_stop_internal(CernControl *self, gboolean value) {
  if (cern_control_get_tab_stop_internal(self) != value) {
    cern_control_set_state(self, CERN_CONTROL_STATE_TABSTOP, value);
  }
}

gpointer
cern_control_get_tag(CernControl *self) {
  CernPropertyStore *store = cern_control_get_properties(self);

  return cern_property_store_get_object(store, CernPropUserData);
}

void
cern_control_set_tag(CernControl *self, gpointer value) {
  CernPropertyStore *store = cern_control_get_properties(self);

  cern_property_store_set_object(store, CernPropUserData, value);
}

gchar *
cern_control_get_text(CernControl *self) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);

  return klass->get_text(self);
}

void
cern_control_set_text(CernControl *self, gchar *value) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);

  klass->set_text(self, value);
}

gint32
cern_control_get_top(CernControl *self) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);
  return priv->y;
}

void
cern_control_set_top(CernControl *self, gint32 value) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);

  cern_control_set_bounds(self, priv->x, valule, priv->width, priv->height, CernBoundsSpecified_Y);
}

CernControl *
cern_control_get_top_level_control(CernControl *self) {
  return cern_control_get_top_level_control_internal(self);
}

CernControl *
cern_control_get_top_level_control_internal(CernControl *self) {
  CernControl *control = self;

  while (control != NULL && !cern_control_get_top_level(control)) {
    control = cern_control_get_parent_internal(control);
  }

  return control;
}

CernControl *
cern_control_get_top_most_parent(CernControl *self) {
  CernControl *control = self;

  while (cern_control_get_parent_internal(control) != NULL) {
    control = cern_control_get_parent_internal(control);
  }

  return control;
}

gboolean
cern_control_get_show_keyboard_cues(CernControl *self) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  return klass->get_show_keyboard_cues(self);
}

gboolean
cern_control_get_show_focus_cues(CernControl *self) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  return klass->get_show_focus_cues(self);
}

gint32
cern_control_get_show_params(CernControl *self) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);

  return klass->get_show_params(self);
}

gboolean
cern_control_get_use_wait_cursor(CernControl *self) {
  return cern_control_get_state(self, CERN_CONTROL_STATE_USEWAITCURSOR);
}

void
cern_control_set_use_wait_cursor(CernControl *self, gboolean value) {
  if (cern_control_get_state(self, CERN_CONTROL_STATE_USEWAITCURSOR) != value) {
    cern_control_set_state(self, CERN_CONTROL_STATE_USEWAITCURSOR, value);

    CernPropertyStore *store = cern_control_get_properties(self);
    CernControlCollection *controls_collection
      = cern_property_store_get_object(store, CernPropControlsCollection);

    if (controls_collection != NULL) {
      for (gsize i = 0; i < cern_control_collection_get_count(controls_collection); i++) {
        CernControl *current_control
          = cern_control_collection_index(controls_collection, i);
        cern_control_set_use_wait_cursor(current_control, value);
      }
    }
  }
}

gboolean
cern_control_get_use_compatible_text_rendering_int(CernControl *self) {
  CernPropertyStore *store = cern_control_get_properties(self);

  if (cern_property_store_contains_integer(store, CernPropUseCompatibleTextRendering)) {
    gboolean found = FALSE;

    gint32 value = cern_property_store_try_get_integer(store, CernPropUseCompatibleTextRendering, &found);

    if (found) {
      return value == 1;
    }
  }

  return UseCompatibleTextRenderingDefault;
}

void
cern_control_set_use_compatible_text_rendering_int(CernControl *self, gboolean value) {
  if (cern_control_get_supports_use_compatible_text_rendering(self)
      && cern_control_get_use_compatible_text_rendering_int(self) != value) {
    CernPropertyStore *store = cern_control_get_properties(self);

    cern_property_store_set_integer(store, CernPropUseCompatibleTextRendering, value ? 1 : 0);
    cern_layout_transaction_do_layout_if(cern_control_get_auto_size(self),
                                         CERN_IARRANGED_ELEMENT(cern_control_get_parent_internal(self)),
                                         CERN_IARRANGED_ELEMENT(self),
                                         cern_property_names_use_compatible_text_rendering());
    cern_control_invalidate(self);
  }
}

gboolean
cern_control_get_supports_use_compatible_text_rendering(CernControl *self) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);

  return klass->get_support_use_compatible_text_rendering(self);
}

gboolean
cern_control_get_visible(CernControl *self) {
  return cern_control_get_visible_core(self);
}

void
cern_control_set_visible(CernControl *self, gboolean value) {
  cern_control_set_visible_core(self, value);
}

gint32
cern_control_get_width(CernControl *self) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);

  return priv->width;
}

void
cern_control_set_width(CernControl *self, gint32 value) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);

  cern_control_set_bounds(self, priv->x, priv->y, value, priv->height, CernBoundsSpecified_Width);
}

gint32
cern_control_get_window_style(CernControl *self) {
  return GetWindowLongA((HWND) cern_control_get_handle(self), GWL_STYLE);
}

void
cern_control_set_window_style(CernControl *self, gint32 value) {
  SetWindowLongA((HWND) cern_control_get_handle(self), GWL_STYLE, (LONG_PTR) value);
}

gchar *
cern_control_get_window_text(CernControl *self) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);

  return klass->get_window_text(self);
}

void
cern_control_set_window_text(CernControl *self, gchar *value) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);

  klass->set_window_text(self, value);
}

CernSize
cern_control_get_preferred_size(CernControl *self) {
  CernSize empty = { 0 };

  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  return klass->get_preferred_size(self, &empty);
}

CernPadding
cern_control_get_padding(CernControl *self) {
  CernPadding default_padding = cern_control_get_default_padding(self);

  return cern_common_properties_get_padding(CERN_IARRANGED_ELEMENT(self), &default_padding);
}

void
cern_control_set_padding(CernControl *self, CernPadding *value) {
  CernPadding current_padding = cern_control_get_padding(self);

  if (!cern_padding_equals(&current_padding, value)) {
    cern_common_properties_set_padding(CERN_IARRANGED_ELEMENT(self), value);
    cern_control_set_state(self, CERN_CONTROL_STATE_LAYOUTISDIRTY, TRUE);

    {
      CernLayoutTransation *lt
        = cern_layout_transacation_new(cern_control_get_parent_internal(self),
                                       CERN_IARRANGED_ELEMENT(self),
                                       cern_property_names_padding(), TRUE);
      CernEventArgs *args = cern_event_args_new();
      cern_control_on_padding_changed(self, args);
      g_clear_object(&args);
    }

    if (cern_control_get_state(self, CERN_CONTROL_STATE_LAYOUTISDIRTY)) {
      cern_layout_transaction_do_layout(CERN_IARRANGED_ELEMENT(self),
                                        CERN_IARRANGED_ELEMENT(self),
                                        cern_property_names_padding());
    }
  }
}

void
cern_control_assign_parent(CernControl *self, CernControl *parent) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);

  klass->assign_parent(self, parent);
}

void
cern_control_begin_update_internal(CernControl *self) {
  if (!cern_control_get_is_handle_created(self)) {
    return;
  }

  CernControlPrivate *priv = cern_control_get_instance_private(self);

  if (priv->update_count == 0) {
    SendMessageA((HWND) cern_control_get_handle(self),
                 WM_SET_REDRAW, (WPARAM) 0, (LPARAM) 0);
  }

  priv->update_count++;
}

void
cern_control_bring_to_front(CernControl *self) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);
  if (cern_control_get_parent(self) != NULL) {
    cern_control_collection_set_child_index(cern_control_get_controls(self), self, 0);
  } else if (cern_control_get_is_handle_created(self)
             && cern_control_get_top_level(self)
             && IsWindowEnable((HWND) cern_control_get_handle(self)) {
      SetWindowPos((HWND) cern_control_get_hanlde(self), HWND_TOP,
                   0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
  }
}

gboolean
cern_control_can_process_mnemonic(CernControl *self) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  return klass->can_process_mnemonic(self);
}

gboolean
cern_control_can_select_core(CernControl *self) {
  CernConrolClass *klass = CERN_CONTROL_GET_CLASS(self);

  klass->can_select_core(self);
}

void
cern_control_check_parenting_cycle(CernControl *bottom, CernControl *to_find) {
  CernWindow *last_owner = NULL;
  CernControl *last_parent = NULL;

  for (Control *ctl = bottom;
       ctl != NULL;
       ctl = cern_control_get_parent_internal(ctl)) {
    last_parent = ctl;

    if (ctl == to_find) {
      g_critical("%s(...): Circular owner", __func__);
      return;
    }
  }

  if (last_parent != NULL) {
    if (CERN_IS_WINDOW(last_parent)) {
      CernWindow *w = CERN_WINDOW(last_parent);

      for (CernWindow *window = w;
           window != NULL;
           window = cern_window_get_parent_internal(window)) {
        last_owner = window;
        if (window == to_find) {
          g_critical("%s(...): Circular owner", __func__);
          return;
        }
      }
    }
  }

  if (last_owner != NULL) {
    if (cern_window_get_parent_internal(last_owner) != NULL) {
      cern_control_check_parenting_cycle(cern_window_get_parent_internal(last_owner), to_find);
    }
  }
}

gboolean
cern_control_contains(CernControl *self, CernControl *control) {
  while (control != NULL) {
    control = cern_control_get_parent_internal(control);

    if (control == NULL) {
      return FALSE;
    }

    if (control == self) {
      return TRUE;
    }
  }

  return FALSE;
}

CernControlCollection *
cern_control_create_controls_instance(CernControl *self) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  return klass->create_controls_instance(self);
}

CernGraphics *
cern_control_create_graphics(CernControl *self) {
  return cern_control_create_graphics_internal(self);
}

CernGraphics *
cern_control_create_graphics_internal(CernControl *self) {
  return cern_graphics_new_from_hwnd(cern_control_get_handle(self));
}

void
cern_control_create_handle(CernControl *self) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  return klass->create_handle(self);
}

void
cern_control_create_control(CernControl *self) {
  gboolean control_is_already_created
    = cern_control_get_created(self);

  cern_control_create_control_internal(self, FALSE);

  CernPropertyStore *store = cern_control_get_properties(self);

  if (cern_property_store_get_object(store, CernPropBindingManager) == NULL
      && cern_control_get_parent_internal(self) != NULL
      && !control_is_already_created) {
    CernEventArgs *args = cern_event_args_new();
    cern_control_on_binding_context_changed(self, args);
    g_clear_object(&args);
  }
}

void
cern_control_create_control_internal(CernControl *self, gboolean ignore_visible) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);
  gboolean ready = (priv->state & CERN_CONTROL_STATE_CREATED) == 0;

  ready = ready && cern_control_get_visible(self);

  if (ready || ignore_visible) {
    priv->state |= CERN_CONTROL_STATE_CREATED;
    gboolean created_ok = FALSE;

    if (!cern_control_get_is_handle_created(self)) {
      cern_control_create_handle(self);
    }

    CernPropertyStore *store = cern_control_get_properties(self);

    CernControlCollection *controls_collection
      = cern_property_store_get_object(store, CernPropControlsCollection);

    if (controls_collection != NULL)  {
      CernControl **control_snapshot
        = g_new(CernControl*, cern_control_collection_get_count(controls_collection));
      cern_control_collection_copy_to_array(controls_collection, control_snapshot, 0);

      for (gsize i = 0; i < cern_control_collection_get_count(controls_collection); i++) {
        CernControl *ctl = control_snapshot[i];
        if (cern_control_get_is_handle_created(ctl)) {
          cern_control_set_parent_handle(ctl, cern_control_get_handle(self));
        }
      }
    }

    created_ok = TRUE;
  }

  cern_control_on_create_control(self);
}

void
cern_control_def_wnd_proc(CernControl *self, CernMessage *message) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  return klass->def_wnd_proc(self, message);
}

void
cern_control_destroy_handle(CernControl *self) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  klass->destroy_handle(self);
}

void
cern_control_draw_to_bitmap(CernControl *self, CernBitmap *bitmap, CernRectangle *target_bounds) {
  if (bitmap == NULL) {
    g_critical("Invalid bitmap.");
    return;
  }

  if (target_bounds->width <= 0
      || target_bounds->height <= 0
      || target_bounds->x < 0
      || target_bounds->y < 0) {
    g_critical("Invalid bounds");
    return;
  }

  if (!cern_control_get_is_handle_created(self)) {
    cern_control_create_handle(self);
  }

  gint32 width = MIN(cern_control_get_width(self), target_bounds->width);
  gint32 height = MIN(cern_control_get_height(self), target_bounds->height);

  CernBitmap *image
    = cern_bitmap_new_with_format(width, height, cern_bitmap_get_pixel_format(bitmap));
  CernGraphics *gfx
    = cern_graphics_new_from_image(image);

  gpointer h_dc = cern_graphics_get_hdc(gfx);
  SendMessageA((HWND) cern_control_get_handle(self),
               WM_PRINT, (WPARAM) h_dc,
               (LPARAM) PRF_CHILDREN | PRF_CLIENT | PRF_ERASEBKGND | PRF_NONCLIENT);

  CernGraphics *dest_gfx
    = cern_graphics_new_from_image(CERN_IMAGE(bitmap));

  gpointer dest_h_dc = cern_graphics_get_hdc(dest_gfx);
  BitBlt((HDC) dest_h_dc, target_bounds->x, target_bounds->y,
         width, height, h_dc, 0, 0, 0xcc0020);

  cern_graphics_release_hdc_value(dest_gfx, dest_h_dc);
  g_clear_object(&dest_gfx);

  cern_graphics_release_hdc_value(gfx, dest_h_dc);
  g_clear_object(&gfx);
}

gboolean
cern_control_end_update_internal(CernControl *self) {
 return cern_control_end_update_internal_invalidate(self, TRUE);
}

gboolean
cern_control_end_update_internal_invalidate(CernControl *self, gboolean invalidate) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);

  if (priv->update_count > 0) {
    priv->update_count--;
    cern_control_send_message(self, WM_SET_REDRAW, -1, -0);

    if (invalidate) {
      cern_control_invalidate(self);
    }

    return TRUE;
  }

  return FALSE;
}

CernWindow *
cern_control_find_window(CernControl *self) {
  return cern_control_find_window_internal(self);
}

CernWindow *
cern_control_find_window_internal(CernControl *self) {
  CernControl *cur = self;

  while (cur != NULL && !CERN_IS_WINDOW(cur)) {
    cur = cern_control_get_parent_internal(cur);
  }

  return CERN_WINDOW(cur);
}

gboolean
cern_control_get_top_level(CernControl *self) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);

  return priv->state & CERN_CONTROL_STATE_TOPLEVEL;
}

void
cern_control_raise_create_handle_event(CernControl *self, CernEventArgs *args) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);
  g_signal_emit(G_OBJECT(self), priv->create_handle_signal_id, 0, args);
}

void
cern_control_raise_key_event(CernControl *self, CernKeyEventArgs *args);

void
cern_control_raise_mouse_event(CernControl *self, CernMouseEventArgs *args);

gboolean
cern_control_focus(CernControl *self) {
  return cern_control_focus_internal(self);
}

gboolean
cern_control_focus_internal(CernControl *self) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  return klass->focus_inernal(self);
}

CernControl *
cern_control_from_child_handle(gpointer handle) {
  return cern_control_from_child_handle_internal(handle);
}

CernControl *
cern_control_from_child_handle_internal(gpointer handle) {
  while (handle != NULL) {
    CernControl *ctl = cern_control_from_handle_internal(handle);
    if (ctl != NULL) {
      return ctl;
    }

    handle = GetAncestor((HWND) handle, GA_PARENT);
  }

  return NULL;
}

CernControl *
cern_control_from_handle(gpointer handle) {
  return cern_control_from_handle_internal(handle);
}

CernControl *
cern_control_from_handle_internal(gpointer handle) {
  CernNativeWindow *w = cern_native_window_new_from_handle(handle);

  while (w != NULL && !CERN_IS_CONTROL_NATIVE_WINDOW(w)) {
    w = cern_native_window_get_previous_window(w);
  }

  if (CERN_IS_CONTROL_NATIVE_WINDOW(w)) {
    return cern_control_native_window_get_control(CERN_CONTROL_NATIVE_WINDOW(w));
  }

  return NULL;
}

CernSize
cern_control_apply_size_constraints(CernControl *self, gint32 width, gint32 height) {
  CernRectangle rect
    = cern_control_apply_bounds_constraints(self, 0, 0, width, heght);

  return cern_rectangle_get_size(&rect);
}

CernSize
cern_control_apply_size_constraints_size(CernControl *self, CernSize *size) {
  CernRectangle rect
    = cern_control_apply_bounds_constraints(self, 0, 0, size->width, size->height);

  return cern_rectangle_get_size(*rect);
}

CernRectangle
cern_control_apply_bounds_constraints(CernControl *self,
                                      gint32 suggested_x, gint32 suggested_y,
                                      gint32 proposed_width, gint32 proposed_height) {
  CernSize minimum_size = cern_control_get_minimum_size(self);
  CernSize maximum_size = cern_control_get_maximum_size(self);
  CernSize empty_size = { 0 };

  if (!cern_size_equals(&minimum_size, empty_size)
      || !cern_size_equals(&maximum_size, &empty_size)) {
    CernSize max_size
      = cern_layouts_utils_convert_zero_to_unbounded(&maximum_size);

    CernRectangle new_bounds = {
      .x = suggested_x,
      .y = suggested_y,
      .width = 0,
      .height = 0
    };

    CernSize proposed_size = {
      .width = proposed_width,
      .height = proposed_height
    };

    CernSize temp = cern_layout_utils_intersect_sizes(&proposed_size, &minimum_size);
    cern_rectangle_set_size(&new_bounds, &temp);

    CernSize temp2 = cern_rectangle_get_size(&new_bounds);

    temp = cern_layout_utils_union_sizes(&temp2, &minimum_size);
    cern_rectangle_set_size(&new_bounds, &temp);
    return new_bounds;
  }

  return (CernRectangle) {
    .x = suggested_x,
    .y = suggested_y,
    .width = proposed_width,
    .height = proposed_height
  };
}

CernControl *
cern_control_get_child_at_point_skip(CernControl *self, CernPoint *point,
                                     CernGetChildAtPointSkip skip) {
  gint32 value = (gint32) skip;

  if (value < 0 || value > 7) {
    g_critical("%s(...): Invalid skip value.", __func__);
  }

  POINT pt = {
    .x = point->x,
    .y = point->y
  };

  HWND h_wnd = ChildWindowFromPointEx((HWND) cern_control_get_handle(self), &pt, value);

  CernControl *ctl = cern_control_from_child_handle((gpointer) h_wnd);

  return (ctl == self) ? NULL : ctl;
 }

CernControl *
cern_control_get_child_at_point(CernControl *self, CernPoint *point) {
  return cern_control_get_child_at_point_skip(self, point, CernGetChildAtPointSkip_None);
}

CernIContainerControl *
cern_control_get_container_control(CernControl *self) {
  return cern_control_get_container_control_internal(self);
}

gboolean
cern_control_is_updating_internal(CernControl *self) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);

  return priv->update_count > 0;
}

static
gboolean
cern_control_is_focus_managing_container_control(CernControl *self) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);

  return
    priv->control_style & CernControlStyles_ContainerControl
      && CERN_IS_CONTAINER_CONTROL(self);
}

CernIContainerControl *
cern_control_get_container_control_internal(CernControl *self) {
  CernControl *c = self;

  if (c != NULL
      && cern_control_get_is_container_control(c)) {
    c = cern_control_get_parent_internal(c);
  }

  while (c != NULL
          && cern_control_is_focus_managing_container_control(c)) {
    c = cern_control_get_parent_internal(c);
  }

  return (CernIContainerControl *) c;
}

gpointer
cern_control_get_h_rgn_internal(CernControl *self, CernRegion *region) {
  CernGraphics *gfx = cern_control_create_graphics_internal(self);

  gpointer handle = cern_region_get_h_region(region, gfx);
  g_clear_object(&gfx);

  return handle;
}

CernRectangle
cern_control_get_scaled_bounds(CernControl *self, CernRectangle *bounds,
                               CernSizeF *factor, CernBoundsSpecified specified) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  return klass->get_scaled_bounds(self, bounds, factor, specified);
}

gboolean
cern_control_get_visible_core(CernControl *self) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  return klass->get_visible_core(self);
}

static
gint
real_cern_control_array_sort(gconstpointer a, gconstpointer b) {
  CernControlTabOrderHolder *hx = (gpointer) a;
  CernControlTabOrderHolder *hy = (gpointer) b;

  gint32 delta
    = cern_control_tab_order_holder_get_new_order(hx)
       - cern_control_tab_order_holder_get_new_order(hy);

  if (delta == 0) {
    delta
      = cern_control_tab_order_holder_get_old_order(hx)
          - cern_control_tab_order_holder_get_old_order(hy);
  }

  return delta;
}

static
GPtrArray *
cern_control_get_child_controls_in_tab_order_list(CernControl *self, gboolean handle_created_only) {
  GPtrArray *array = g_ptr_array_new_with_free_func(g_object_unref);

  CernControlCollection *controls
    = cern_control_get_controls(self);

  gsize controls_count
    = cern_control_collection_get_count(controls);

  for (gsize i = 0; i < controls_count; i++) {
    CernControl *ctl = cern_control_collection_get_by_index(collections, i);

    if (!handle_created_only || cern_control_get_is_handle_created(ctl)) {
      CernControlTabOrderHolder *holder
        = cern_control_tab_order_holder_new(array->len, cern_control_get_tab_index(ctl), ctl);
      g_ptr_array_add(array, holder);
    }
  }

  g_ptr_array_sort_values(array, real_cern_control_array_sort);

  return array;
}

static
GPtrArray *
cern_control_get_child_windows(HWND parent) {
  GPtrArray *windows = g_ptr_array_new();

  for (HWND child = GetWindow(parent, GW_CHILD);
       child != NULL;
       child = GetWindow(child, GW_HWNDNEXT)) {
    g_ptr_array_add(windows, (gpointer) child);
  }

  return windows;
}

static
void
cern_control_child_windows_add_to_holders(gpointer item, gpointer user_data) {
  GPtrArray *holders = user_data;

  CernControl *ctl = cern_control_from_handle_internal(item);
  gint32 tab_index
    = (ctl == NULL)
        ? -1
        : cern_control_get_tab_index(ctl);

  g_ptr_array_add(holders, cern_control_tab_order_holder_new(holders->len, tab_index, ctl));
}

static
GPtrArray *
cern_control_get_child_windows_tab_order_list(CernControl *self) {
  GPtrArray *holders = g_ptr_array_new_with_free_func(g_object_unref);
  GPtrArray *windows
    = cern_control_get_child_windows((HWND) cern_control_get_handle(self));

  g_ptr_array_foreach(windows, cern_control_child_windows_add_to_holders, holders);
  g_ptr_array_sort_values(holders, real_cern_control_array_sort);

  return holders;
}

static
void
child_windows_fill(gpointer item, gpointer data) {
  GArray indexes = data;
}

GArray *
cern_control_get_child_windows_in_tab_order(CernControl *self) {
  GPtrArray *holders
    = cern_control_get_child_windows_tab_order_list(self);

  gsize count = holders->len;

  GArray *indexes = g_array_new(FALSE, TRUE, sizeof(gint32));

  g_ptr_array_foreach(holders, child_windows_fill, indexes);

  g_clear_object(&holders);

  return indexes;
}

GPtrArray *
cern_control_get_child_controls_in_tab_order_internal(CernControl *self, gboolean handle_crated_only) {
  GPtrArray *holders
    = cern_control_get_child_controls_in_tab_order_list(self, handle_crated_only);

  GPtrArray *ctls = g_ptr_array_new_with_free_func(g_object_unref);

  for (gsize i = 0; i < holders->len; i++) {
    CernControlTabOrderHolder *holder = ((CernControlTabOrderHolder **) holders->pdata)[i];
    g_ptr_array_add(ctls, cern_control_tab_order_holder_get_control(holder));
  }

  g_clear_object(&holders);

  return ctls;
}

CernControl *
cern_control_get_first_child_control_in_tab_order(CernControl *self, gboolean forward) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  return klass->get_first_child_control_in_tab_order(self, forward);
}

CernControl *
cern_control_get_next_control(CernControl *self, CernControl *ctl, gboolean forward) {
  CernPropertyStore *store = cern_control_get_properties(self);
  if (!cern_control_contains(self, ctl)) {
    ctl = self;
  }

  if (forward) {
    CernControlCollection *ctl_collection
      = cern_property_store_get_object(store, CernPropControlsCollection);

    if (ctl_collection != NULL
        && cern_control_collection_get_count(ctl_collection) > 0
        && (ctl == self && !cern_control_is_focus_managing_container_control(ctl))) {
      CernControl *found = cern_control_get_first_child_control_in_tab_order(ctl, forward);
      if (found != NULL) {
        return found;
      }
    }

    while (ctl != self) {
      gint32 target_index = cern_control_get_tab_index(ctl);
      gboolean hit_ctl = FALSE;
      CernControl *found = NULL;
      CernControl *p = cern_control_get_parent(ctl);

      gint32 parent_controls_count = 0;

      CernPropertyStore *parent_store = cern_control_get_properties(p);
      CernControlCollection *parent_controls
        = cern_property_store_get_color(parent_store, CernPropControlsCollection);

      if (parent_controls != NULL) {
        parent_controls_count
          = cern_control_collection_get_count(parent_controls);
      }

      for (gsize c = 0; c < parent_controls_count; c++) {
        CernControl *cursor = cern_control_collection_get_by_index(parent_controls, c);
        if (cursor != ctl) {
          if (cern_control_get_tab_index(cursor) >= target_index) {
            if (found == NULL
                || cern_control_get_tab_index(found) > cern_control_get_tab_index(cursor)) {
              if (cern_control_get_tab_index(cursor) != target_index || hit_ctl) {
                found = cursor;
              }
            }
          }
        } else {
          hit_ctl = TRUE;
        }
      }

      if (found != NULL) {
        return found;
      }

      ctl = cern_control_get_parent(ctl);
    }
  } else {
    if (ctl != self) {
      gint32 target_index = cern_control_get_tab_index(ctl);
      gboolean hit_ctl = FALSE;
      CernControl *found = NULL;
      CernControl *p = cern_control_get_parent(ctl);

      gsize parent_controls_count = 0;

      CernPropertyStore *parent_store
        = cern_control_get_properties(p);
      CernControlCollection *parent_controls
        = cern_property_store_get_object(parent_store, CernPropControlsCollection);

      if (parent_controls != NULL) {
        parent_controls_count
          = cern_control_collection_get_count(parent_controls);
      }

      for (gsize c = parent_controls_count - 1; c >= 0; c--) {
        CernControl *cursor
          = cern_control_collection_get_by_index(parent_control, c);
        if (cursor != ctl) {
          if (cern_control_get_tab_index(cursor)<= target_index) {
            if (found == NULL
                || cern_control_get_tab_index(found) < cern_control_get_tab_index(cursor)) {
              if (cern_control_get_tab_index(cursor) != target_index || hit_ctl) {
                found = cursor;
              }
            }
          }
        } else {
          hit_ctl = TRUE;
        }
      }

      if (found != NULL) {
        ctl = found;
      } else {
        if (p == self) {
          return NULL;
        } else {
          return p;
        }
      }
    }

    CernPropertyStore *ctl_store
      = cern_control_get_properties(ctl);
    CernControlCollection *ctl_controls
      = cern_property_store_get_object(ctl_store, CernPropControlsCollection);
    while (ctl_controls != NULL && cern_control_collection_get_count(ctl_controls) > 0
           && (ctl == self || cern_control_is_focus_managing_container_control(ctl))) {
      CernControl *found
        = cern_control_get_first_child_control_in_tab_order(ctl, FALSE);
      if (found != NULL) {
        ctl = found;
        ctl_store = cern_control_get_properties(ctl);
        ctl_controls = cern_property_store_get_object(ctl_store, CernPropControlsCollection);
      } else {
        break;
      }
    }
  }

  return ctl == self ? NULL : ctl;
}

gpointer
cern_control_get_safe_handle_internal(CernWin32Window *window);

gboolean
cern_control_get_state(CernControl *self, gint32 flag) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);
  return (priv->state & flag) != 0;
}

gboolean
cern_control_get_style(CernControl *self, CernControlStyles style) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);
  return (priv->control_style & style) == style;
}

void
cern_control_hide(CernControl *self) {
  cern_control_set_visible(self, FALSE);
}

void
cern_control_init_layout(CernControl *self) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  klass->init_layout(self);
}

gpointer
cern_control_initialize_dc_for_wm_ctl_color(CernControl *self, gpointer dc, guint32 msg) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  return klass->initialize_dc_for_wm_ctl_color(self, dc, msg);
}

void
cern_control_invalidate_region(CernControl *self, CernRectangle *region) {
  cern_control_invalidate_region_ex(self, region, FALSE);
}

void
cern_control_invalidate_region_ex(CernControl *self, CernRectangle *region, gboolean invalidat_children) {
  if (region == NULL) {
    cern_control_invalidate_ex(self, invalidate_children);
  } else if (cern_control_get_is_handle_created(self)) {
    gpointer region_handle = cern_control_get_h_rgn_internal(self, region);

    if (invalidate_children) {
      RedrawWindow((HWND) cern_control_get_handle(self),
                   NULL, (HRGN) region_handle,
                   RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN);

    } else {
      InvalidateRgn((HWND) cern_control_get_handle(self),
                    (HRGN) region_handle,
                    !cern_control_get_style(self, CernControlStyles_Opaque));
    }

    DeleteObject((HGDIOBJ) region_handle);

    CernRectangle bounds = { 0 };

    CernGraphics *gfx = cern_control_create_graphics_internal(self);

    {
      CernRectangleF f = cern_region_get_bounds_f(region, gfx);
      bounds.x = (gint32) ceilf(f.x);
      bounds.y = (gint32) ceilf(f.y);
      bounds.width = (gint32) ceilf(f.width);
      bounds.height = (gint32) ceilf(f.height);
    }

    g_clear_object(&gfx);

    CernInvalidateEventArgs *args = cern_invalidate_event_args_new(&bounds);
    cern_control_on_invalidated(self, args);
    g_clear_object(&args);
  }
}

void
cern_control_invalidate(CernControl *self) {
  cern_control_invalidate_ex(self, FALSE);
}

void
cern_control_invalidate_ex(CernControl *self, gboolean invalidate_children) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);
  if (cern_control_get_is_handle_created(self)) {
    if (invalidate_children) {
      RedrawWindow((HWND) cern_control_get_handle(self),
                   NULL, NULL,
                   RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN);
    } esle {
      InvalidateRect((HWND) cern_control_get_handle(self),
                     NULL, (priv->control_style & CernControlStyles_Opaque) != CernControlStyles_Opaque);
    }

    CernRectangle self_rect = cern_control_get_client_rectangle(self);
    cern_control_notify_invalidate(self, &self_rect);
  }
}

void
cern_control_invalidate_rect(CernControl *self, CernRectangle *rect) {
  cern_control_invalidate_rect_ex(self, rect, FALSE);
}

void
cern_control_invalidate_rect_ex(CernControl *self, CernRectangle *rect, gboolean invalidate_children) {
  if (cern_rectangle_is_empty(rect)) {
    cern_control_invalidate_ex(self, invalidate_children);
  } else if (cern_control_get_is_handle_created(self)) {
    if (invalidate_children) {
      RECT rc_area = {
        .left = rect->x,
        .top = rect->y,
        .right = rect->x + rect->width,
        .bottom = rect.y + rect->height
      };

      RedrawWindow((HWND) cern_control_get_handle(self),
                   &rc_area, NULL,
                   RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN);
    } else {
      CernControlPrivate *priv = cern_control_get_instance_private(self);
      RECT rc_area = {
        .left = rect->x,
        .top = rect->y,
        .right = rect->x + rect->width,
        .bottom = rect.y + rect->height
      };

      InvalidateRect((HWND) cern_control_get_handle(self), &rc_area,
                     (priv->control_style & CernControlStyles_Opaque) != CernControlStyles_Opaque);
    }

    cern_control_notify_invalidate(self, rect);
  }
}

void
cern_control_invoke_paint(CernControl *self, CernContol *ctl, CernPaintEventArgs *args) {
  cern_control_on_paint(ctl, args);
}

void
cern_control_invoke_paint_background(CernControl *self, CernControl *ctl, CernPaintEventArgs *args) {
  cern_conrol_on_paint_background(ctl, args);
}

gboolean
cern_control_is_font_set_internal(CernControl *self) {
  CernPropertyStore *store = cern_control_get_properties(self);
  CernFont *font = cern_property_store_get_object(store, CernPropFont);
  return font != NULL;
 }

gboolean
cern_control_is_descendant_internal(CernControl *self, CernControl *control) {
  CernContol *descendant = control;

  while (descendant != NULL) {
    if (self == descendant) {
      return TRUE;
    }

    descendant = cern_control_get_parent_internal(descendant);
  }
  return FALSE;
}

gboolean
cern_control_is_key_locked(CernKeys key_val) {
  if (key_val == CernKeys_Insert
      || key_val == CernKeys_CapsLock
      || key_val == CernKeys_NumLock
      || key_val == CernKeys_Scroll) {
    gint32 result = GetKeyState((int) key_val);

    if (key_val == CernKeys_Insert
        || key_val == CernKeys_CapsLock) {
      return (result & 0x1) != 0x0;
    }

    return (result & 0x8001) != 0x0;
  }

  g_critical("%s(...): Not supported...", __func__);
}

gboolean
cern_control_is_input_char(CernControl *self, gchar ch) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);

  return klass->is_input_char(self, ch);
}

gboolean
cern_control_is_input_key(CernControl *self, CernKeys key_data) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);

  return klass->is_input_key(self, key_data);
}

gboolean
cern_control_is_mnemonic(CernControl *self, gchar char_code, gchar const *text) {
  if (char_code == '&') {
    return FALSE;
  }

  if (text != NULL) {
    gint32 pos = -1;
    gchar c2 = g_ascii_toupper(char_code);
    gsize text_len = g_utf8_strlen(text, 0);

    for (;;) {
      if ((pos + 1) >= text_len) {
        break;
      }

      gchar *pos = g_utf8_strchr(text + pos + 1, text_len, '&');
      pos = pos - text;

      if (pos <= 0 || pos >= text_len) {
        break;
      }

      gchar c1 = g_ascii_toupper(text[pos]);

      if (c1 == c2 || g_ascii_tolower(c1) == g_ascii_tolower(c2)) {
        return TRUE;
      }
    }
  }

  return FALSE;
}

gint32
cern_control_logical_to_device_units(CernControl *self, gint32 logical_value) {
  return cern_dpi_helper_logical_to_device_units(value, cern_control_get_device_dpi(self));
}

CernSize
cern_control_logical_to_device_units_size(CernControl *self, CernSize *logical_size) {
  return cern_dpi_helper_logical_to_device_units_size(logical_size, cern_control_get_device_dpi(self));
}

void
cern_control_scale_bitmap_logical_to_device(CernControl *self, CernBitmap *bitmap) {
  cern_dpi_helper_logical_to_device_bitmap(bitmap, cern_control_get_device_dpi(self));
}

void
cern_control_adjust_window_rect_ex(CernRectangle *rect, gint32 style,
                                   gboolean menu, gint32 ex_style) {
  CernControlPrivate *priv = cern_control_get_instance_private(self);
  RECT rect = {
    .left = rect->x,
    .top = rect->y,
    .right = rect->x + rect->width,
    .bottom = rect->y + rect->height
  };

  if (cern_dpi_helper_enable_dpi_changed_message_handling()) {
    AdjustWindowRectExForDpi(&rect, style, menu, ex_style, priv->device_dpi);
  } else {
    AdjustWindowRectEx(&rect, style, menu, ex_style);
  }
}

void
cern_control_notify_enter_internal(CernControl *self) {
  CernEventArgs *args = cern_event_args_new();
  cern_control_on_enter(self, args);
  g_clear_object(&args);
}

void
cern_control_notify_leave_internal(CernControl *self) {
  CernEventArgs *args = cern_event_args_new();
  cern_control_on_leave(self, args);
  g_clear_object(&args);
}

void
cern_control_notify_invalidate(CernControl *self, CernRectangle *rect) {
  CernInvalidateEventArgs *args = cern_invalidate_event_args_new(rect);
  cern_control_on_invalidated(self, args);
  g_clear_object(&args);
}

void
cern_control_on_background_color_changed(CernControl *self, CernEventArgs *args) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  klass->on_background_color_changed(self, args);
}

void
cern_control_on_background_image_changed(CernControl *self, CernEventArgs *args) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  klass->on_background_image_changed(self, args);
}

void
cern_control_on_background_image_layout_changed(CernControl *self, CernEventArgs *args) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  klass->on_background_image_layout_changed(self, args);
}

void
cern_control_on_binding_context_changed(CernControl *self, CernEventArgs *args) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  klass->on_binding_context_changed(self, args);
}

void
cern_control_on_causes_validation_changed(CernControl *self, CernEventArgs *args) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  klass->on_causes_validation_changed(self, args);
}

void
cern_control_on_child_layout_resuming(CernControl *self, CernControl *child, gboolean perform_layout) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  klass->on_child_layout_resuming(self, child, perform_layout);
}

void
cern_control_on_context_menu_changed(CernControl *self, CernEventArgs *args) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  klass->on_context_menu_changed(self, args);
}

void
cern_control_on_context_menu_strip_changed(CernControl *self, CernEventArgs *args) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  klass->on_context_menu_strip_changed(self, args);
}

void
cern_control_on_cursor_changed(CernControl *self, CernEventArgs *args) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  klass->on_cursor_changed(self, args);
}

void
cern_control_on_dock_changed(CernControl *self, CernEventArgs *args) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  klass->on_dock_changed(self, args);
}


void
cern_control_on_enabled_changed(CernControl *self, CernEventArgs *args) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  klass->on_enabled_changed(self, args);
}

void
cern_control_on_frame_window_activate(CernControl *self, gboolean activate) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  klass->on_frame_window_activate(self, activate);
}

void
cern_control_on_font_changed(CernControl *self, CernEventArgs *args) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  klass->on_font_changed(self, args);
}

void
cern_control_on_fore_color_changed(CernControl *self, CernEventArgs *args) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  klass->on_fore_color_changed(self, args);
}

void
cen_control_on_right_to_left_changed(CernControl *self, CernEventArgs *args) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  klass->on_right_to_left_changed(self, args);
}

void
cern_control_on_notify_message(CernControl *self, CernMessage *message) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  klass->on_notify_message(self, message);
}

void
cern_control_on_parent_back_color_changed(CernControl *self, CernEventArgs *args) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  klass->on_parent_back_color_changed(self, args);
}

void
cern_control_on_parent_background_image_changed(CernControl *self, CernEventArgs *args) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  klass->on_parent_background_image_changed(self, args);
}

void
cern_control_on_parent_binding_context_changed(CernControl *self, CernEventArgs *args) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  klass->on_parent_binding_context_changed(self, args);
}

void
cern_control_on_parent_cursor_changed(CernControl *self, CernEventArgs *args) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  klass->on_parent_cursor_changed(self, args);
}

void
cern_control_on_parent_enabled_changed(CernControl *self, CernEventArgs *args) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  klass->on_parent_enabled_changed(self, args);
}

void
cern_control_on_parent_font_changed(CernControl *self, CernEventArgs *args) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  klass->on_parent_font_changed(self, args);
}

void
cern_control_on_parent_handle_recreated(CernControl *self) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  klass->on_parent_handle_recreated(self);
}

void
cern_control_on_parent_handle_recreating(CernControl *self) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  klass->on_parent_handle_recreating(self);
}

void
cern_control_on_parent_fore_color_changed(CernControl *self, CernEventArgs *args) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  klass->on_parent_fore_color_changed(self, args);
}

void
cern_control_on_parent_right_to_left_changed(CernControl *self, CernEventArgs *args) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  klass->on_parent_right_to_left_changed(self, args);
}

void
cern_control_on_parent_visible_changed(CernControl *self, CernEventArgs *args) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  klass->on_parent_visible_changed(self, args);
}

void
cern_control_on_parent_became_invisible(CernControl *self) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  klass->on_parent_became_invisible(self);
}

void
cern_control_on_paint(CernControl *self, CernPaintEventArgs *args) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  klass->on_paint(self, args);
}

void
cern_control_on_tab_index_changed(CernControl *self, CernEventArgs *args) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  klass->on_tab_index_changed(self, args);
}

void
cern_control_on_tab_stop_changed(CernControl *self, CernEventArgs *args) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  klass->on_tab_stop_changed(self, args);
}

void
cern_control_on_text_changed(CernControl *self, CernEventArgs *args) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  klass->on_text_changed(self, args);
}

void
cern_control_on_visible_changed(CernControl *self, CernEventArgs *args) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  klass->on_visible_changed(self, args);
}

void
cern_control_on_parent_changed(CernControl *self, CernEventArgs *args) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  klass->on_parent_changed(self, args);
}

void
cern_control_on_click(CernControl *self, CernEventArgs *args) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  klass->on_click(self, args);
}

void
cern_control_on_client_size_changed(CernControl *self, CernEventArgs *args) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  klass->on_client_size_changed(self, args);
}

void
cern_control_on_control_added(CernControl *self, CernEventArgs *args) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  klass->on_control_added(self, args);
}

void
cern_control_on_control_removed(CernControl *self, CernEventArgs *args) {
  CernControlClass *klass = CERN_CONTROL_GET_CLASS(self);
  klass->on_control_removed(self, args);
}

