#include "cern/ui/property_store.h"

static
gint32
property_store_current_key = 0;

#define COLOR_WRAPPER_TYPE (color_wrapper_get_type())

G_DECLARE_FINAL_TYPE(ColorWrapper, color_wrapper, COLOR, WRAPPER, GObject)

struct _ColorWrapper {
  GObject parent_instance;

  CernColor *color;
};

static
void
color_wrapper_class_init(ColorWrapperClass *klass) {}

static
void
color_wrapper_init(ColorWrapper *self) { }

G_DEFINE_FINAL_TYPE(ColorWrapper, color_wrapper, G_TYPE_OBJECT)

ColorWrapper *
color_wrapper_new(CernColor *color) {
  ColorWrapper *self = g_object_new(COLOR_WRAPPER_TYPE, NULL);
  self->color = color;
  return self;
}

#define RECTANGLE_WPARRER_TYPE (rectangle_wrapper_get_type())

G_DECLARE_FINAL_TYPE(RectangleWrapper, rectangle_wrapper, RECTANGLE, WRAPPER, GObject)

struct _RectangleWrapper {
  GObject parent_instance;

  CernRectangle rectangle;
};

static
void
rectangle_wrapper_init(RectangleWrapper *self) { }

static
void
rectangle_wrapper_class_init(RectangleWrapperClass *klass) { }

G_DEFINE_FINAL_TYPE(RectangleWrapper, rectangle_wrapper, G_TYPE_OBJECT)

RectangleWrapper *
rectangle_wrapper_new(CernRectangle *rectangle) {
  RectangleWrapper *self = g_object_new(RECTANGLE_WPARRER_TYPE, NULL);
  self->rectangle = *rectangle;
  return self;
}

#define PADDING_WRAPPER_TYPE (padding_wrapper_get_type())
G_DECLARE_FINAL_TYPE(PaddingWrapper, padding_wrapper, PADDING, WRAPPER, GObject)

struct _PaddingWrapper {
  GObject parent_instance;
  CernPadding padding;
};

static
void
padding_wrapper_init(PaddingWrapper *self) { }

static
void
padding_wrapper_class_init(PaddingWrapperClass *klass) { }

G_DEFINE_FINAL_TYPE(PaddingWrapper, padding_wrapper, G_TYPE_OBJECT)

PaddingWrapper *
padding_wrapper_new(CernPadding *padding) {
  PaddingWrapper *self = g_object_new(PADDING_WRAPPER_TYPE, NULL);
  self->padding = *padding;
  return self;
}

#define SIZE_WRAPPER_TYPE (size_wrapper_get_type())

G_DECLARE_FINAL_TYPE(SizeWrapper, size_wrapper, SIZE, WRAPPER, GObject)

struct _SizeWrapper {
  GObject parent_instance;

  CernSize size;
};

static
void
size_wrapper_init(SizeWrapper *self) { }

static
void
size_wrapper_class_init(SizeWrapperClass *klass) { }

G_DEFINE_FINAL_TYPE(SizeWrapper, size_wrapper, G_TYPE_OBJECT)

SizeWrapper *
size_wrapper_new(CernSize *size) {
  SizeWrapper *self = g_object_new(SIZE_WRAPPER_TYPE, NULL);
  self->size = *size;
  return self;
}

#define CERN_INTEGER_ENTRY_SIZE (sizeof(CernIntegerEntry))
#define CERN_INTEGER_ENTRIES_SIZE(length) (length * CERN_INTEGER_ENTRY_SIZE)

#define CERN_OBJECT_ENTRY_SIZE (sizeof(CernObjectEntry))
#define CERN_OBJECT_ENTRIES_SIZE(length) (length * CERN_OBJECT_ENTRY_SIZE)

typedef struct _CernIntegerEntry {
  gint16 key;
  gint16 mask;

  struct {
    gint32 one;
    gint32 two;
    gint32 three;
    gint32 four;
  } values;
} CernIntegerEntry;

typedef struct _CernObjectEntry {
  gint16 key;
  gint16 mask;

  struct {
    GObject *one;
    GObject *two;
    GObject *three;
    GObject *four;
  } values;
} CernObjectEntry;

typedef struct _CernPropertyStoreIntegersArray {
  CernIntegerEntry *data;
  gsize             length;
} CernPropertyStoreIntegersArray;

