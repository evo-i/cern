#ifndef CERN_DRAWING_INTERNAL_WINDOWS_REGION_H
#define CERN_DRAWING_INTERNAL_WINDOWS_REGION_H 1

#include <glib-object.h>

#include "../rectangle.h"
#include "region_flags.h"
#include "region_combine_mode.h"

G_BEGIN_DECLS

typedef struct _CernRegion    CernRegion;
typedef struct _CernGraphics  CernGraphics;

#define CERN_TYPE_WINDOWS_REGION (cern_windows_region_get_type())

G_DECLARE_FINAL_TYPE(CernWindowsRegion, cern_windows_region, CERN, WINDOWS_REGION, GObject)

CernWindowsRegion *
cern_windows_region_new(CernRectangle *rectangle);

CernWindowsRegion *
cern_windows_region_new_with_rectangle(gint32 x, gint32 y,
                                       gint32 width, gint32 height);

CernWindowsRegion *
cern_windows_region_new_from_h_region(gpointer h_region,
                                      gboolean take_ownership);

CernWindowsRegion *
cern_windows_region_new_from_region(CernRegion *region, CernGraphics *graphics);

gboolean
cern_windows_region_is_infinite(CernWindowsRegion *self);

CernRectangle
cern_windows_region_to_rectangle(CernWindowsRegion *self);

CernRegionFlags
cern_windows_region_combine(CernWindowsRegion *self, CernWindowsRegion *region1,
                            CernWindowsRegion *region2, CernRegionCombineMode mode);

G_END_DECLS

#endif /* CERN_DRAWING_INTERNAL_WINDOWS_REGION_H */
