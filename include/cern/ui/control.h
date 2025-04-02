#ifndef CERN_UI_CONTROL_H
#define CERN_UI_CONTROL_H 1

#include <glib-object.h>

G_BEGIN_DECLS

#include "cern/drawing/image.h"
#include "cern/drawing/color.h"
#include "dock_style.h"
#include "property_store.h"
#include "control_collection.h"
#include "control_bindings_collection.h"
#include "layout/layout_engine.h"
#include "auto_size_mode.h"
#include "context_menu.h"
#include "cern/drawing/rectangle.h"
#include "component_model/component.h"
#include "component_model/cancel_event_args.h"
#include "component_model/event_args.h"
#include "component_model/control_event_args.h"
#include "component_model/invalidate_event_args.h"
#include "component_model/paint_event_args.h"
#include "cern/drawing/bitmap.h"
#include "cern/drawing/point.h"
#include "create_params.h"
#include "get_child_at_point_skip.h"
#include "component_model/layout_event_args.h"
#include "right_to_left.h"
#include "control_styles.h"
#include "anchor_styles.h"
#include "padding.h"
#include "cursor.h"
#include "mouse_buttons.h"
#include "cern/drawing/size_f.h"
#include "win32/window.h"
#include "keys.h"
#include "image_layout.h"
#include "ambient_properties.h"
#include "message.h"
#include "horizontal_alignment.h"
#include "left_right_alignment.h"
#include "content_alignment.h"

#define CERN_CONTROL_STATE_CREATED                            (0x00000001)
#define CERN_CONTROL_STATE_VISIBLE                            (0x00000002)
#define CERN_CONTROL_STATE_ENABLED                            (0x00000004)
#define CERN_CONTROL_STATE_TABSTOP                            (0x00000008)
#define CERN_CONTROL_STATE_RECREATE                           (0x00000010)
#define CERN_CONTROL_STATE_MODAL                              (0x00000020)
#define CERN_CONTROL_STATE_ALLOWDROP                          (0x00000040)
#define CERN_CONTROL_STATE_DROPTARGET                         (0x00000080)
#define CERN_CONTROL_STATE_NOZORDER                           (0x00000100)
#define CERN_CONTROL_STATE_LAYOUTDEFERRED                     (0x00000200)
#define CERN_CONTROL_STATE_USEWAITCURSOR                      (0x00000400)
#define CERN_CONTROL_STATE_DISPOSED                           (0x00000800)
#define CERN_CONTROL_STATE_DISPOSING                          (0x00001000)
#define CERN_CONTROL_STATE_MOUSEENTERPENDING                  (0x00002000)
#define CERN_CONTROL_STATE_TRACKINGMOUSEEVENT                 (0x00004000)
#define CERN_CONTROL_STATE_THREADMARSHALLPENDING              (0x00008000)
#define CERN_CONTROL_STATE_SIZELOCKEDBYOS                     (0x00010000)
#define CERN_CONTROL_STATE_CAUSESVALIDATION                   (0x00020000)
#define CERN_CONTROL_STATE_CREATINGHANDLE                     (0x00040000)
#define CERN_CONTROL_STATE_TOPLEVEL                           (0x00080000)
#define CERN_CONTROL_STATE_ISACCESSIBLE                       (0x00100000)
#define CERN_CONTROL_STATE_OWNCTLBRUSH                        (0x00200000)
#define CERN_CONTROL_STATE_EXCEPTIONWHILEPAINTING             (0x00400000)
#define CERN_CONTROL_STATE_LAYOUTISDIRTY                      (0x00800000)
#define CERN_CONTROL_STATE_CHECKEDHOST                        (0x01000000)
#define CERN_CONTROL_STATE_HOSTEDINDIALOG                     (0x02000000)
#define CERN_CONTROL_STATE_DOUBLECLICKFIRED                   (0x04000000)
#define CERN_CONTROL_STATE_MOUSEPRESSED                       (0x08000000)
#define CERN_CONTROL_STATE_VALIDATIONCANCELLED                (0x10000000)
#define CERN_CONTROL_STATE_PARENTRECREATING                   (0x20000000)
#define CERN_CONTROL_STATE_MIRRORED                           (0x40000000)

#define CERN_CONTROL_STATE2_HAVEINVOKED                       (0x00000001)
#define CERN_CONTROL_STATE2_SETSCROLLPOS                      (0x00000002)
#define CERN_CONTROL_STATE2_LISTENINGTOUSERPREFERENCECHANGED  (0x00000004)
#define CERN_CONTROL_STATE2_INTERESTEDINUSERPREFERENCECHANGED (0x00000008)
#define CERN_CONTROL_STATE2_MAINTAINSOWNCAPTUREMODE           (0x00000010)
#define CERN_CONTROL_STATE2_BECOMINGACTIVECONTROL             (0x00000020)
#define CERN_CONTROL_STATE2_CLEARLAYOUTARGS                   (0x00000040)
#define CERN_CONTROL_STATE2_INPUTKEY                          (0x00000080)
#define CERN_CONTROL_STATE2_INPUTCHAR                         (0x00000100)
#define CERN_CONTROL_STATE2_UICUES                            (0x00000200)
#define CERN_CONTROL_STATE2_ISACTIVEX                         (0x00000400)
#define CERN_CONTROL_STATE2_USEPREFERREDSIZECACHE             (0x00000800)
#define CERN_CONTROL_STATE2_TOPMDIWINDOWCLOSING               (0x00001000)
#define CERN_CONTROL_STATE2_CURRENTLYBEINGSCALED              (0x00002000)

typedef struct _CernBindingContext CernBindingContext;
typedef struct _CernKeyEventArgs CernKeyEventArgs;
typedef struct _CernKeyPressEventArgs CernKeyPressEventArgs;
typedef struct _CernMouseEventArgs CernMouseEventArgs;
typedef struct _CernPreviewKeyDownEventArgs CernPreviewKeyDownEventArgs;
typedef struct _CernUICuesEventArgs CernUICuesEventArgs;
typedef struct _CernWindow CernWindow;
typedef struct _CernIContainerControl CernIContainerControl;
typedef struct _CernContextMenuStrip CernContextMenuStrip;
typedef struct _CernDragEventArgs CernDragEventArgs;
typedef struct _CernHelpEventArgs CernHelpEventArgs;
typedef struct _CernGiveFeedbackEventArgs CernGiveFeedbackEventArgs;
typedef struct _CernQueryContinueEventArgs CernQueryContinueDragEventArgs;
typedef enum _CernPreProcessControlState CernPreProcessControlState;
typedef gint32 CernValidationConstraints;
typedef struct _CernContainerControl CernContainerControl;
typedef struct _CernToolStripControlHost CernToolStripControlHost;
typedef struct _CernToolTip CernToolTip;

#define CERN_TYPE_CONTROL cern_control_get_type()

G_DECLARE_DERIVABLE_TYPE(CernControl, cern_control,
                         CERN, CONTROL, CernComponentModelComponent);

struct _CernControlClass {
  CernComponentModelComponentClass parent_class;

  void
  (*set_allow_drop)(CernControl *self, gboolean allow_drop);

  gboolean
  (*get_allow_drop)(CernControl *self);

  CernAnchorStyles
  (*get_anchor)(CernControl *self);

  void
  (*set_anchor)(CernControl *self, CernAnchorStyles anchor);

  gboolean
  (*get_auto_size)(CernControl *self);