typedef struct _CernPropertyStoreObjectsArray {
  CernObjectEntry   *data;
  gsize             length;
} CernPropertyStoreObjectsArray;

typedef struct _CernPropertyStorePrivate {
  CernPropertyStoreIntegersArray integers;
  CernPropertyStoreObjectsArray  objects;
} CernPropertyStorePrivate;

static
void
cern_object_entry_free(CernObjectEntry *self) {
  if (self->values.one != NULL) {
    g_object_unref(self->values.one);
  }

  if (self->values.two != NULL) {
    g_object_unref(self->values.two);
  }

  if (self->values.three != NULL) {
    g_object_unref(self->values.three);
  }

  if (self->values.four != NULL) {
    g_object_unref(self->values.four);
  }
}

void
cern_property_store_array_objects_free(CernPropertyStoreObjectsArray *self) {
  gsize index;

  for (index = 0; index < self->length; index++) {
    cern_object_entry_free(self->data + index);
  }

  g_free(self->data);

  memset(self, 0, sizeof(CernPropertyStoreObjectsArray));
}

void
cern_property_store_array_integers_free(CernPropertyStoreIntegersArray *self) {
  g_free(self->data);
  memset(self, 0, sizeof(CernPropertyStoreObjectsArray));
}

G_DEFINE_TYPE_WITH_PRIVATE(CernPropertyStore, cern_property_store, G_TYPE_OBJECT)


static
void
cern_property_store_dispose(GObject *object) {
  CernPropertyStore *self;
  CernPropertyStorePrivate *priv;

  CernIntegerEntry *integer_entry;
  CernObjectEntry *object_entry;

  // g_array_unref(priv->integers);
  // g_array_unref(priv->objects);

  /* ??? what can i do here ??? */
  /* IDK because memory for arrays are freeded from finalize... */

  G_OBJECT_CLASS(cern_property_store_parent_class)->dispose(object);
}

static
void
cern_property_store_finalize(GObject *object) {
  CernPropertyStore *self;
  CernPropertyStorePrivate *priv;

  self = CERN_PROPERTY_STORE(object);
  priv = cern_property_store_get_instance_private(self);

  cern_property_store_array_integers_free(&priv->integers);
  cern_property_store_array_objects_free(&priv->objects);

  G_OBJECT_CLASS(cern_property_store_parent_class)->finalize(object);
}

gint16
static
cern_property_store_split_key(gint32 key, gint16 *element) {
  (*element) = (gint16) (key & 0x00000003);
  return (gint16) (key & 0xFFFFFFFC);
}

static
gboolean
cern_property_store_locate_integer_entry(CernPropertyStore *self,
                                         gint16 entry_key, gint32 *index) {
  CernPropertyStorePrivate *priv;
  CernIntegerEntry *entries;

  priv = cern_property_store_get_instance_private(self);
  entries = priv->integers.data;

  if (entries != NULL) {
    guint32 length = priv->integers.length;
    if (length <= 16) {
      (*index) = 0;

      gint32 mid_point = length / 2;

      if (entries[mid_point].key <= entry_key) {
        (*index) = mid_point;
      }

      if (entries[(*index)].key == entry_key) {
        return TRUE;
      }

      mid_point = (length + 1) / 4;

      if (entries[(*index) + mid_point].key <= entry_key) {
        (*index) += mid_point;
        if (entries[(*index)].key == entry_key) {
          return TRUE;
        }
      }

      mid_point = (length + 7) / 16;

      if (entries[(*index) + mid_point].key <= entry_key) {
        (*index) += mid_point;
        if (entries[(*index)].key == entry_key) {
          return TRUE;
        }
      }

      if (entry_key > entries[(*index)].key) {
        (index)++;
      }

      return FALSE;
    } else {
      gint32 max = length - 1;
      gint32 min = 0;
      gint32 idx = 0;

      do {
        idx = (max + min) / 2;
        gint16 current_key = entries[idx].key;

        if (current_key == entry_key) {
          (*index) = idx;
          return TRUE;
        }
        else if (entry_key < current_key) {
          max = idx - 1;
        }
        else {
          min = idx + 1;
        }
      } while (max >= min);

      (*index) = idx;

      if (entry_key > entries[(*index)].key) {
        (*index)++;
      }
      return FALSE;
    }
  } else {
    (*index) = 0;
    return FALSE;
  }
}

