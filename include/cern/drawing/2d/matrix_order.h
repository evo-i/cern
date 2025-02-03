#ifndef CERN_DRAWING_2D_MATRIX_ORDER_H
#define CERN_DRAWING_2D_MATRIX_ORDER_H 1

/**
 * Specifies the order for matrix transform operations.
 */
typedef enum _CernMatrixOrder {
  /**
   * The new operation is applied before the old operation.
   */
  CernMatrixOrder_Prepend = 0,
  /**
   * The new operation is applied after the old operation.
   */
  CernMatrixOrder_Append = 1
} CernMatrixOrder;

#endif /* CERN_DRAWING_2D_MATRIX_ORDER_H */