  void
  (*set_auto_size)(CernControl *self, gboolean auto_size);

  CernPoint
  (*get_auto_scroll_offset)(CernControl *self);

  void
  (*set_auto_scroll_offset)(CernControl *self, CernPoint *offset);

  CernLayoutEngine *
  (*get_layout_engine)(CernControl *self);

  CernColor
  (*get_back_color)(CernControl *self);

  void
  (*set_back_color)(CernControl *self, CernColor *back_color);

  CernImage *
  (*get_background_image)(CernControl *self);

  void
  (*set_background_image)(CernControl *self, CernImage *background_image);

  CernImageLayout
  (*get_background_image_layout)(CernControl *self);

  void
  (*set_background_image_layout)(CernControl *self, CernImageLayout layout);

  CernBindingContext *
  (*get_binding_context)(CernControl *self);

  void
  (*set_binding_context)(CernControl *self, CernBindingContext *context);

  gboolean
  (*get_can_access_properties)(CernControl *self);


  CernContextMenu *
  (*get_context_menu)(CernControl *self);

  void
  (*set_context_menu)(CernControl *self, CernContextMenu *context_menu);

  CernContextMenuStrip *
  (*get_context_menu_strip)(CernControl *self);

  void
  (*set_context_menu_strip)(CernControl *self, CernContextMenuStrip *context_menu_strip);

  CernCreateParams *
  (*get_create_params)(CernControl *self);

  void
  (*notify_validation_result)(CernControl *sender, CernCancelEventArgs *result);

  CernCursor *
  (*get_cursor)(CernControl *self);

  void
  (*set_cursor)(CernControl *self, CernCursor *cursor);

  CernCursor *
  (*get_default_cursor)(CernControl *self);

  CernPadding
  (*get_default_margins)(CernControl *self);

  CernSize
  (*get_default_maximum_size)(CernControl *self);

  CernSize
  (*get_default_minimum_size)(CernControl *self);

  CernPadding
  (*get_default_padding)(CernControl *self);

  CernSize
  (*get_default_size)(CernControl *self);

  CernRectangle
  (*get_display_rectangle)(CernControl *self);

  CernDockStyle
  (*get_dock)(CernControl *self);

  void
  (*set_dock)(CernControl *self, CernDockStyle dock);

  gboolean
  (*get_double_buffered)(CernControl *self);

  void
  (*set_double_buffered)(CernControl *self, gboolean double_buffered);

  gboolean
  (*get_focused)(CernControl *self);

  CernFont *
  (*get_font)(CernControl *self);

  void
  (*set_font)(CernControl *self, CernFont *font);

  CernColor
  (*get_fore_color)(CernControl *self);

  void
  (*set_fore_color)(CernControl *self, CernColor *fore_color);

  CernSize
  (*get_preferred_size)(CernControl *self, CernSize *proposed_size);

  CernSize
  (*get_preferred_size_core)(CernControl *self, CernSize *proposed_size);

  gboolean
  (*get_has_children)(CernControl *self);

  gboolean
  (*get_is_container_control)(CernControl *self);

  CernSize
  (*get_maximum_size)(CernControl *self);

  void
  (*set_maximum_size)(CernControl *self, CernSize maximum_size);

  CernSize
  (*get_minimum_size)(CernControl *self);

  void
  (*set_minimum_size)(CernControl *self, CernSize minimum_size);

  CernControl *
  (*get_parent_internal)(CernControl *self);

  void
  (*set_parent_internal)(CernControl *self, CernControl *parent);

  void
  (*add_reflect_child)(CernControl *self);

  void
  (*remove_reflect_child)(CernControl *self);

  gboolean
  (*get_render_transparency_with_visual_styles)(CernControl *self);

  CernRightToLeft
  (*get_right_to_left)(CernControl *self);

  void
  (*set_right_to_left)(CernControl *self, CernRightToLeft right_to_left);

  gboolean
  (*get_scale_children)(CernControl *self);

  gchar *
  (*get_text)(CernControl *self);

  void
  (*set_text)(CernControl *self, gchar *text);

  gboolean
  (*get_show_keyboard_cues)(CernControl *self);

  gboolean
  (*get_show_focus_cues)(CernControl *self);

  gint32
  (*get_show_params)(CernControl *self);

  gchar *
  (*get_window_text)(CernControl *self);

  void
  (*set_window_text)(CernControl *self, gchar *window_text);

  void
  (*assign_parent)(CernControl *self, CernControl *parent);

  gboolean
  (*can_process_mnemonic)(CernControl *self);

  gboolean
  (*can_select_core)(CernControl *self);

  void
  (*create_handle)(CernControl *self);

  void
  (*def_wnd_proc)(CernControl *self, CernMessage *message);

  void
  (*destroy_handle)(CernControl *self);

  gboolean
  (*focus_internal)(CernControl *self);

  CernRectangle
  (*apply_bounds_constraints)
  (CernControl *self, gint32 suggested_x, gint32 suggested_y, gint32 proposed_width, gint32 proposed_height);

  CernRectangle
  (*get_scaled_bounds)(CernControl *self, CernRectangle *bounds, CernSizeF *factor, CernBoundsSpecified specified);

  gboolean
  (*get_visible_core)(CernControl *self);

  CernControl *
  (*get_first_child_control_in_tab_order)(CernControl *self, gboolean forward);

  void
  (*init_layout)(CernControl *self);

  gpointer
  (*initialize_dc_for_wm_ctl_color)(CernControl *self, gpointer dc, guint32 msg);

  gboolean
  (*is_input_char)(CernControl *self, gchar *character);

  gboolean
  (*is_input_key)(CernControl *self, CernKeys key_data);

  void
  (*notify_invalidate)(CernControl *self, CernRectangle *rect);

  void
  (*notify_validating)(CernControl *self);

  void
  (*notify_validated)(CernControl *self);

  void
  (*on_background_color_changed)(CernControl *self, CernEventArgs *args);

  void
  (*on_background_image_changed)(CernControl *self, CernEventArgs *args);

  void
  (*on_background_image_layout_changed)(CernControl *self, CernEventArgs *args);

  void
  (*on_binding_context_changed)(CernControl *self, CernEventArgs *args);

  void
  (*on_causes_validation_changed)(CernControl *self, CernEventArgs *args);

  void
  (*on_child_layout_resuming)(CernControl *self, CernControl *child, gboolean perform_layout);
 
  void
  (*on_context_menu_strip_changed)(CernControl *self, CernEventArgs *args);

  void
  (*on_cursor_changed)(CernControl *self, CernEventArgs *args);

  void
  (*on_dock_changed)(CernControl *self, CernEventArgs *args);

  void
  (*on_enabled_changed)(CernControl *self, CernEventArgs *args);

  void
  (*on_font_changed)(CernControl *self, CernEventArgs *args);

  void
  (*on_fore_color_changed)(CernControl *self, CernEventArgs *args);

  void
  (*on_right_to_left_changed)(CernControl *self, CernEventArgs *args);

  void
  (*on_notify_message)(CernControl *self, CernMessage *message);

  void
  (*on_parent_back_color_changed)(CernControl *self, CernEventArgs *args);

  void
  (*on_parent_background_image_changed)(CernControl *self, CernEventArgs *args);

  void
  (*on_parent_binding_context_changed)(CernControl *self, CernEventArgs *args);

  void
  (*on_parent_cursor_changed)(CernControl *self, CernEventArgs *args);

