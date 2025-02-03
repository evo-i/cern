#ifndef CERN_SIZE_F_H
#define CERN_SIZE_F_H 1

#include <glib-object.h>

G_BEGIN_DECLS

typedef struct _CernSizeF {
  gfloat width;
  gfloat height;
} CernSizeF;

#define CERN_TYPE_SIZE_F (cern_size_f_get_type())

GType
cern_size_f_get_type (void);

CernSizeF *
cern_size_f_copy (CernSizeF *self);

void
cern_size_f_free (CernSizeF *self);

CernSizeF
cern_size_f_create(gfloat width, gfloat height);

gfloat
cern_size_f_get_width(CernSizeF *self);

void
cern_size_f_set_width(CernSizeF *self, gfloat width);

gfloat
cern_size_f_get_height(CernSizeF *self);

void
cern_size_f_set_height(CernSizeF *self, gfloat height);

G_END_DECLS

#endif /* CERN_SIZE_F_H */
