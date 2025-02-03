#ifndef CERN_DRAWING_2D_FILL_MODE_H
#define CERN_DRAWING_2D_FILL_MODE_H 1

#include <glib-object.h>

G_BEGIN_DECLS

typedef enum _CernFillMode {
  /**
   * Odd-even fill rule
   */
  CernFillMode_Alternate = 0,

  /**
   * Non-zero winding fill rule
   */
  CernFillMode_Winding = 1
} CernFillMode;

G_END_DECLS

#endif /* CERN_DRAWING_2D_FILL_MODE_H */
