#ifndef CERN_DRAWING_2D_REGION_DATA_H
#define CERN_DRAWING_2D_REGION_DATA_H 1

#include <glib-object.h>

G_BEGIN_DECLS

#define CERN_TYPE_REGION_DATA (cern_region_data_get_type())

G_DECLARE_FINAL_TYPE(CernRegionData, cern_region_data, CERN, REGION_DATA, GObject)

CernRegionData *
cern_region_data_new(GByteArray *data);

GByteArray *
cern_region_data_get_data(CernRegionData *self);

void
cern_region_data_set_data(CernRegionData *self, GByteArray *data);

G_END_DECLS

#endif /* CERN_DRAWING_2D_REGION_DATA_H */
