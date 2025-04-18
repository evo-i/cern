cmake_minimum_required (VERSION 3.30)

project (Cern C)

find_package (Threads REQUIRED)
find_package (PkgConfig REQUIRED)

pkg_check_modules (GObject REQUIRED IMPORTED_TARGET "gobject-2.0;gio-2.0")

add_library (cern_core
  SHARED
    src/core/icloneable.c
    src/core/windows/handle.c)

target_include_directories (cern_core
  PUBLIC
    "include/")

target_link_libraries (cern_core
  PUBLIC
    Threads::Threads
    PkgConfig::GObject)

file(GLOB cern_drawing_headers "include/cern/drawing/*.h")

add_library (cern_drawing
  SHARED
    ${cern_drawing_headers}

    src/drawing/gdi/color.c
    src/drawing/gdi/drawing.c
    src/drawing/gdi/region.c
    src/drawing/gdi/graphics.c
    src/drawing/gdi/image_format.c
    src/drawing/gdi/property_item.c
    src/drawing/gdi/color_palette.c
    src/drawing/gdi/frame_dimension.c
    src/drawing/gdi/graphics_context.c

    src/drawing/gdi/size.c
    src/drawing/gdi/point.c
    src/drawing/gdi/size_f.c
    src/drawing/gdi/point_f.c
    src/drawing/gdi/rectangle.c
    src/drawing/gdi/rectangle_f.c

    # fonts
    src/drawing/gdi/font_family.c
    src/drawing/gdi/font.c

    # images
    src/drawing/gdi/icon.c
    src/drawing/gdi/image.c
    src/drawing/gdi/bitmap.c

    # text
    src/drawing/gdi/text/font_collection.c
    src/drawing/gdi/text/private_font_collection.c
    src/drawing/gdi/text/installed_font_collection.c

    # draw resources
    src/drawing/gdi/pen.c
    src/drawing/gdi/brush.c
    src/drawing/gdi/solid_brush.c

    # 2d
    src/drawing/gdi/2d/blend.c
    src/drawing/gdi/2d/matrix.c
    src/drawing/gdi/2d/color_blend.c
    src/drawing/gdi/2d/hatch_brush.c
    src/drawing/gdi/2d/region_data.c
    src/drawing/gdi/2d/graphics_path.c
    src/drawing/gdi/2d/graphics_state.c
    src/drawing/gdi/2d/graphics_container.c

    # imaging
    src/drawing/gdi/imaging/bitmap_data.c

    # internal
    src/drawing/gdi/internal/device_context.c
    src/drawing/gdi/internal/color_translator.c
    src/drawing/gdi/internal/i_device_context.c
    src/drawing/gdi/internal/windows_graphics.c
    src/drawing/gdi/internal/windows_region.c
    src/drawing/gdi/internal/font_handle_wrapper.c
    src/drawing/gdi/internal/native_gdi_object.c)

target_include_directories (cern_drawing
  PRIVATE
    "include/")

target_link_libraries (cern_drawing
  PUBLIC
    cern_core
    Shlwapi
    Gdiplus
    Threads::Threads
    PkgConfig::GObject)

set_target_properties (cern_drawing
  PROPERTIES
    C_STANDARD 17
    C_STANDARD_REQUIRED ON
    C_EXTENSIONS OFF)

add_library (cern_io
  SHARED
    src/io/stream.c)

target_link_libraries (cern_io
  PUBLIC
    ws2_32
    cern_core
    Threads::Threads
    PkgConfig::GObject)

set_target_properties (cern_io
  PROPERTIES
    C_STANDARD 17
    C_STANDARD_REQUIRED ON
    C_EXTENSIONS OFF)

target_include_directories (cern_io
  PRIVATE
    "include/")

file(GLOB cern_ui_headers "include/cern/ui/*.h")

add_library (cern_ui
  SHARED
    ${cern_ui_headers}
    src/ui/win32/window.c
    src/ui/drop_target.c

    src/ui/layout/default_layout.c
    src/ui/layout/layout_transaction.c

    src/ui/control.c
    src/ui/component_model/isite.c
    src/ui/component_model/component.c
    src/ui/component_model/icomponent.c
    src/ui/component_model/icontainer.c
    src/ui/component_model/event_args.c
    src/ui/component_model/cancel_event_args.c

    src/ui/cursor.c
    src/ui/control_tab_order_holder.c

    src/ui/native_window.c
    src/ui/control_native_window.c

    src/ui/ambient_properties.c
    src/ui/common_properties.c
    src/ui/property_names.c

    src/ui/property_store.c
    src/ui/window_target.c
    src/ui/component_model/iservice_provider.c)

target_include_directories (cern_ui
  PRIVATE
    "include/")

set_target_properties (cern_ui
  PROPERTIES
    C_STANDARD 17)

target_link_libraries (cern_ui
  PUBLIC
    cern_drawing
    Threads::Threads
    PkgConfig::GObject)

add_executable (main main.c)

target_link_libraries(main PRIVATE cern_drawing)

target_include_directories (main PRIVATE "include/")

