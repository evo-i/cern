cmake_minimum_required (VERSION 3.31)

project (Cern C)

find_package (Threads REQUIRED)
find_package (PkgConfig REQUIRED)

pkg_check_modules (GObject REQUIRED IMPORTED_TARGET "gobject-2.0")

add_library (cern_core
  SHARED
    src/core/icloneable.c)

target_include_directories (cern_core
  PUBLIC
    "include/")

target_link_libraries (cern_core
  PUBLIC
    Threads::Threads
    PkgConfig::GObject)

add_library (cern_drawing
  SHARED
    src/drawing/gdi/bitmap.c
    src/drawing/gdi/color_palette.c
    src/drawing/gdi/color.c
    src/drawing/gdi/drawing.c
    src/drawing/gdi/image.c
    src/drawing/gdi/region.c
    src/drawing/gdi/graphics.c
    src/drawing/gdi/size_f.c
    src/drawing/gdi/point_f.c
    src/drawing/gdi/rectangle_f.c
    src/drawing/gdi/size.c
    src/drawing/gdi/point.c
    src/drawing/gdi/rectangle.c
    src/drawing/gdi/property_item.c
    src/drawing/gdi/image_format.c
    src/drawing/gdi/frame_dimension.c
    src/drawing/gdi/graphics_context.c

    src/drawing/gdi/pen.c
    src/drawing/gdi/brush.c
    src/drawing/gdi/solid_brush.c

    src/drawing/gdi/2d/matrix.c
    src/drawing/gdi/2d/hatch_brush.c
    src/drawing/gdi/2d/region_data.c
    src/drawing/gdi/2d/graphics_path.c
    src/drawing/gdi/2d/graphics_state.c
    src/drawing/gdi/2d/graphics_container.c

    src/drawing/gdi/imaging/bitmap_data.c

    src/drawing/gdi/internal/device_context.c
    src/drawing/gdi/internal/color_translator.c
    src/drawing/gdi/internal/i_device_context.c
    src/drawing/gdi/internal/native_gdi_object.c)

target_include_directories (cern_drawing
  PRIVATE
    "include/")

target_link_libraries (cern_drawing
  PUBLIC
    cern_core
    Gdiplus
    Threads::Threads
    PkgConfig::GObject)

add_library (cern_ui
  SHARED
    src/ui/win32/window.c
    src/ui/drop_target.c

    src/ui/control.c
    src/ui/component_model/isite.c
    src/ui/component_model/component.c
    src/ui/component_model/icomponent.c
    src/ui/component_model/icontainer.c
    src/ui/component_model/iservice_provider.c)

target_include_directories (cern_ui
  PRIVATE
    "include/")

target_link_libraries (cern_ui
  PUBLIC
    cern_drawing
    Threads::Threads
    PkgConfig::GObject)

add_executable (main main.c)

target_link_libraries(main PRIVATE cern_ui cern_drawing)

target_include_directories (main PRIVATE "include/")