  void
  (*on_parent_enabled_changed)(CernControl *self, CernEventArgs *args);

  void
  (*on_parent_font_changed)(CernControl *self, CernEventArgs *args);

  void
  (*on_parent_handle_recreated)(CernControl *self);

  void
  (*on_parent_handle_recreating)(CernControl *self);

  void
  (*on_parent_fore_color_changed)(CernControl *self, CernEventArgs *args);

  void
  (*on_parent_right_to_left_changed)(CernControl *self, CernEventArgs *args);

  void
  (*on_parent_visible_changed)(CernControl *self, CernEventArgs *args);

  void
  (*on_parent_became_invisible)(CernControl *self);

  void
  (*on_paint)(CernControl *self, CernPaintEventArgs *args);

  void
  (*on_tab_index_changed)(CernControl *self, CernEventArgs *args);

  void
  (*on_tab_stop_changed)(CernControl *self, CernEventArgs *args);

  void
  (*on_text_changed)(CernControl *self, CernEventArgs *args);

  void
  (*on_visible_changed)(CernControl *self, CernEventArgs *args);

  void
  (*on_parent_changed)(CernControl *self, CernEventArgs *args);

  void
  (*on_click)(CernControl *self, CernEventArgs *args);

  void
  (*on_client_size_changed)(CernControl *self, CernEventArgs *args);

  void
  (*on_control_added)(CernControl *self, CernEventArgs *args);

  void
  (*on_control_removed)(CernControl *self, CernEventArgs *args);

  void
  (*on_create_control)(CernControl *self);

  void
  (*on_handle_created)(CernControl *self, CernEventArgs *args);

  /* < signals > */
  void
  (*auto_size_changed)(CernControl *sender, CernEventArgs *args);

  void
  (*back_color_changed)(CernControl *sender, CernEventArgs *args);

  void
  (*background_image_changed)(CernControl *sender, CernEventArgs *args);

  void
  (*background_image_layout_changed)(CernControl *sender, CernEventArgs *args);

  void
  (*binding_context_changed)(CernControl *sender, CernEventArgs *args);

  void
  (*cause_validation_changed)(CernControl *sender, CernEventArgs *args);

  void
  (*client_size_changed)(CernControl *sender, CernEventArgs *args);

  void
  (*on_context_menu_changed)(CernControl *sender, CernEventArgs *args);

  void
  (*context_menu_strip_changed)(CernControl *sender, CernEventArgs *args);

  void
  (*validation_result)(CernControl *sender, CernCancelEventArgs *result);

  void
  (*cursor_changed)(CernControl *sender, CernEventArgs *args);

  void
  (*dock_changed)(CernControl *sender, CernEventArgs *args);

  void
  (*enabled_changed)(CernControl *sender, CernEventArgs *args);

  void
  (*font_changed)(CernControl *sender, CernEventArgs *args);

  void
  (*fore_color_changed)(CernControl *sender, CernEventArgs *args);

  void
  (*location_changed)(CernControl *sender, CernEventArgs *args);

  void
  (*margin_changed)(CernControl *sender, CernEventArgs *args);

  void
  (*region_changed)(CernControl *sender, CernEventArgs *args);

  void
  (*right_to_left_changed)(CernControl *sender, CernEventArgs *args);

  void
  (*size_changed)(CernControl *sender, CernEventArgs *args);

  void
  (*tab_index_changed)(CernControl *sender, CernEventArgs *args);

  void
  (*tab_stop_changed)(CernControl *sender, CernEventArgs *args);

  void
  (*text_changed)(CernControl *sender, CernEventArgs *args);

  void
  (*visible_changed)(CernControl *sender, CernEventArgs *args);

  void
  (*click)(CernControl *sender, CernEventArgs *args);

  void
  (*control_added)(CernControl *sender, CernControlEventArgs *args);

  void
  (*control_removed)(CernControl *sender, CernControlEventArgs *args);

  /* TODO: Need drag drop... */

  void
  (*handle_created)(CernControl *sender, CernEventArgs *args);

  void
  (*handle_destroyed)(CernControl *sender, CernEventArgs *args);

  void
  (*invalidated)(CernControl *sender, CernInvalidateEventArgs *args);

  void
  (*padding_changed)(CernControl *sender, CernEventArgs *args);

  void
  (*paint)(CernControl *sender, CernPaintEventArgs *args);

  void
  (*double_click)(CernControl *sender, CernEventArgs *args);

  void
  (*enter)(CernControl *sender, CernEventArgs *args);

  void
  (*got_focus)(CernControl *sender, CernEventArgs *args);

  void
  (*key_down)(CernControl *sender, CernKeyEventArgs *args);

  void
  (*key_press)(CernControl *sender, CernKeyPressEventArgs *args);

  void
  (*key_up)(CernControl *sender, CernKeyEventArgs *args);

  void
  (*layout)(CernControl *sender, CernLayoutEventArgs *args);

  void
  (*leave)(CernControl *sender, CernEventArgs *args);

  void
  (*lost_focus)(CernControl *sender, CernEventArgs *args);

  void
  (*mouse_click)(CernControl *sender, CernMouseEventArgs *args);

  void
  (*mouse_double_click)(CernControl *sender, CernMouseEventArgs *args);

  void
  (*mouse_capture_chanched)(CernControl *sender, CernEventArgs *args);

  void
  (*mouse_down)(CernControl *sender, CernMouseEventArgs *args);

  void
  (*mouse_enter)(CernControl *sender, CernMouseEventArgs *args);

  void
  (*mouse_leave)(CernControl *sender, CernMouseEventArgs *args);

  void
  (*dpi_changed_before_parent)(CernControl *sender, CernEventArgs *args);

  void
  (*dpi_changed_after_parent)(CernControl *sender, CernEventArgs *args);

  void
  (*mouse_hover)(CernControl *sender, CernEventArgs *args);

  void
  (*mouse_move)(CernControl *sender, CernMouseEventArgs *args);

  void
  (*mouse_up)(CernControl *sender, CernMouseEventArgs *args);

  void
  (*mouse_wheel)(CernControl *sender, CernMouseEventArgs *args);

  void
  (*move)(CernControl *sender, CernEventArgs *args);

  void
  (*preview_key_down)(CernControl *sender, CernPreviewKeyDownEventArgs *args);

  void
  (*resize)(CernControl *sender, CernEventArgs *args);

  void
  (*change_ui_cues)(CernControl *sender, CernUICuesEventArgs *args);

  void
  (*style_changed)(CernControl *sender, CernEventArgs *args);

  void
  (*system_color_changed)(CernControl *sender, CernEventArgs *args);

  void
  (*validating)(CernControl *sender, CernCancelEventArgs *args);

  void
  (*validated)(CernControl *sender, CernEventArgs *args);

  void
  (*parent_changed)(CernControl *sender, CernEventArgs *args);


  /* ContinueHere */
};

CernControl *
cern_control_new(void);

CernControl *
cern_control_new_2(gchar const *text);

CernControl *
cern_control_new_3(gchar const *text, gint32 left,
                   gint32 top, gint32 width, gint32 height);

CernControl *
cern_control_new_4(CernControl *parent, gchar const *text);

CernControl *
cern_control_new_5(CernControl *parent, gchar const *text,
                   gint32 left, gint32 top, gint32 width, gint32 height);

gboolean
cern_control_get_allow_drop(CernControl *self);

void
cern_control_set_allow_drop(CernControl *self, gboolean allow_drop);