static
gboolean
cern_property_store_locate_object_entry(CernPropertyStore *self,
                                         gint16 entry_key, gint32 *index) {
  CernPropertyStorePrivate *priv;
  CernObjectEntry *entries;

  priv = cern_property_store_get_instance_private(self);
  entries = (CernObjectEntry *) priv->objects.data;

  if (entries != NULL) {
    guint32 length = priv->objects.length;
    if (length <= 16) {
      (*index) = 0;

      gint32 mid_point = length / 2;

      if (entries[mid_point].key <= entry_key) {
        (*index) = mid_point;
      }

      if (entries[(*index)].key == entry_key) {
        return TRUE;
      }

      mid_point = (length + 1) / 4;

      if (entries[(*index) + mid_point].key <= entry_key) {
        (*index) += mid_point;
        if (entries[(*index)].key == entry_key) {
          return TRUE;
        }
      }

      mid_point = (length + 7) / 16;

      if (entries[(*index) + mid_point].key <= entry_key) {
        (*index) += mid_point;
        if (entries[(*index)].key == entry_key) {
          return TRUE;
        }
      }

      if (entry_key > entries[(*index)].key) {
        (index)++;
      }

      return FALSE;
    } else {
      gint32 max = length - 1;
      gint32 min = 0;
      gint32 idx = 0;

      do {
        idx = (max + min) / 2;
        gint16 current_key = entries[idx].key;

        if (current_key == entry_key) {
          (*index) = idx;
          return TRUE;
        }
        else if (entry_key < current_key) {
          max = idx - 1;
        }
        else {
          min = idx + 1;
        }
      } while (max >= min);

      (*index) = idx;

      if (entry_key > entries[(*index)].key) {
        (*index)++;
      }
      return FALSE;
    }
  } else {
    (*index) = 0;
    return FALSE;
  }
}

static
gboolean
real_cern_property_store_contains_integer(CernPropertyStore *self, gint32 key) {
  gboolean result = FALSE;
  cern_property_store_try_get_integer(self, key, &result);
  return result;
}

static
gboolean
real_cern_property_store_contains_object(CernPropertyStore *self, gint32 key) {
  gboolean result = FALSE;
  cern_property_store_try_get_object(self, key, &result);
  return result;
}

static
CernPadding
real_cern_property_store_try_get_padding(CernPropertyStore *self, gint32 key, gboolean *found) {
  GObject *stored_object = NULL;
  stored_object = cern_property_store_try_get_object(self, key, found);
  if ((*found)) {
    if (PADDING_IS_WRAPPER(stored_object)) {
      return PADDING_WRAPPER(stored_object)->padding;
    } else {
      return (CernPadding) { 0 };
    }
  }

  return (CernPadding) { 0 };
}

static
CernSize
real_cern_property_store_get_size(CernPropertyStore *self, gint32 key) {
  GObject *stored_object = cern_property_store_get_object(self, key);
  SizeWrapper *size_wrapper = SIZE_WRAPPER(stored_object);
  if (stored_object != NULL) {
    return SIZE_WRAPPER(stored_object)->size;
  } else {
    return (CernSize) { 0 };
  }
  return (CernSize) { 0 };
}

static
CernSize
real_cern_property_store_try_get_size(CernPropertyStore *self, gint32 key, gboolean *found) {
  GObject *stored_object = cern_property_store_try_get_object(self, key, found);
  if ((*found)) {
    if (SIZE_IS_WRAPPER(stored_object)) {
      return SIZE_WRAPPER(stored_object)->size;
    } else {
      return (CernSize) { 0 };
    }
  }
  return (CernSize) { 0 };
}

static
CernRectangle
real_cern_property_store_get_rectangle(CernPropertyStore *self, gint32 key) {
  GObject *stored_object = cern_property_store_get_object(self, key);
  if (stored_object != NULL) {
    return RECTANGLE_WRAPPER(stored_object)->rectangle;
  } else {
    return (CernRectangle) { 0 };
  }
}

static
CernRectangle
real_cern_property_store_try_get_rectangle(CernPropertyStore *self, gint32 key, gboolean *found) {
  GObject *stored_object = cern_property_store_try_get_object(self, key, found);
  if ((*found)) {
    if (RECTANGLE_IS_WRAPPER(stored_object)) {
      return RECTANGLE_WRAPPER(stored_object)->rectangle;
    } else {
      return (CernRectangle) { 0 };
    }
  }
  return (CernRectangle) { 0 };
}

