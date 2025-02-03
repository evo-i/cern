#ifndef CERN_IMAGING_LOCK_MODE_H
#define CERN_IMAGING_LOCK_MODE_H 1

#include <glib-object.h>

G_BEGIN_DECLS

/**
 * @brief Indicates the access mode for an CernImage.
 */
typedef enum _CernImageLockMode {
  /**
   * @brief Specifies the image is read-only.
   */
  CernImageLockMode_ReadOnly        = 0x0001,
  /**
   * @brief Specifies the image is write-only.
   */
  CernImageLockMode_WriteOnly       = 0x0002,
  /**
   * @brief Specifies the image is read-write.
   */
  CernImageLockMode_ReadWrite
    = CernImageLockMode_ReadOnly
      | CernImageLockMode_WriteOnly,
  /**
   * @brief Indicates the image resides in a user input buffer, to which the user controls access.
   */
  CernImageLockMode_UserInputBuffer = 0x0004,
} CernImageLockMode;
G_END_DECLS

#endif /* CERN_IMAGING_LOCK_MODE_H */