CernAmbientProperties *
cern_control_get_ambient_properties_service(CernControl *self);

CernAnchorStyles
cern_control_get_anchor(CernControl *self);

void
cern_control_set_anchor(CernControl *self, CernAnchorStyles anchor);

gboolean
cern_control_get_auto_size(CernControl *self);

void
cern_control_set_auto_size(CernControl *self, gboolean auto_size);

CernPoint
cern_control_get_auto_scroll_offset(CernControl *self);

void
cern_control_set_auto_scroll_offset(CernControl *self, CernPoint *offset);

void
cern_control_set_auto_size_mode(CernControl *self, CernAutoSizeMode mode);

CernAutoSizeMode
cern_control_get_auto_size_mode(CernControl *self);

CernLayoutEngine *
cern_control_get_layout_engine(CernControl *self);

gpointer
cern_control_back_color_brush(CernControl *self);

CernColor
cern_control_get_back_color(CernControl *self);

void
cern_control_set_back_color(CernControl *self, CernColor *value);

CernImage *
cern_control_get_background_image(CernControl *self);

void
cern_control_set_background_image(CernControl *self, CernImage *value);

CernImageLayout
cern_control_get_background_image_layout(CernControl *self);

void
cern_control_set_background_image_layout(CernControl *self, CernImageLayout value);

gboolean
cern_control_get_becoming_active_control(CernControl *self);

void
cern_control_set_becoming_active_control(CernControl *self, gboolean value);

void
cern_control_reset_bindings(CernControl *self);

CernBindingContext *
cern_control_get_binding_context_internal(CernControl *self);

void
cern_control_set_binding_context_internal(CernControl *self, CernBindingContext *value);

CernBindingContext *
cern_control_get_binding_context(CernControl *self);

void
cern_control_set_binding_context(CernControl *self, CernBindingContext *value);

gint32
cern_control_get_bottom(CernControl *self);

CernRectangle
cern_control_get_bounds(CernControl *self);

void
cern_control_set_bounds(CernControl *self, CernRectangle *bounds);

gboolean
cern_control_get_can_access_properties(CernControl *self);

gboolean
cern_control_get_can_focus(CernControl *self);

gboolean
cern_control_get_can_raise_events(CernControl *self);

gboolean
cern_control_get_can_select(CernControl *self);

gboolean
cern_control_get_capture(CernControl *self);

void
cern_control_set_capture(CernControl *self, gboolean value);

gboolean
cern_control_get_capture_internal(CernControl *self);

void
cern_control_set_capture_internal(CernControl *self, gboolean value);

gboolean
cern_control_get_causes_validation(CernControl *self);

void
cern_control_set_causes_validation(CernControl *self, gboolean value);

gboolean
cern_control_get_cache_text_internal(CernControl *self);

void
cern_control_set_cache_text_internal(CernControl *self, gboolean value);

gboolean
cern_control_get_check_for_illegal_cross_thread_calls(void);

void
cern_control_set_check_for_illegal_cross_thread_calls(gboolean value);

CernRectangle
cern_control_get_client_rectangle(CernControl *self);

CernSize
cern_control_get_client_size(CernControl *self);

void
cern_control_set_client_size(CernControl *self, CernSize *value);

gchar *
cern_control_get_company_name(CernControl *self);

gboolean
cern_control_get_contains_focus(CernControl *self);

CernContextMenu *
cern_control_get_context_menu(CernControl *self);

void
cern_control_set_context_menu(CernControl *self, CernContextMenu *value);

CernContextMenuStrip *
cern_control_get_context_menu_strip(CernControl *self);

void
cern_control_set_context_menu_strip(CernControl *self, CernContextMenuStrip *value);

CernControlCollection *
cern_control_get_controls(CernControl *self);

gboolean
cern_control_get_created(CernControl *self);

CernCreateParams *
cern_control_get_create_params(CernControl *self);

gboolean
cern_control_validate_active_control(CernControl *self,
                                     gboolean *validated_control_allows_focus_change);

gboolean
cern_control_get_validation_cancelled(CernControl *self);

void
cern_control_set_validation_cancelled(CernControl *self, gboolean value);

gboolean
cern_control_get_is_top_mdi_window_closing(CernControl *self);

void
cern_control_set_is_top_mdi_window_closing(CernControl *self, gboolean value);

gboolean
cern_control_get_is_curently_being_scaled(CernControl *self);

void
cern_control_set_is_curently_being_scaled(CernControl *self, gboolean value);

gint32
cern_control_get_create_thread_id(CernControl *self);

CernCursor *
cern_control_get_cursor(CernControl *self);

void
cern_control_set_cursor(CernControl *self, CernCursor *value);

CernControlBindingsCollection *
cern_control_get_data_bindings(CernControl *self);

CernColor
cern_control_get_default_back_color(void);

CernCursor *
cern_control_get_default_cursor(CernControl *self);

CernFont *
cern_control_get_default_font(void);

CernColor
cern_control_get_default_fore_color(void);

CernPadding
cern_control_get_default_margin(CernControl *self);

CernSize
cern_control_get_default_maximum_size(CernControl *self);

CernSize
cern_control_get_default_minimum_size(CernControl *self);

CernPadding
cern_control_get_default_padding(CernControl *self);

CernRightToLeft
cern_control_get_default_right_to_left(void);

CernSize
cern_control_get_default_size(CernControl *self);

gint32
cern_control_get_device_dpi(CernControl *self);

gboolean
cern_control_get_double_buffered(CernControl *self);

void
cern_control_set_double_buffered(CernControl *self, gboolean value);

gboolean
cern_control_get_enabled(CernControl *self);

void
cern_control_set_enabled(CernControl *self, gboolean value);

gboolean
cern_control_get_focused(CernControl *self);

void
cern_control_scale_font(CernControl *self, gfloat factor);

gpointer
cern_control_get_font_handle(CernControl *self);

gint32
cern_control_get_font_height(CernControl *self);

void
cern_control_set_font_height(CernControl *self, gint32 value);

CernColor
cern_control_get_fore_color(CernControl *self);

void
cern_control_set_fore_color(CernControl *self, CernColor *value);

CernSize
cern_control_get_preferred_size_with_proposed(CernControl *self, CernSize *proposed_size);

CernSize
cern_control_get_preferred_size_core(CernControl *self, CernSize *proposed_size);

gpointer
cern_control_get_handle(CernControl *self);

gpointer
cern_control_get_handle_internal(CernControl *self);

gboolean
cern_control_get_has_children(CernControl *self);

gboolean
cern_control_get_has_menu(CernControl *self);

gint32
cern_control_get_height(CernControl *self);

void
cern_control_set_height(CernControl *self, gint32 value);

gboolean
cern_control_get_hosted_in_win32_dialog_manager(CernControl *self);

gboolean
cern_control_get_is_handle_created(CernControl *self);

gboolean
cern_control_get_is_layout_suspended(CernControl *self);

gboolean
cern_control_get_is_window_obscured(CernControl *self);

gpointer
cern_control_get_internal_handle(CernControl *self);

gboolean
cern_control_get_invoke_required(CernControl *self);

gboolean
cern_control_get_is_accessible(CernControl *self);

void
cern_control_set_is_accessible(CernControl *self, gboolean value);

gboolean
cern_control_get_is_container_control(CernControl *self);

gboolean
cern_control_get_is_mirrored(CernControl *self);

gint32
cern_control_get_left(CernControl *self);