static
gint32
real_cern_property_store_get_integer(CernPropertyStore *self, gint32 key) {
  gboolean found = FALSE;
  return cern_property_store_try_get_integer(self, key, &found);
}

static
gint32
real_cern_property_store_try_get_integer(CernPropertyStore *self, gint32 key, gboolean *found) {
  CernPropertyStorePrivate *priv;

  gint32 value = 0;
  gint32 index = 0;
  gint16 element = 0;
  gint16 key_index = cern_property_store_split_key(key, &element);

  priv = cern_property_store_get_instance_private(self);
  if (cern_property_store_locate_integer_entry(self, key_index, &index)) {
    if (((1 << element) & priv->integers.data[index].mask)) {
      (*found) = TRUE;

      switch (element) {
        case 0: {
          value = priv->integers.data[index].values.one;
        } break;
        case 1: {
          value = priv->integers.data[index].values.two;
        } break;
        case 2: {
          value = priv->integers.data[index].values.three;
        } break;
        case 3: {
          value = priv->integers.data[index].values.four;
        } break;
        default: {
          g_assert_not_reached();
          value = 0;
        }
      }
    }
  }

  return value;
}

static
gpointer
real_cern_property_store_get_object(CernPropertyStore *self, gint32 key) {
  gboolean found = FALSE;
  return cern_property_store_try_get_object(self, key, &found);
}

static
gpointer
real_cern_property_store_try_get_object(CernPropertyStore *self, gint32 key, gboolean *found) {
  CernPropertyStorePrivate *priv;
  GObject *value = NULL;
  gint32 index;
  gint16 element;
  gint16 key_index = cern_property_store_split_key(key, &element);
  priv = cern_property_store_get_instance_private(self);

  (*found) = FALSE;

  if (cern_property_store_locate_object_entry(self, key_index, &index)) {
    if (((1 << element) & priv->objects.data[index].mask)) {
      (*found) = TRUE;
      switch(element) {
        case 0: {
          value = priv->objects.data[index].values.one;
        } break;
        case 1: {
          value = priv->objects.data[index].values.two;
        } break;
        case 2: {
          value = priv->objects.data[index].values.three;
        } break;
        case 3: {
          value = priv->objects.data[index].values.four;
        } break;
        default: {
          g_assert_not_reached();
          value = NULL;
        } break;
      }
    }
  }

  return value;
}

static
void
real_cern_property_store_set_color(CernPropertyStore *self, gint32 key, CernColor *value) {
  gboolean found = FALSE;
  GObject *stored_object
    = cern_property_store_try_get_object(self, key, &found);

  if (!found) {
    cern_property_store_set_object(self, key, color_wrapper_new(value));
  } else {
    if (COLOR_IS_WRAPPER(stored_object)) {
      COLOR_WRAPPER(stored_object)->color = value;
    } else {
      g_assert(value != NULL && "value should either be NULL or ColorWrapper");
      cern_property_store_set_object(self, key, color_wrapper_new(value));
    }
  }
}

static
void
real_cern_property_store_set_padding(CernPropertyStore *self, gint32 key, CernPadding value) {
  gboolean found = FALSE;
  GObject *stored_object
    = cern_property_store_try_get_object(self, key, &found);

  if (!found) {
    cern_property_store_set_object(self, key, padding_wrapper_new(&value));
  } else {
    if (PADDING_IS_WRAPPER(stored_object)) {
      PADDING_WRAPPER(stored_object)->padding = value;
    } else {
      cern_property_store_set_object(self, key, padding_wrapper_new(&value));
    }
  }
}

static
void
real_cern_property_store_set_size(CernPropertyStore *self, gint32 key, CernSize value) {
  gboolean found = FALSE;
  GObject *stored_object
    = cern_property_store_try_get_object(self, key, &found);

  if (!found) {
    cern_property_store_set_object(self, key, size_wrapper_new(&value));
  } else {
    if (SIZE_IS_WRAPPER(stored_object)) {
      SIZE_WRAPPER(stored_object)->size = value;
    } else {
      cern_property_store_set_object(self, key, size_wrapper_new(&value));
    }
  }
}

