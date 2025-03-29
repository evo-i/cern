#ifndef CERN_UI_CREATE_PARAMS_H
#define CERN_UI_CREATE_PARAMS_H 1

#include <glib-object.h>

G_BEGIN_DECLS

#define CERN_TYPE_CREATE_PARAMS cern_create_params_get_type ()

G_DECLARE_DERIVABLE_TYPE(CernCreateParams, cern_create_params, CERN, CREATE_PARAMS, GObject)

struct _CernCreateParamsClass {
  GObjectClass parent_class;

  void(*reserved[12])(void);
};

CernCreateParams *
cern_create_params_new_empty(void);

CernCreateParams *
cern_create_params_new(gchar const *class_name, gchar const *title,
                       gint32 style, gint32 ex_style, gint32 class_style,
                       gint32 x, gint32 y, gint32 width, gint32 height,
                       gpointer parent, gpointer param);


gchar const *
cern_create_params_get_class_name(CernCreateParams *self);

void
cern_create_params_set_class_name(CernCreateParams *self, gchar const *class_name);

gchar const *
cern_create_params_get_title(CernCreateParams *self);

void
cern_create_params_set_title(CernCreateParams *self, gchar const *title);

gint32
cern_create_params_get_style(CernCreateParams *self);

void
cern_create_params_set_style(CernCreateParams *self, gint32 style);

gint32
cern_create_params_get_ex_style(CernCreateParams *self);

void
cern_create_params_set_ex_style(CernCreateParams *self, gint32 ex_style);

gint32
cern_create_params_get_class_style(CernCreateParams *self);

void
cern_create_params_set_class_style(CernCreateParams *self, gint32 class_style);

gint32
cern_create_params_get_x(CernCreateParams *self);

void
cern_create_params_set_x(CernCreateParams *self, gint32 x);

gint32
cern_create_params_get_y(CernCreateParams *self);

void
cern_create_params_set_y(CernCreateParams *self, gint32 y);

gint32
cern_create_params_get_width(CernCreateParams *self);

void
cern_create_params_set_width(CernCreateParams *self, gint32 width);

gint32
cern_create_params_get_height(CernCreateParams *self);

void
cern_create_params_set_height(CernCreateParams *self, gint32 height);

gpointer
cern_create_params_get_parent(CernCreateParams *self);

void
cern_create_params_set_parent(CernCreateParams *self, gpointer parent);

gpointer
cern_create_params_get_param(CernCreateParams *self);

void
cern_create_params_set_param(CernCreateParams *self, gpointer param);


G_END_DECLS

#endif /* CERN_UI_CREATE_PARAMS_H */