void
cern_control_set_left(CernControl *self, gint32 value);

CernPoint
cern_control_get_location(CernControl *self);

void
cern_control_set_location(CernControl *self, CernPoint *value);

CernPadding
cern_control_get_margin(CernControl *self);

void
cern_control_set_margin(CernControl *self, CernPadding *value);

CernSize
cern_control_get_maximum_size(CernControl *self);

void
cern_control_set_maximum_size(CernControl *self, CernSize *value);

CernSize
cern_control_get_minimum_size(CernControl *self);

void
cern_control_set_minimum_size(CernControl *self, CernSize *value);

CernKeys
cern_control_get_modifier_keys(void);

CernMouseButtons
cern_control_get_mouse_buttons(void);

CernPoint
cern_control_get_mouse_position(void);

gchar *
cern_control_get_name(CernControl *self);

void
cern_control_set_name(CernControl *self, gchar *value);

CernControl *
cern_control_get_parent(CernControl *self);

void
cern_control_set_parent(CernControl *self, CernControl *parent);

CernControl *
cern_control_get_parent_internal(CernControl *self);

void
cern_control_set_parent_internal(CernControl *self, CernControl *parent);

gchar *
cern_control_get_product_name(CernControl *self);

gchar *
cern_control_get_product_version(CernControl *self);

CernPropertyStore *
cern_control_get_properties(CernControl *self);

CernColor
cern_control_get_raw_back_color(CernControl *self);

gboolean
cern_control_get_recreating_handle(CernControl *self);

CernRegion *
cern_control_get_region(CernControl *self);

void
cern_control_set_region(CernControl *self, CernRegion *value);

gboolean
cern_control_get_render_transparent(CernControl *self);

gboolean
cern_control_get_render_transparency_with_visual_styles(CernControl *self);

CernBoundsSpecified
cern_control_get_required_scaling(CernControl *self);

void
cern_control_set_required_scaling(CernControl *self, CernBoundsSpecified value);

gboolean
cern_control_get_required_scaling_enabled(CernControl *self);

void
cern_control_set_required_scaling_enabled(CernControl *self, gboolean value);

gboolean
cern_control_get_resize_redraw(CernControl *self);

void
cern_control_set_resize_redraw(CernControl *self, gboolean value);

gint32
cern_control_get_right(CernControl *self);

CernRightToLeft
cern_control_get_right_to_left(CernControl *self);

void
cern_control_set_right_to_left(CernControl *self, CernRightToLeft value);

gboolean
cern_control_get_scale_children(CernControl *self);

CernSize
cern_control_get_size(CernControl *self);

void
cern_control_set_size(CernControl *self, CernSize *value);

gint32
cern_control_get_tab_index(CernControl *self);

void
cern_control_set_tab_index(CernControl *self, gint32 insex);

gboolean
cern_control_get_tab_stop(CernControl *self);

void
cern_control_set_tab_stop(CernControl *self, gboolean value);

gboolean
cern_control_get_tab_stop_internal(CernControl *self);

void
cern_control_set_tab_stop_internal(CernControl *self, gboolean value);

gpointer
cern_control_get_tag(CernControl *self);

void
cern_control_set_tag(CernControl *self, gpointer value);

gchar *
cern_control_get_text(CernControl *self);

void
cern_control_set_text(CernControl *self, gchar *value);

gint32
cern_control_get_top(CernControl *self);

void
cern_control_set_top(CernControl *self, gint32 value);

CernControl *
cern_control_get_top_level_control(CernControl *self);

CernControl *
cern_control_get_top_level_control_internal(CernControl *self);

CernControl *
cern_control_get_top_most_parent(CernControl *self);

gboolean
cern_control_get_show_keyboard_cues(CernControl *self);

gboolean
cern_control_get_show_focus_cues(CernControl *self);

gint32
cern_control_get_show_params(CernControl *self);

gboolean
cern_control_get_use_wait_cursor(CernControl *self);

void
cern_control_set_use_wait_cursor(CernControl *self, gboolean value);

gboolean
cern_control_get_use_compatible_text_rendering_int(CernControl *self);

void
cern_control_set_use_compatible_text_rendering_int(CernControl *self, gboolean value);

gboolean
cern_control_get_supports_use_compatible_text_rendering(CernControl *self);

gboolean
cern_control_get_visible(CernControl *self);

void
cern_control_set_visible(CernControl *self, gboolean value);

gint32
cern_control_get_width(CernControl *self);

void
cern_control_set_width(CernControl *self, gint32 value);

gint32
cern_control_get_window_style(CernControl *self);

void
cern_control_set_window_style(CernControl *self, gint32 value);

gchar *
cern_control_get_window_text(CernControl *self);

void
cern_control_set_window_text(CernControl *self, gchar *value);

CernSize
cern_control_get_preferred_size(CernControl *self);

CernPadding
cern_control_get_padding(CernControl *self);

void
cern_control_set_padding(CernControl *self, CernPadding *value);

void
cern_control_assign_parent(CernControl *self, CernControl *parent);

void
cern_control_begin_update_internal(CernControl *self);

void
cern_control_bring_to_front(CernControl *self);

gboolean
cern_control_can_process_mnemonic(CernControl *self);

gboolean
cern_control_can_select_core(CernControl *self);

void
cern_control_check_parenting_cycle(CernControl *bottom, CernControl *to_find);

gboolean
cern_control_contains(CernControl *self, CernControl *control);

CernControlCollection *
cern_control_create_controls_instance(CernControl *self);

CernGraphics *
cern_control_create_graphics(CernControl *self);

CernGraphics *
cern_control_create_graphics_internal(CernControl *self);

void
cern_control_create_handle(CernControl *self);

void
cern_control_create_control(CernControl *self);

void
cern_control_create_control_internal(CernControl *self, gboolean ignore_visible);

void
cern_control_draw_to_bitmap(CernControl *self, CernBitmap *bitmap, CernRectangle *target_bound);

gboolean
cern_control_end_update_internal(CernControl *self);

gboolean
cern_control_end_update_internal_invalidate(CernControl *self, gboolean invalidate);

CernWindow *
cern_control_find_window(CernControl *self);

CernWindow *
cern_control_find_window_internal(CernControl *self);

gboolean
cern_control_get_top_level(CernControl *self);

void
cern_control_raise_create_handle_event(CernControl *self, CernEventArgs *args);

void
cern_control_raise_key_event(CernControl *self, CernKeyEventArgs *args);

void
cern_control_raise_mouse_event(CernControl *self, CernMouseEventArgs *args);

gboolean
cern_control_focus(CernControl *self);


gboolean
cern_control_focus_internal(CernControl *self);

CernControl *
cern_control_from_child_handle(gpointer handle);

CernControl *
cern_control_from_child_handle_internal(gpointer handle);

CernControl *
cern_control_from_handle(gpointer handle);

CernControl *
cern_control_from_handle_internal(gpointer handle);

CernSize
cern_control_apply_size_constraints(CernControl *self, gint32 width, gint32 height);

CernSize
cern_control_apply_size_constraints_size(CernControl *self, CernSize *size);

CernRectangle
cern_control_apply_bounds_constraints(CernControl *self,
                                      gint32 suggested_x, gint32 suggested_y,
                                      gint32 proposed_width, gint32 proposed_height);

CernControl *
cern_control_get_child_at_point_skip(CernControl *self, CernPoint *point,
                                     CernGetChildAtPointSkip skip);

