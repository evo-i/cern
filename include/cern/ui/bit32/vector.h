#ifndef CERN_UI_BIT32_VECTOR_H_DEFINED
#define CERN_UI_BIT32_VECTOR_H_DEFINED 1

#include <glib-object.h>

G_BEGIN_DECLS

typedef struct _CernBitVector32 {
  guint32 data;
} CernBitVector32;

typedef struct _CernBitVector32Section {
  guint16 mask;
  guint16 offset;
} CernBitVector32Section;

gboolean
cern_bit_vector32_get(CernBitVector32 *vector);

void
cern_bit_vector32_set(CernBitVector32 *vector, gint32 bit);

gint32
cern_bit_vector32_section_get(CernBitVector32 *vector, CernBitVector32Section section);

void
cern_bit_vector32_section_set(CernBitVector32 *vector, CernBitVector32Section section, gint32 value);

gint32
cern_bit_vector32_get_data(CernBitVector32 *vector);

guint16
cern_bit_vector32_get_count_bits_set(guint16 mask);

gint32
cern_bit_vector32_create_mask(void);

gint32
cern_bit_vector32_create_mask_ex(gint32 previous);

CernBitVector32Section
cern_bit_vector32_create_section(guint16 max_value);

CernBitVector32Section
cern_bit_vector32_create_section_ex(guint16 max_value, CernBitVector32Section previous);

gboolean
cern_bit_vector32_section_is_equals(CernBitVector32Section first, CernBitVector32Section second);

G_END_DECLS

#endif /* CERN_UI_BIT32_VECTOR_H_DEFINED */