static
void
real_cern_property_store_set_rectangle(CernPropertyStore *self, gint32 key, CernRectangle value) {
  gboolean found = FALSE;
  GObject *stored_object
    = cern_property_store_try_get_object(self, key, &found);

  if (!found) {
    cern_property_store_set_object(self, key, rectangle_wrapper_new(&value));
  } else {
    if (RECTANGLE_IS_WRAPPER(stored_object)) {
      RECTANGLE_WRAPPER(stored_object)->rectangle = value;
    } else {
      cern_property_store_set_object(self, key, rectangle_wrapper_new(&value));
    }
  }
}

static
void
real_cern_property_store_set_integer(CernPropertyStore *self, gint32 key, gint32 value) {
  CernPropertyStorePrivate *priv;

  gint32 index = 0;
  gint16 element = 0;
  gint16 entry_key = cern_property_store_split_key(key, &element);

  priv = cern_property_store_get_instance_private(self);

  if (!cern_property_store_locate_object_entry(self, entry_key, &index)) {
    gsize new_length = priv->integers.length + 1;
    CernIntegerEntry *new_integers;

    new_length = priv->integers.length + 1;
    new_integers
      = g_realloc(priv->integers.data, CERN_INTEGER_ENTRIES_SIZE(new_length));
    if (new_integers == NULL) {
      g_error("Out of memory") ;
    }

    memset(new_integers + priv->integers.length, 0, CERN_INTEGER_ENTRY_SIZE);

    priv->integers.data = new_integers;
    priv->integers.length = new_length;
    priv->integers.data[index].key = entry_key;
  }

  switch(element) {
    case 0: {
      priv->integers.data[index].values.one = value;
    }break;
    case 1: {
      priv->integers.data[index].values.two = value;
    } break;
    case 2: {
      priv->integers.data[index].values.three = value;
    } break;
    case 3: {
      priv->integers.data[index].values.four = value;
    } break;
    default:
      g_assert_not_reached();
      break;
  }

  priv->integers.data[index].mask |= (1 << element);
}

static
void
real_cern_property_store_set_object(CernPropertyStore *self, gint32 key,
                                    gpointer value) {
  g_assert(G_IS_OBJECT(value));

  CernPropertyStorePrivate *priv;

  gint32 index = 0;
  gint16 element = 0;
  gint16 entry_key = cern_property_store_split_key(key, &element);
  priv = cern_property_store_get_instance_private(self);

  if (!cern_property_store_locate_object_entry(self, entry_key, &index)) {
    gsize new_length = priv->objects.length + 1;
    CernObjectEntry *new_objects;

    new_length = priv->objects.length + 1;
    new_objects
      = g_realloc(priv->objects.data, CERN_OBJECT_ENTRIES_SIZE(new_length));
    if (new_objects == NULL) {
      g_error("Out of memory") ;
    }

    memset(new_objects + priv->objects.length, 0, CERN_OBJECT_ENTRY_SIZE);

    priv->objects.data = new_objects;
    priv->objects.length = new_length;
    priv->objects.data[index].key = entry_key;
  }

  switch(element) {
    case 0: {
      priv->objects.data[index].values.one = value;
    } break;
    case 1: {
      priv->objects.data[index].values.two = value;
    } break;
    case 2: {
      priv->objects.data[index].values.three = value;
    } break;
    case 3: {
      priv->objects.data[index].values.four = value;
    } break;
    default: {
     g_assert_not_reached();
    } break;
  }

  priv->objects.data[index].mask |= (1 << element);
}
static
CernPadding
real_cern_property_store_get_padding(CernPropertyStore *self, gint32 key) {
  gboolean found = FALSE;
  return cern_property_store_try_get_padding(self, key, &found);
}

static
CernColor *
real_cern_property_store_get_color(CernPropertyStore *self, gint32 key) {
  gboolean found = FALSE;
  return cern_property_store_try_get_color(self, key, &found);
}

static
CernColor *
real_cern_property_store_try_get_color(CernPropertyStore *self, gint32 key, gboolean *found) {
  GObject *stored_object = cern_property_store_try_get_object(self, key, found);

  if ((*found)) {
    if(COLOR_IS_WRAPPER(stored_object)) {
      return COLOR_WRAPPER(stored_object)->color;
    }
  } else {
    return NULL;
  }

  return NULL;
}