CernControl *
cern_control_get_child_at_point(CernControl *self, CernPoint *point);

CernIContainerControl *
cern_control_get_container_control(CernControl *self);

gboolean
cern_control_is_updating_internal(CernControl *self);

CernIContainerControl *
cern_control_get_container_control_internal(CernControl *self);

gpointer
cern_control_get_h_rgn_internal(CernControl *self, CernRegion *region);

CernRectangle
cern_control_get_scaled_bounds(CernControl *self, CernRectangle *bounds,
                               CernSizeF *factor, CernBoundsSpecified specified);

gboolean
cern_control_get_visible_core_internal(CernControl *self);

GPtrArray *
cern_control_get_child_controls_in_tab_order_internal(CernControl *self, gboolean handle_crated_only);

CernControl *
cern_control_get_first_child_control_in_tab_order(CernControl *self, gboolean forward);

CernControl *
cern_control_get_next_control(CernControl *self, CernControl *ctl, gboolean forward);

gpointer
cern_control_get_safe_handle_internal(CernWin32Window *window);

gboolean
cern_control_get_state(CernControl *self, gint32 flag);

gboolean
cern_control_get_style(CernControl *self, CernControlStyles style);

void
cern_control_hide(CernControl *self);

void
cern_control_init_layout(CernControl *self);

gpointer
cern_control_initialize_dc_for_wm_ctl_color(CernControl *self, gpointer dc, guint32 msg);

void
cern_control_invalidate_region(CernControl *self, CernRectangle *region);

void
cern_control_invalidate_region_ex(CernControl *self, CernRectangle *region, gboolean invalidat_children);

void
cern_control_invalidate(CernControl *self);

void
cern_control_invalidate_ex(CernControl *self, gboolean invalidate_children);

void
cern_control_invalidate_rect(CernControl *self, CernRectangle *rect);

void
cern_control_invalidate_rect_ex(CernControl *self, CernRectangle *rect, gboolean invalidate_children);

void
cern_control_invoke_paint(CernControl *self, CernControl *ctl, CernPaintEventArgs *args);

void
cern_control_invoke_paint_background(CernControl *self, CernControl *ctl, CernPaintEventArgs *args);

gboolean
cern_control_is_font_set_internal(CernControl *self);

gboolean
cern_control_is_descendant_internal(CernControl *self, CernControl *control);

gboolean
cern_control_is_key_locked(CernKeys key_val);

gboolean
cern_control_is_input_char(CernControl *self, gchar ch);

gboolean
cern_control_is_input_key(CernControl *self, CernKeys key_data);

gboolean
cern_control_is_mnemonic(CernControl *self, gchar char_code, gchar const *text);

gint32
cern_control_logical_to_device_units(CernControl *self, gint32 logical_value);

CernSize
cern_control_logical_to_device_units_size(CernControl *self, CernSize *logical_size);

void
cern_control_scale_bitmap_logical_to_device(CernControl *self, CernBitmap *bitmap);

void
cern_control_adjust_window_rect_ex(CernRectangle *rect, gint32 style,
                                   gboolean menu, gint32 ex_style);

void
cern_control_notify_enter_internal(CernControl *self);

void
cern_control_notify_leave_internal(CernControl *self);

void
cern_control_notify_invalidate(CernControl *self, CernRectangle *rect);

void
cern_control_on_background_color_changed(CernControl *self, CernEventArgs *args);

void
cern_control_on_background_image_changed(CernControl *self, CernEventArgs *args);

void
cern_control_on_background_image_layout_changed(CernControl *self, CernEventArgs *args);

void
cern_control_on_binding_context_changed(CernControl *self, CernEventArgs *args);

void
cern_control_on_causes_validation_changed(CernControl *self, CernEventArgs *args);

void
cern_control_on_child_layout_resuming(CernControl *self, CernControl *child, gboolean perform_layout);

void
cern_control_on_context_menu_changed(CernControl *self, CernEventArgs *args);

void
cern_control_on_context_menu_strip_changed(CernControl *self, CernEventArgs *args);

void
cern_control_on_cursor_changed(CernControl *self, CernEventArgs *args);

void
cern_control_on_dock_changed(CernControl *self, CernEventArgs *args);

void
cern_control_on_enabled_changed(CernControl *self, CernEventArgs *args);

void
cern_control_on_frame_window_activate(CernControl *self, gboolean activate);

void
cern_control_on_font_changed(CernControl *self, CernEventArgs *args);

void
cern_control_on_fore_color_changed(CernControl *self, CernEventArgs *args);

void
cen_control_on_right_to_left_changed(CernControl *self, CernEventArgs *args);

void
cern_control_on_notify_message(CernControl *self, CernMessage *message);

void
cern_control_on_parent_back_color_changed(CernControl *self, CernEventArgs *args);

void
cern_control_on_parent_background_image_changed(CernControl *self, CernEventArgs *args);

void
cern_control_on_parent_binding_context_changed(CernControl *self, CernEventArgs *args);

void
cern_control_on_parent_cursor_changed(CernControl *self, CernEventArgs *args);

void
cern_control_on_parent_enabled_changed(CernControl *self, CernEventArgs *args);

void
cern_control_on_parent_font_changed(CernControl *self, CernEventArgs *args);

void
cern_control_on_parent_handle_recreated(CernControl *self);

void
cern_control_on_parent_handle_recreating(CernControl *self);

void
cern_control_on_parent_fore_color_changed(CernControl *self, CernEventArgs *args);

void
cern_control_on_parent_right_to_left_changed(CernControl *self, CernEventArgs *args);

void
cern_control_on_parent_visible_changed(CernControl *self, CernEventArgs *args);

void
cern_control_on_parent_became_invisible(CernControl *self);

void
cern_control_on_tab_index_changed(CernControl *self, CernEventArgs *args);

void
cern_control_on_tab_stop_changed(CernControl *self, CernEventArgs *args);

void
cern_control_on_text_changed(CernControl *self, CernEventArgs *args);

void
cern_control_on_visible_changed(CernControl *self, CernEventArgs *args);

void
cern_control_on_parent_changed(CernControl *self, CernEventArgs *args);

void
cern_control_on_click(CernControl *self, CernEventArgs *args);

void
cern_control_on_client_size_changed(CernControl *self, CernEventArgs *args);

void
cern_control_on_control_added(CernControl *self, CernEventArgs *args);

void
cern_control_on_control_removed(CernControl *self, CernEventArgs *args);

/* new */

void
cern_control_on_create_control(CernControl *self);

void
cern_control_on_handle_created(CernControl *self, CernEventArgs *args);

void
cern_control_on_invoked_set_scroll_position(CernControl *self, GObject sender, CernEventArgs *args);

void
cern_control_on_location_changed(CernControl *self, CernEventArgs *args);

void
cern_control_on_handle_destroyed(CernControl *self, CernEventArgs *args);

void
cern_control_on_double_click(CernControl *self, CernEventArgs *args);

void
cern_control_on_drag_enter(CernControl *self, CernDragEventArgs *args);

void
cern_control_on_drag_over(CernControl *self, CernDragEventArgs *args);

void
cern_control_on_drag_leave(CernControl *self, CernEventArgs *args);

void
cern_control_on_drag_drop(CernControl *self, CernDragEventArgs *args);

void
cern_control_on_give_feedback(CernControl *self, CernGiveFeedbackEventArgs *args);

void
cern_control_on_enter(CernControl *self, CernEventArgs *args);

