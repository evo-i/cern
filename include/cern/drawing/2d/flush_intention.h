#ifndef CERN_DRAWING_2D_FLUSH_INTENTION_H
#define CERN_DRAWING_2D_FLUSH_INTENTION_H

/**
 * @enum CernFlushIntention
 * @brief Specifies whether commands in the graphics stack are terminated (flushed) immediately or executed as soon as possible.
 */
typedef enum _CernFlushIntention {

  /**
   * @brief Specifies the stack of all graphics operations is flushed immediately.
   */
  CernFlushIntention_Flush = 0,

  /**
   * @brief Specifies that all graphics operations on the stack are executed as soon as possible. This synchronizes the graphics state.
   */
  CernFlushIntention_Sync = 1
} CernFlushIntention;

#endif /* CERN_DRAWING_2D_FLUSH_INTENTION_H */
