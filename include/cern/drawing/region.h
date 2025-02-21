#ifndef CERN_DRAWING_REGION_H
#define CERN_DRAWING_REGION_H 1

#include <glib-object.h>

G_BEGIN_DECLS

#include "rectangle_f.h"
#include "rectangle.h"
#include "2d/region_data.h"

typedef struct _CernGraphicsPath CernGraphicsPath;
typedef struct _CernGraphics CernGraphics;
typedef struct _CernMatrix CernMatrix;

#define CERN_TYPE_REGION (cern_region_get_type())

G_DECLARE_FINAL_TYPE(CernRegion, cern_region, CERN, REGION, GObject);

CernRegion *
cern_region_new(void);

CernRegion *
cern_region_new_rect_f(CernRectangleF *rect);

CernRegion *
cern_region_new_rect(CernRectangle *rect);

CernRegion *
cern_region_new_with_graphics_path(CernGraphicsPath *path);

void
cern_region_make_infinite(CernRegion *self);

void
cern_region_make_empty(CernRegion *self);

/* intersect */
void
cern_region_intersect(CernRegion *self, CernRegion *region);

void
cern_region_intersect_with_graphics_path(CernRegion *region, CernGraphicsPath *path);

void
cern_region_intersect_with_rectangle(CernRegion *region, CernRectangle *rect);

void
cern_region_intersect_with_rectangle_f(CernRegion *region, CernRectangleF *rect);

/* complement */
void
cern_region_complement(CernRegion *self, CernRegion *region);

void
cern_region_complement_with_graphics_path(CernRegion *region, CernGraphicsPath *path);

void
cern_region_complement_with_rectangle(CernRegion *region, CernRectangle *rect);

void
cern_region_complement_with_rectangle_f(CernRegion *region, CernRectangleF *rect);

void
cern_region_translate(CernRegion *self, gint32 dx, gint32 dy);

void
cern_region_translate_f(CernRegion *self, gfloat dx, gfloat dy);

void
cern_region_transform(CernRegion *self, CernMatrix *matrix);

/* exclude */
void
cern_region_exclude(CernRegion *self, CernRegion *region);

void
cern_region_exclude_with_graphics_path(CernRegion *region, CernGraphicsPath *path);

void
cern_region_exclude_with_rectangle(CernRegion *region, CernRectangle *rect);

void
cern_region_exclude_with_rectangle_f(CernRegion *region, CernRectangleF *rect);

/* bounds */
CernRectangleF
cern_region_get_bounds_f(CernRegion *self, CernGraphics *graphics);

gpointer
cern_region_get_h_region(CernRegion *self, CernGraphics *graphics);

void
cern_region_release_h_region(CernRegion *self, gpointer h_region);

CernRegionData *
cern_region_get_region_data(CernRegion *self);

GList *
cern_region_get_region_scans(CernRegion *self, CernMatrix *matrix);

gboolean
cern_region_is_empty(CernRegion *self, CernGraphics *graphics);

gboolean
cern_region_is_equals(CernRegion *self, CernRegion *region, CernGraphics *graphics);

gboolean
cern_region_is_infinite(CernRegion *self, CernGraphics *graphics);

gboolean
cern_region_is_visible_point_f(CernRegion *self, CernPointF *point);

gboolean
cern_region_is_visible_point(CernRegion *self, CernPoint *point);

gboolean
cern_region_is_visible_rect_f(CernRegion *self, CernRectangleF *rect);

gboolean
cern_region_is_visible_rect_f_graphics(CernRegion *self, CernRectangleF *rect,
                                       CernGraphics *graphics);

G_END_DECLS

#endif /* CERN_DRAWING_REGION_H */