void
cern_control_invoke_got_focus(CernControl *self, CernControl *to_invoke, CernEventArgs *args);

void
cern_control_on_got_focus(CernControl *self, CernEventArgs *args);

void
cern_control_on_help_requested(CernControl *self, CernHelpEventArgs *args);

void
cern_control_on_invalidated(CernControl *self, CernInvalidateEventArgs *args);

void
cern_control_on_key_down(CernControl *self, CernKeyEventArgs *args);

void
cern_control_on_key_press(CernControl *self, CernKeyEventArgs *args);

void
cern_control_on_key_up(CernControl *self, CernKeyEventArgs *args);

void
cern_control_on_layout(CernControl *self, CernLayoutEventArgs *args);

void
cern_control_on_layout_resuming(CernControl *self, gboolean perform_layout);

void
cern_control_on_layout_suspended(CernControl *self);

void
cern_control_on_leave(CernControl *self, CernEventArgs *args);

void
cern_control_invoke_lost_focus(CernControl *self, CernControl *to_invoke, CernEventArgs *args);

void
cern_control_on_lost_focus(CernControl *self, CernEventArgs *args);

void
cern_control_on_margin_changed(CernControl *self, CernEventArgs *args);

void
cern_control_on_mouse_double_click(CernControl *self, CernEventArgs *args);

void
cern_control_on_mouse_clock(CernControl *self, CernEventArgs *args);

void
cern_control_on_mouse_capture_changed(CernControl *self, CernEventArgs *args);

void
cern_control_on_mouse_down(CernControl *self, CernEventArgs *args);

void
cern_control_on_mouse_enter(CernControl *self, CernEventArgs *args);

void
cern_control_on_mouse_leave(CernControl *self, CernEventArgs *args);

void
cern_control_on_dpi_changed_before_parent(CernControl *self, CernEventArgs *args);

void
cern_control_on_dpi_changed_afrer_parent(CernControl *self, CernEventArgs *args);

void
cern_control_on_mouse_hover(CernControl *self, CernEventArgs *args);

void
cern_control_on_mouse_move(CernControl *self, CernMouseEventArgs *args);

void
cern_control_on_mouse_up(CernControl *self, CernMouseEventArgs *args);

void
cern_control_on_mouse_up(CernControl *self, CernMouseEventArgs *args);

void
cern_control_on_move(CernControl *self, CernEventArgs *args);

void
cern_control_on_paint(CernControl *self, CernPaintEventArgs *args);

void
cern_control_on_padding_changed(CernControl *self, CernEventArgs *args);

void
cern_control_on_paint_background(CernControl *self, CernPaintEventArgs *args);

void
cern_control_on_parent_invalidated(CernControl *self, CernInvalidateEventArgs *args);

void
cern_control_on_query_continue_drag(CernControl *self, CernQueryContinueDragEventArgs *args);

void
cern_control_on_region_changed(CernControl *self, CernEventArgs *args);

void
cern_control_on_resize(CernControl *self, CernEventArgs *args);

void
cern_control_on_preview_key_down(CernControl *self, CernPreviewKeyDownEventArgs *args);

void
cern_control_on_size_changed(CernControl *self, CernEventArgs *args);

void
cern_control_on_change_ui_cues(CernControl *self, CernUICuesEventArgs *args);

void
cern_control_on_style_changed(CernControl *self, CernEventArgs *args);

void
cern_control_on_system_colors_changed(CernControl *self, CernEventArgs *args);

void
cern_control_on_validating(CernControl *self, CernCancelEventArgs *args);

void
cern_control_on_validated(CernControl *self, CernEventArgs *args);

void
cern_control_rescale_constants_for_dpi(CernControl *self,
                                       gint32 old_device_dpi,
                                       gint32 new_device_dpi);

void
cern_control_paint_background(CernControl *self, CernPaintEventArgs *args, 
                              CernRectangle *rectangle, CernColor *back_color,
                              CernPoint *scroll_offset);

void
cern_control_paint_transparent_background(CernControl *self,
                                          CernPaintEventArgs *args,
                                          CernRectangle *rectangle,
                                          CernRegion *transparent_region);

CernContainerControl *
cern_control_get_parent_container_control(CernControl *self);

void
cern_control_perform_layout(CernControl *self);

void
cern_control_perform_layout_ex(CernControl *self,
                               CernControl *affected_control,
                               gchar const *affected_property);

void
cern_control_perform_layout_ea(CernControl *self,
                               CernLayoutEventArgs *args);

void
cern_control_perform_control_validation(CernControl *self,
                                        gboolean bulk_validation);

void
cern_control_perform_container_validation(CernControl *self,
                                          CernValidationConstraints validation_onstraints);

CernPoint
cern_control_point_to_client(CernControl *self, CernPoint *point);

CernPoint
cern_control_point_to_client_internal(CernControl *self, CernPoint *point);

CernPoint
cern_control_point_to_screen(CernControl *self, CernPoint *point);

gboolean
cern_control_pre_process_message(CernControl *self, CernMessage *message);

CernPreProcessControlState
cern_control_pre_process_control_message(CernControl *self, CernMessage *message);

CernPreProcessControlState
cern_control_pre_process_control_message_internal(CernControl *control, CernMessage *message);

gboolean
cern_control_process_cmd_key(CernControl *self, CernMessage *message, CernKeys key_data);

void
cern_control_print_to_metafile_recursive(CernControl *self,
                                         gpointer hdc,
                                         gpointer lparam,
                                         CernRectangle *bounds);

gboolean
cern_control_process_dialog_char(CernControl *self, gchar char_code);

gboolean
cern_control_process_dialog_key(CernControl *self, CernKeys key_data);

gboolean
cern_control_process_key_event_args(CernControl *self, CernMessage *message);

gboolean
cern_control_process_key_message(CernControl *self, CernMessage *message);

gboolean
cern_control_process_key_preview(CernControl *self, CernMessage *message);

gboolean
cern_control_process_mnemonic(CernControl *self, gchar key_code);

gboolean
cern_control_process_ui_cues(CernControl *self, CernMessage *message);

void
cern_control_raise_drag_event(CernControl *self, GObject *key, CernDragEventArgs *args);

void
cern_control_raise_paint_event(CernControl *self, GObject *key, CernPaintEventArgs *args);

void
cern_control_reset_back_color(CernControl *self);

void
cern_control_reset_cursor(CernControl *self);

void
cern_control_reset_font(CernControl *self);

void
cern_control_reset_fore_color(CernControl *self);

void
cern_control_reset_right_to_left(CernControl *self);

void
cern_control_recreate_handle(CernControl *self);

void
cern_control_recreat_handle_core(CernControl *self);

CernRectangle
cern_control_rectangle_to_client(CernControl *self, CernRectangle *rectangle);

CernRectangle
cern_control_rectangle_to_screen(CernControl *self, CernRectangle *rectangle);

gboolean
cern_control_reflect_message(gpointer hwnd, CernMessage *message);

gboolean
cern_control_reflect_message_internal(gpointer hwnd, CernMessage *message);

void
cern_control_refresh(CernControl *self);

void
cern_control_release_uia_provider(CernControl *self, gpointer handle);

void
cern_control_reset_mouse_event_args(CernControl *self);

void
cern_control_reset_text(CernControl *self);

void
cern_control_resume_layout(CernControl *self);

void
cern_control_resume_layout_ex(CernControl *self, gboolean perform_layout);

