#ifndef CERN_DRAWING_2D_GRAPHICS_CONTAINER_H
#define CERN_DRAWING_2D_GRAPHICS_CONTAINER_H 1

#include <glib-object.h>

G_BEGIN_DECLS

#define CERN_TYPE_GRAPHICS_CONTAINER (cern_graphics_container_get_type())

G_DECLARE_FINAL_TYPE(CernGraphicsContainer, cern_graphics_container, CERN, GRAPHICS_CONTAINER, GObject)

CernGraphicsContainer *
cern_graphics_container_new(guint32 container);

guint32
cern_graphics_container_get_value(CernGraphicsContainer *self);

G_END_DECLS

#endif /* CERN_DRAWING_2D_GRAPHICS_CONTAINER_H */