static
void
cern_property_store_class_init(CernPropertyStoreClass *klass) {
  GObjectClass *object_class = G_OBJECT_CLASS(klass);

  object_class->finalize    = cern_property_store_finalize;
  object_class->dispose     = cern_property_store_dispose;

  klass->contains_integer   = real_cern_property_store_contains_integer;
  klass->contains_object    = real_cern_property_store_contains_object;

  klass->try_get_padding    = real_cern_property_store_try_get_padding;
  klass->set_padding        = real_cern_property_store_set_padding;
  klass->get_padding        = real_cern_property_store_get_padding;

  klass->get_size           = real_cern_property_store_get_size;
  klass->set_size           = real_cern_property_store_set_size;
  klass->try_get_size       = real_cern_property_store_try_get_size;

  klass->get_rectangle      = real_cern_property_store_get_rectangle;
  klass->set_rectangle      = real_cern_property_store_set_rectangle;
  klass->try_get_rectangle  = real_cern_property_store_try_get_rectangle;

  klass->get_integer        = real_cern_property_store_get_integer;
  klass->set_integer        = real_cern_property_store_set_integer;
  klass->try_get_integer    = real_cern_property_store_try_get_integer;

  klass->get_object         = real_cern_property_store_get_object;
  klass->set_object         = real_cern_property_store_set_object;
  klass->try_get_object     = real_cern_property_store_try_get_object;

  klass->get_color          = real_cern_property_store_get_color;
  klass->set_color          = real_cern_property_store_set_color;
  klass->try_get_color      = real_cern_property_store_try_get_color;
}

static
void
cern_property_store_init(CernPropertyStore *self) { }

gint32
cern_property_store_create_key(void) {
  return property_store_current_key++;
}

CernPropertyStore *
cern_property_store_new(void) {
  CernPropertyStore *self;
  self = g_object_new(CERN_TYPE_PROPERTY_STORE, NULL);
  return self;
}

CernPadding
cern_property_store_try_get_padding(CernPropertyStore *self, gint32 key, gboolean *found) {
  g_return_val_if_fail(CERN_IS_PROPERTY_STORE(self), (CernPadding) { 0 });
  CernPropertyStoreClass *klass = CERN_PROPERTY_STORE_GET_CLASS(self);
  g_return_val_if_fail(klass->try_get_padding, (CernPadding) { 0 });
  return klass->try_get_padding(self, key, found);
}

CernSize
cern_property_store_get_size(CernPropertyStore *self, gint32 key) {
  g_return_val_if_fail(CERN_IS_PROPERTY_STORE(self), (CernSize){0});
  CernPropertyStoreClass *klass = CERN_PROPERTY_STORE_GET_CLASS(self);
  g_return_val_if_fail(klass->get_size, (CernSize) { 0 });
  return klass->get_size(self, key);
}

CernSize
cern_property_store_try_get_size(CernPropertyStore *self, gint32 key, gboolean *found) {
  g_return_val_if_fail(CERN_IS_PROPERTY_STORE(self), (CernSize) { 0 });
  CernPropertyStoreClass *klass = CERN_PROPERTY_STORE_GET_CLASS(self);
  g_return_val_if_fail(klass->try_get_size, (CernSize) { 0 });
  return klass->try_get_size(self, key, found);
}

CernRectangle
cern_property_store_get_rectangle(CernPropertyStore *self, gint32 key) {
  g_return_val_if_fail(CERN_IS_PROPERTY_STORE(self), (CernRectangle){0});
  CernPropertyStoreClass *klass = CERN_PROPERTY_STORE_GET_CLASS(self);
  g_return_val_if_fail(klass->get_rectangle, (CernRectangle) { 0 });
  return klass->get_rectangle(self, key);
}

CernRectangle
cern_property_store_try_get_rectangle(CernPropertyStore *self, gint32 key, gboolean *found) {
  g_return_val_if_fail(CERN_IS_PROPERTY_STORE(self), (CernRectangle){0});
  CernPropertyStoreClass *klass = CERN_PROPERTY_STORE_GET_CLASS(self);
  g_return_val_if_fail(klass->try_get_rectangle, (CernRectangle) { 0 });
  return klass->try_get_rectangle(self, key, found);
}

gint32
cern_property_store_get_integer(CernPropertyStore *self, gint32 key) {
  g_return_val_if_fail(CERN_IS_PROPERTY_STORE(self), 0);
  CernPropertyStoreClass *klass = CERN_PROPERTY_STORE_GET_CLASS(self);
  g_return_val_if_fail(klass->get_integer, -1);
  return klass->get_integer(self, key);
}