void
cern_control_set_accept_drops(CernControl *self, gboolean accept);

void
cern_control_scale(CernControl *self, CernSizeF *factor);

void
cern_control_scale_ex(CernControl *self,
                      CernSizeF *include_factor,
                      CernSizeF exclude_factor,
                      CernControl *requesting_control);

void
cern_control_scale_child_controls(CernControl *self,
                                  CernSizeF *inlcude_factor,
                                  CernSizeF *exclude_factor,
                                  CernControl *requesting_control,
                                  gboolean update_window_font_if_needed);

void
cern_control_update_window_font_if_needed(CernControl *self);

void
cern_control_scale_control(CernControl *self,
                           CernSizeF *include_factor,
                           CernSizeF *exclude_factor,
                           CernControl *requesting_control);

void
cern_control_scale_control_with_bounds(CernControl *self,
                                       CernSizeF *factor,
                                       CernBoundsSpecified specified);

void
cern_control_scale_core(CernControl *self, gfloat dx, gfloat dy);

CernSize
cern_control_scale_size(CernControl *self, CernSize *start_size, gfloat x, float y);

void
cern_control_select(CernControl *self);

void
cern_control_select_ex(CernControl *self, gboolean directed, gboolean forward);

gboolean
cern_control_select_next_control(CernControl *self,
                                 CernControl *ctl,
                                 gboolean forward,
                                 gboolean tab_stop_only,
                                 gboolean nested,
                                 gboolean wrap);

gboolean
cern_control_select_next_control_internal(CernControl *self,
                                          CernControl *ctl,
                                          gboolean forward,
                                          gboolean tab_stop_only,
                                          gboolean nested,
                                          gboolean wrap);

gpointer
cern_control_send_message(CernControl *self, guint32 msg, gpointer w, gpointer l);

void
cern_control_sent_back(CernControl *self);

void
cern_control_set_bounds_i(CernControl *self, 
                          gint32 x, gint32 y,
                          gint32 w, gint32 h);

void
cern_control_set_bounds_is(CernControl *self,
                           gint32 x, gint32 y,
                           gint32 w, gint32 h,
                           CernBoundsSpecified specified);

void
cern_control_set_bounds_core(CernControl *self,
                             gint32 x, gint32 y,
                             gint32 w, gint32 h,
                             CernBoundsSpecified specified);

void
cern_control_set_client_size_core(CernControl *self,
                                  gint32 width, gint32 height);

CernSize
cern_control_size_from_client_size(CernControl *self,
                                   CernSize *client_size);

CernSize
cern_control_size_from_client_size_i(CernControl *self,
                                     gint32 width, gint32 height);

void
cern_control_set_state(CernControl *self, gint32 flag, gboolean value);

void
cern_control_set_state2(CernControl *self, gint32 flag, gboolean value);

void
cern_control_set_style(CernControl *self, CernControlStyles flag, gboolean value);

gpointer
cern_control_set_up_palette(gpointer dc, gboolean force, gboolean release_palette);

void
cern_control_set_top_level(CernControl *self, gboolean value);

void
cern_control_set_top_level_internal(CernControl *self, gboolean value);

void
cern_control_set_visible_core(CernControl *self, gboolean value);

/*
 * FIXME: Realize this
internal static AutoValidate GetAutoValidateForControl(Control control) {
  ContainerControl parent = control.ParentContainerControl;
  return (parent != null) ? parent.AutoValidate : AutoValidate.EnablePreventFocusChange;
}
*/

gboolean
cern_control_get_should_auto_validate(CernControl *self);

gboolean
cern_control_get_should_perform_container_validation(CernControl *self);

gboolean
cern_control_get_should_serialize_back_color(CernControl *self);

gboolean
cern_control_get_should_serialize_cursor(CernControl *self);

gboolean
cern_control_get_should_serialize_enabled(CernControl *self);

gboolean
cern_control_get_should_serialize_fore_color(CernControl *self);

gboolean
cern_control_get_should_serialize_font(CernControl *self);

gboolean
cern_control_get_should_serialize_right_to_left(CernControl *self);

gboolean
cern_control_get_should_serialize_visible(CernControl *self);


CernHorizontalAlignment
cern_control_rtl_translate_alignment_h(CernControl *self, CernHorizontalAlignment align);

CernLeftRightAlignment
cern_control_rtl_translate_alignment_lr(CernControl *self, CernLeftRightAlignment align);

CernContentAlignment
cern_control_rtl_translate_alignment_c(CernControl *self, CernContentAlignment align);

CernHorizontalAlignment
cern_control_rtl_translate_horizontal(CernControl *self, CernHorizontalAlignment align);

CernLeftRightAlignment
cern_control_rtl_translate_left_right(CernControl *self, CernLeftRightAlignment align);

CernContentAlignment
cern_control_rtl_translate_content(CernControl *self, CernContentAlignment align);

void
cern_control_show(CernControl *self);

gboolean
cern_control_get_should_serialize_margin(CernControl *self);

gboolean
cern_control_get_should_serialize_maximum_size(CernControl *self);

gboolean
cern_control_get_should_serialize_minimum_size(CernControl *self);

gboolean
cern_control_get_should_serialize_padding(CernControl *self);

gboolean
cern_control_get_should_serialize_size(CernControl *self);

gboolean
cern_control_should_serialize_text(CernControl *self);

void
cern_control_suspend_layout(CernControl *self);

void
cern_control_update(CernControl *self);

void
cern_control_update_bounds(CernControl *self);

void
cern_control_update_bouds_i(CernControl *self, gint32 x, gint32 y,
                            gint32 width, gint32 height);

void
cern_control_update_bounds_i_ex(CernControl *self, gint32 x, gint32 y,
                                gint32 width, gint32 height,
                                gint32 client_width, gint32 client_height);

void
cern_control_update_z_order(CernControl *self);

void
cern_control_update_style(CernControl *self);

void
cern_control_update_style_core(CernControl *self);

void
cern_control_on_bounds_update(CernControl *self,
                              gint32 x, gint32 y,
                              gint32 width, gint32 height);

void
cern_control_window_assign_handle(CernControl *self, gpointer handle, gboolean value);

void
cern_control_window_release_handle(CernControl *self);

void
cern_control_wm_context_menu(CernControl *self, CernMessage *message);

void
cern_control_wm_context_menu_ex(CernControl *self,
                                CernMessage *messasge,
                                CernControl *source_control);

void
cern_control_wnd_proc(CernControl *self, CernMessage *message);

gboolean
cern_control_get_supports_uia_providers(CernControl *self);

gboolean
cern_control_get_shows_own_keyboard_tool_tip(CernControl *self);

void
cern_control_on_keyboard_tool_tip_hook(CernControl *self, CernToolTip *tool_tip);

void
cern_control_on_keyboard_tool_tip_unhook(CernControl *self, CernToolTip *tool_tip);

CernRectangle
cern_control_get_tool_native_screen_rectangle(CernControl *self);

gboolean
cern_control_get_allows_keyboard_tool_tip(CernControl *self);

gboolean
cern_control_get_are_common_navigation_keys_down(void);

CernToolStripControlHost *
cern_control_get_tool_strip_control_host(CernControl *self);

void
cern_control_set_tool_strip_control_host(CernControl *self, CernToolStripControlHost *value);

gboolean
cern_control_get_allows_childer_to_show_tool_tips(CernControl *self);

G_END_DECLS

#endif /* CERN_UI_CONTROL_H */

