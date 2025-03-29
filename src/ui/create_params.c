#include "cern/ui/create_params.h"

typedef struct _CernCreateParamsPrivate {
  gchar *class_name;
  gchar *title;
  gint32 style;
  gint32 ex_style;
  gint32 class_style;
  gint32 x;
  gint32 y;
  gint32 width;
  gint32 height;
  gpointer parent;
  gpointer param;
} CernCreateParamsPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(CernCreateParams, cern_create_params, G_TYPE_OBJECT)

static
void
cern_create_params_finalize(GObject *object) {
  CernCreateParams *self;
  CernCreateParamsPrivate *priv;
  self = CERN_CREATE_PARAMS(object);
  priv = cern_create_params_get_instance_private(self);

  g_free(priv->class_name);
  g_free(priv->title);

  G_OBJECT_CLASS(cern_create_params_parent_class)->finalize(object);
}

static
void
cern_create_params_class_init(CernCreateParamsClass *klass) {
  GObjectClass *object_class;
  object_class = G_OBJECT_CLASS(klass);
  object_class->finalize = cern_create_params_finalize;
};

static
void
cern_create_params_init(CernCreateParams *self) {
  CernCreateParamsPrivate *priv;
  priv = cern_create_params_get_instance_private(self);

  priv->class_name = NULL;
  priv->title = NULL;
  priv->style = 0;
  priv->ex_style = 0;
  priv->class_style = 0;
  priv->x = 0;
  priv->y = 0;
  priv->width = 0;
  priv->height = 0;
  priv->parent = NULL;
  priv->param = NULL;
}

CernCreateParams *
cern_create_params_new_empty(void) {
  return g_object_new(CERN_TYPE_CREATE_PARAMS, NULL);
}

CernCreateParams *
cern_create_params_new(gchar const *class_name, gchar const *title,
                       gint32 style, gint32 ex_style, gint32 class_style,
                       gint32 x, gint32 y, gint32 width, gint32 height,
                       gpointer parent, gpointer param) {
  CernCreateParams *self;

  self = g_object_new(CERN_TYPE_CREATE_PARAMS, NULL);

  CernCreateParamsPrivate *priv;
  priv = cern_create_params_get_instance_private(self);

  priv->class_name = g_strdup(class_name);
  priv->title = g_strdup(title);
  priv->style = style;
  priv->ex_style = ex_style;
  priv->class_style = class_style;
  priv->x = x;
  priv->y = y;
  priv->width = width;
  priv->height = height;
  priv->parent = parent;
  priv->param = param;

  return self;
}

gchar const *
cern_create_params_get_class_name(CernCreateParams *self) {
  CernCreateParamsPrivate *priv;
  priv = cern_create_params_get_instance_private(self);
  return priv->class_name;
}

gchar const *
cern_create_params_get_title(CernCreateParams *self) {
  CernCreateParamsPrivate *priv;
  priv = cern_create_params_get_instance_private(self);
  return priv->title;
}

gint32
cern_create_params_get_style(CernCreateParams *self) {
  CernCreateParamsPrivate *priv;
  priv = cern_create_params_get_instance_private(self);
  return priv->style;
}

gint32
cern_create_params_get_ex_style(CernCreateParams *self) {
  CernCreateParamsPrivate *priv;
  priv = cern_create_params_get_instance_private(self);
  return priv->ex_style;
}

gint32
cern_create_params_get_class_style(CernCreateParams *self) {
  CernCreateParamsPrivate *priv;
  priv = cern_create_params_get_instance_private(self);
  return priv->class_style;
}

gint32
cern_create_params_get_x(CernCreateParams *self) {
  CernCreateParamsPrivate *priv;
  priv = cern_create_params_get_instance_private(self);
  return priv->x;
}

gint32
cern_create_params_get_y(CernCreateParams *self) {
  CernCreateParamsPrivate *priv;
  priv = cern_create_params_get_instance_private(self);
  return priv->y;
}

gint32
cern_create_params_get_width(CernCreateParams *self) {
  CernCreateParamsPrivate *priv;
  priv = cern_create_params_get_instance_private(self);
  return priv->width;
}

gint32
cern_create_params_get_height(CernCreateParams *self) {
  CernCreateParamsPrivate *priv;
  priv = cern_create_params_get_instance_private(self);
  return priv->height;
}

gpointer
cern_create_params_get_parent(CernCreateParams *self) {
  CernCreateParamsPrivate *priv;
  priv = cern_create_params_get_instance_private(self);
  return priv->parent;
}

gpointer
cern_create_params_get_param(CernCreateParams *self) {
  CernCreateParamsPrivate *priv;
  priv = cern_create_params_get_instance_private(self);
  return priv->param;
}

void
cern_create_params_set_class_style(CernCreateParams *self, gint32 class_style) {
  CernCreateParamsPrivate *priv;
  priv = cern_create_params_get_instance_private(self);
  priv->class_style = class_style;
}

void
cern_create_params_set_x(CernCreateParams *self, gint32 x) {
  CernCreateParamsPrivate *priv;
  priv = cern_create_params_get_instance_private(self);
  priv->x = x;
}

void
cern_create_params_set_y(CernCreateParams *self, gint32 y) {
  CernCreateParamsPrivate *priv;
  priv = cern_create_params_get_instance_private(self);
  priv->y = y;
}

void
cern_create_params_set_width(CernCreateParams *self, gint32 width) {
  CernCreateParamsPrivate *priv;
  priv = cern_create_params_get_instance_private(self);
  priv->width = width;
}

void
cern_create_params_set_height(CernCreateParams *self, gint32 height) {
  CernCreateParamsPrivate *priv;
  priv = cern_create_params_get_instance_private(self);
  priv->height = height;
}

void
cern_create_params_set_parent(CernCreateParams *self, gpointer parent) {
  CernCreateParamsPrivate *priv;
  priv = cern_create_params_get_instance_private(self);
  priv->parent = parent;
}

void
cern_create_params_set_param(CernCreateParams *self, gpointer param) {
  CernCreateParamsPrivate *priv;
  priv = cern_create_params_get_instance_private(self);
  priv->param = param;
}

void
cern_create_params_set_title(CernCreateParams *self, gchar const *title) {
  CernCreateParamsPrivate *priv;
  priv = cern_create_params_get_instance_private(self);

  if (priv->title != NULL) {
    g_free(priv->title);
  }

  priv->title = g_strdup(title);
}

void
cern_create_params_set_style(CernCreateParams *self, gint32 style) {
  CernCreateParamsPrivate *priv;
  priv = cern_create_params_get_instance_private(self);
  priv->style = style;
}

void
cern_create_params_set_ex_style(CernCreateParams *self, gint32 ex_style) {
  CernCreateParamsPrivate *priv;
  priv = cern_create_params_get_instance_private(self);
  priv->ex_style = ex_style;
}

void
cern_create_params_set_class_name(CernCreateParams *self, gchar const *class_name) {
  CernCreateParamsPrivate *priv;
  priv = cern_create_params_get_instance_private(self);

  if (priv->class_name != NULL) {
    g_free(priv->class_name);
  }

  priv->class_name = g_strdup(class_name);
};
