#include "cern/drawing/region.h"
#include "cern/drawing/graphics_context.h"
#include "cern/drawing/internal/native_gdi_object.h"

#include <windows.h>
#include <gdiplus.h>

typedef struct _CernGraphicsContext {
  GObject parent_instance;
  guint context_state;
  CernPointF transform_offset;
  CernRegion *clip_region;
  struct _CernGraphicsContext *next_context;
  struct _CernGraphicsContext *prev_context;
  gboolean is_cumulative;
} CernGraphicsContext;

struct _CernGraphicsContextClass {
  GObjectClass parent_class;
};

G_DEFINE_TYPE(CernGraphicsContext, cern_graphics_context, G_TYPE_OBJECT)

static void cern_graphics_context_dispose(GObject *object) {
  CernGraphicsContext *self = CERN_GRAPHICS_CONTEXT(object);

  if (self->next_context) {
    g_object_unref(self->next_context);
    self->next_context = NULL;
  }

  if (self->clip_region) {
    GdipDeleteRegion(self->clip_region);
    self->clip_region = NULL;
  }

  G_OBJECT_CLASS(cern_graphics_context_parent_class)->dispose(object);
}

static
void
cern_graphics_context_class_init(CernGraphicsContextClass *klass) {
  GObjectClass *object_class = G_OBJECT_CLASS(klass);
  object_class->dispose = cern_graphics_context_dispose;
}

static
void
cern_graphics_context_init(CernGraphicsContext *self) { }

CernGraphicsContext *
cern_graphics_context_new(CernGraphics *graphics) {
  CernGraphicsContext *self = g_object_new(CERN_TYPE_GRAPHICS_CONTEXT, NULL);
  CernNativeGdiObject *native = CERN_NATIVE_GDI_OBJECT(graphics);

  GpMatrix *transform;
  GdipCreateMatrix(&transform);
  GdipGetWorldTransform(cern_native_gdi_object_get_native_handle(native), transform);
  BOOL is_identity;
  GdipIsMatrixIdentity(transform, &is_identity);
  if (!is_identity) {
    REAL elements[6];
    GdipGetMatrixElements(transform, elements);
    cern_point_f_set_x(&self->transform_offset, elements[4]);
    cern_point_f_set_y(&self->transform_offset, elements[5]);
  }

  GdipDeleteMatrix(transform);

  GpRegion *clip;
  GdipCreateRegion(&clip);
  GdipGetClip(cern_native_gdi_object_get_native_handle(native), clip);
  BOOL is_infinite;
  GdipIsInfiniteRegion(clip, cern_native_gdi_object_get_native_handle(native), &is_infinite);
  if (is_infinite) {
    GdipDeleteRegion(clip);
  } else {
    self->clip_region = clip;
  }

  return self;
}

guint
cern_graphics_context_get_state(CernGraphicsContext *self) {
  return self->context_state;
}

void
cern_graphics_context_set_state(CernGraphicsContext *self, guint state) {
  self->context_state = state;
}

CernPointF
cern_graphics_context_get_transform_offset(CernGraphicsContext *self) {
  return self->transform_offset;
}

CernRegion *
cern_graphics_context_get_clip(CernGraphicsContext *self) {
  return self->clip_region;
}

CernGraphicsContext *
cern_graphics_context_get_next(CernGraphicsContext *self) {
  return self->next_context;
}

void
cern_graphics_context_set_next(CernGraphicsContext *self, CernGraphicsContext *next) {
  if (self->next_context) {
    g_object_unref(self->next_context);
  }
  self->next_context = next;
  if (next) {
    g_object_ref(next);
  }
}

CernGraphicsContext *
cern_graphics_context_get_previous(CernGraphicsContext *self) {
  return self->prev_context;
}

void
cern_graphics_context_set_previous(CernGraphicsContext *self, CernGraphicsContext *prev) {
  if (self->prev_context) {
    g_object_unref(self->prev_context);
  }
  self->prev_context = prev;
  if (prev) {
    g_object_ref(prev);
  }
}

gboolean
cern_graphics_context_get_is_cumulative(CernGraphicsContext *self) {
  return self->is_cumulative;
}

void cern_graphics_context_set_is_cumulative(CernGraphicsContext *self, gboolean is_cumulative) {
  self->is_cumulative = is_cumulative;
}
