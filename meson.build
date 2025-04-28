project('cern', 'c',
  version : '0.1',
  default_options : ['warning_level=3', 'c_std=c17']) 

if host_machine.system() == 'windows'
  add_project_arguments('-D_WIN32_WINNT=0x0601', '-DWINVER=0x0601', language: ['c', 'cpp'])
endif

cc = meson.get_compiler('c')

cc.check_header('windows.h', required: true)
cc.check_header('stdio.h', required: true)

gdiplus_dep = cc.find_library('gdiplus', required: true)
shlwapi_dep = cc.find_library('shlwapi', required: true)

glib_dep = dependency('glib-2.0')
gobj_dep = dependency('gobject-2.0')
gmodule_dep = dependency('gmodule-2.0')
gio_dep = dependency('gio-2.0')
gthread_dep = dependency('gthread-2.0')

cern_core_src = [
  'src/core/icloneable.c',
  'src/core/windows/handle.c'
]

cern_core_deps = [
  gobj_dep,
  gthread_dep,
  gio_dep,
  glib_dep,
  gmodule_dep
]

cern_core = shared_library('cern_core',
  cern_core_src,
  dependencies: cern_core_deps,
  include_directories: 'include/')

cern_drawing_src = [
  'src/drawing/gdi/color.c',
  'src/drawing/gdi/drawing.c',
  'src/drawing/gdi/region.c',
  'src/drawing/gdi/graphics.c',
  'src/drawing/gdi/image_format.c',
  'src/drawing/gdi/property_item.c',
  'src/drawing/gdi/color_palette.c',
  'src/drawing/gdi/frame_dimension.c',
  'src/drawing/gdi/graphics_context.c',
  'src/drawing/gdi/size.c',
  'src/drawing/gdi/point.c',
  'src/drawing/gdi/size_f.c',
  'src/drawing/gdi/point_f.c',
  'src/drawing/gdi/rectangle.c',
  'src/drawing/gdi/rectangle_f.c',
  'src/drawing/gdi/font_family.c',
  'src/drawing/gdi/font.c',
  'src/drawing/gdi/icon.c',
  'src/drawing/gdi/image.c',
  'src/drawing/gdi/bitmap.c',
  'src/drawing/gdi/text/font_collection.c',
  'src/drawing/gdi/text/private_font_collection.c',
  'src/drawing/gdi/text/installed_font_collection.c',
  'src/drawing/gdi/pen.c',
  'src/drawing/gdi/brush.c',
  'src/drawing/gdi/solid_brush.c',
  'src/drawing/gdi/2d/blend.c',
  'src/drawing/gdi/2d/matrix.c',
  'src/drawing/gdi/2d/color_blend.c',
  'src/drawing/gdi/2d/hatch_brush.c',
  'src/drawing/gdi/2d/region_data.c',
  'src/drawing/gdi/2d/graphics_path.c',
  'src/drawing/gdi/2d/graphics_state.c',
  'src/drawing/gdi/2d/graphics_container.c',
  'src/drawing/gdi/imaging/bitmap_data.c',
  'src/drawing/gdi/internal/device_context.c',
  'src/drawing/gdi/internal/color_translator.c',
  'src/drawing/gdi/internal/i_device_context.c',
  'src/drawing/gdi/internal/windows_graphics.c',
  'src/drawing/gdi/internal/windows_region.c',
  'src/drawing/gdi/internal/windows_font.c',
  'src/drawing/gdi/internal/font_handle_wrapper.c',
  'src/drawing/gdi/internal/native_gdi_object.c',
]

cern_drawing_deps = [
  gobj_dep,
  gmodule_dep,
  gdiplus_dep,
  gio_dep,
  shlwapi_dep
]

cern_drawing = shared_library('cern_drawing',
  cern_drawing_src,
  dependencies: cern_drawing_deps,
  include_directories: 'include/',
  link_with: [cern_core])

cern_ui_src = [
  'src/ui/win32/window.c',
  'src/ui/drop_target.c',
  'src/ui/layout/default_layout.c',
  'src/ui/layout/layout_transaction.c',
  'src/ui/control.c',
  'src/ui/component_model/isite.c',
  'src/ui/component_model/component.c',
  'src/ui/component_model/icomponent.c',
  'src/ui/component_model/icontainer.c',
  'src/ui/component_model/event_args.c',
  'src/ui/component_model/cancel_event_args.c',
  'src/ui/cursor.c',
  'src/ui/control_tab_order_holder.c',
  'src/ui/native_window.c',
  'src/ui/control_native_window.c',
  'src/ui/ambient_properties.c',
  'src/ui/common_properties.c',
  'src/ui/property_names.c',
  'src/ui/property_store.c',
  'src/ui/window_target.c',
  'src/ui/component_model/iservice_provider.c',
]

shared_library('cern_ui',
  cern_ui_src,
  link_with: [cern_core, cern_drawing],
  include_directories: 'include/')


executable('main', 'main.c')
