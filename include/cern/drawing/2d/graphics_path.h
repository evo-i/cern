#ifndef CERN_DRAWING_2D_GRAPHICS_PATH_H
#define CERN_DRAWING_2D_GRAPHICS_PATH_H 1

#include <glib-object.h>

G_BEGIN_DECLS

#include "fill_mode.h"
#include "../point_f.h"
#include "../point.h"

#define CERN_TYPE_GRAPHICS_PATH (cern_graphics_path_get_type())

G_DECLARE_FINAL_TYPE(CernGraphicsPath, cern_graphics_path, CERN, GRAPHICS_PATH, GObject)

CernGraphicsPath *
cern_graphics_path_new(void);

CernGraphicsPath *
cern_graphics_path_new_fill_mode(CernFillMode fill_mode);

CernGraphicsPath *
cern_fraphics_path_new_points(CernPoint *points, gsize count, GByteArray *types);

CernGraphicsPath *
cern_fraphics_path_new_points_f(CernPointF *points, gsize count, GByteArray *types);

CernGraphicsPath *
cern_graphics_path_new_points_fill_mode_f(CernPointF *points,
                                          gsize points_count,
                                          GByteArray *types,
                                          CernFillMode fill_mode);

CernGraphicsPath *
cern_graphics_path_new_points_fill_mode(CernPoint *points,
                                        gsize points_count,
                                        GByteArray *types,
                                        CernFillMode fill_mode);

CernGraphicsPath *
cern_graphics_path_clone(CernGraphicsPath *self);

G_END_DECLS

#endif /* CERN_DRAWING_2D_GRAPHICS_PATH_H */
