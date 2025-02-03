#ifndef CERN_FRAME_DIMENSION_H
#define CERN_FRAME_DIMENSION_H 1

#include <glib-object.h>
#include <guiddef.h>

G_BEGIN_DECLS

#define CERN_TYPE_FRAME_DIMENSION (cern_frame_dimension_get_type())

G_DECLARE_FINAL_TYPE(CernFrameDimension, cern_frame_dimension, CERN, FRAME_DIMENSION, GObject)

extern
GUID const cern_frame_dimension_time_guid;

extern
GUID const cern_frame_dimension_resolution_guid;

extern
GUID const cern_frame_dimension_page_guid;

CernFrameDimension *
cern_frame_dimension_new(GUID const *guid);

CernFrameDimension const *
cern_frame_dimension_time(void);

CernFrameDimension const *
cern_frame_dimension_resolution(void);

CernFrameDimension const *
cern_frame_dimension_page(void);

GUID const *
cern_frame_dimension_get_guid(CernFrameDimension *self);

gchar const *
cern_frame_dimension_get_name(CernFrameDimension *self);

gboolean
cern_frame_dimension_equals(CernFrameDimension *self, CernFrameDimension *other);

G_END_DECLS

#endif /* CERN_FRAME_DIMENSION_H */