gint32
cern_property_store_try_get_integer(CernPropertyStore *self, gint32 key, gboolean *found) {
  g_return_val_if_fail(CERN_IS_PROPERTY_STORE(self), 0);
  CernPropertyStoreClass *klass = CERN_PROPERTY_STORE_GET_CLASS(self);
  g_return_val_if_fail(klass->try_get_integer, -1);
  return klass->try_get_integer(self, key, found);
}

gpointer
cern_property_store_get_object(CernPropertyStore *self, gint32 key) {
  g_return_val_if_fail(CERN_IS_PROPERTY_STORE(self), NULL);
  CernPropertyStoreClass *klass = CERN_PROPERTY_STORE_GET_CLASS(self);
  g_return_val_if_fail(klass->get_object, NULL);
  return klass->get_object(self, key);
}

gpointer
cern_property_store_try_get_object(CernPropertyStore *self, gint32 key, gboolean *found) {
  g_return_val_if_fail(CERN_IS_PROPERTY_STORE(self), NULL);
  CernPropertyStoreClass *klass = CERN_PROPERTY_STORE_GET_CLASS(self);
  g_return_val_if_fail(klass->try_get_object, NULL);
  return klass->try_get_object(self, key, found);
}

void
cern_property_store_set_color(CernPropertyStore *self, gint32 key, CernColor *value) {
  g_return_if_fail(CERN_IS_PROPERTY_STORE(self));
  CernPropertyStoreClass *klass = CERN_PROPERTY_STORE_GET_CLASS(self);
  g_return_if_fail(klass->set_color);
  klass->set_color(self, key, value);
}

void
cern_property_store_set_padding(CernPropertyStore *self, gint32 key, CernPadding value) {
  g_return_if_fail(CERN_IS_PROPERTY_STORE(self));
  CernPropertyStoreClass *klass = CERN_PROPERTY_STORE_GET_CLASS(self);
  g_return_if_fail(klass->set_padding);
  klass->set_padding(self, key, value);
}

void
cern_property_store_set_size(CernPropertyStore *self, gint32 key, CernSize value) {
  g_return_if_fail(CERN_IS_PROPERTY_STORE(self));
  CernPropertyStoreClass *klass = CERN_PROPERTY_STORE_GET_CLASS(self);
  klass->set_size(self, key, value);
}

void
cern_property_store_set_rectangle(CernPropertyStore *self, gint32 key, CernRectangle value) {
  g_return_if_fail(CERN_IS_PROPERTY_STORE(self));
  CernPropertyStoreClass *klass = CERN_PROPERTY_STORE_GET_CLASS(self);
  g_return_if_fail(klass->set_rectangle);
  klass->set_rectangle(self, key, value);
}

void
cern_property_store_set_integer(CernPropertyStore *self, gint32 key, gint32 value) {
  g_return_if_fail(CERN_IS_PROPERTY_STORE(self));
  CernPropertyStoreClass *klass = CERN_PROPERTY_STORE_GET_CLASS(self);
  g_return_if_fail(klass->set_integer);
  klass->set_integer(self, key, value);
}

void
cern_property_store_set_object(CernPropertyStore *self, gint32 key, gpointer value) {
  g_return_if_fail(CERN_IS_PROPERTY_STORE(self));
  CernPropertyStoreClass *klass = CERN_PROPERTY_STORE_GET_CLASS(self);
  g_return_if_fail(klass->set_object);
  klass->set_object(self, key, value);
}

CernColor *
cern_property_store_get_color(CernPropertyStore *self, gint32 key) {
  g_return_val_if_fail(CERN_IS_PROPERTY_STORE(self), NULL);
  CernPropertyStoreClass *klass = CERN_PROPERTY_STORE_GET_CLASS(self);
  g_return_val_if_fail(klass->get_color, NULL);
  return klass->get_color(self, key);
}

CernColor *
cern_property_store_try_get_color(CernPropertyStore *self, gint32 key, gboolean *found) {
  g_return_val_if_fail(CERN_IS_PROPERTY_STORE(self), NULL);
  CernPropertyStoreClass *klass = CERN_PROPERTY_STORE_GET_CLASS(self);
  g_return_val_if_fail(klass->try_get_color, NULL);
  return klass->try_get_color(self, key, found);
}
