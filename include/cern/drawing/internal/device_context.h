#ifndef CERN_DRAWING_DEVICE_CONTEXT_H
#define CERN_DRAWING_DEVICE_CONTEXT_H 1

#include <glib-object.h>

G_BEGIN_DECLS

#include "device_context_type.h"
#include "gdi_object_type.h"
#include "device_context_graphics_mode.h"
#include "windows_region.h"
#include "windows_font.h"
#include "device_context_background_mode.h"
#include "device_context_binary_raster_operation_flags.h"
#include "device_context_text_alignment.h"
#include "../device_capabilities.h"
#include "device_context_map_mode.h"
#include "../color.h"
#include "../point.h"
#include "../size.h"

typedef struct _devicemodeA DEVMODEA;
typedef struct _devicemodeW DEVMODEW;

#define CERN_TYPE_DEVICE_CONTEXT cern_device_context_get_type ()

G_DECLARE_FINAL_TYPE(CernDeviceContext,
                     cern_device_context,
                     CERN, DEVICE_CONTEXT,
                     GObject)

/**
 * @brief Creates a new CernDeviceContext object for a device context (DC).
 *
 * This function initializes and returns a new CernDeviceContext object
 * configured as a device context. It uses the provided driver, device,
 * and file information to create the context.
 *
 * @param driver_name A null-terminated string specifying the name of the device driver.
 * @param device_name A null-terminated string specifying the name of the specific output device.
 * @param file_name A null-terminated string specifying the name of the file to be printed to.
 *                  This parameter can be NULL.
 * @param dev_mode A pointer to a DEVMODEA structure containing device-specific initialization data.
 *                 This parameter can be NULL.
 *
 * @return A pointer to the newly created CernDeviceContext object configured as a
 *         device context. Returns NULL if the creation fails.
 */
CernDeviceContext *
cern_device_context_new_dc(gchar const *driver_name, gchar const *device_name,
                           gchar const *file_name, DEVMODEA const *dev_mode);


/**
 * @brief Creates a new CernDeviceContext object for an information context (IC).
 *
 * This function initializes and returns a new CernDeviceContext object
 * configured as an information context. It uses the provided driver, device,
 * and file information to create the context.
 *
 * @param driver_name A null-terminated string specifying the name of the device driver.
 * @param device_name A null-terminated string specifying the name of the specific output device.
 * @param file_name A null-terminated string specifying the name of the file to be printed to.
 *                  This parameter can be NULL.
 * @param dev_mode A pointer to a DEVMODEA structure containing device-specific initialization data.
 *                 This parameter can be NULL.
 *
 * @return A pointer to the newly created CernDeviceContext object configured as an
 *         information context. Returns NULL if the creation fails.
 */
CernDeviceContext *
cern_device_context_new_ic(gchar const *driver_name, gchar const *device_name,
                           gchar const *file_name, DEVMODEA const *dev_mode);


/**
 * @brief Creates a new CernDeviceContext object from a compatible device context.
 *
 * This function initializes and returns a new CernDeviceContext object
 * using an existing compatible device context. It allows for the creation
 * of a new device context that is compatible with the provided one.
 *
 * @param h_dc A gpointer representing the handle to a compatible device context (HDC)
 *             from which the new CernDeviceContext will be created.
 *
 * @return A pointer to the newly created CernDeviceContext object.
 *         Returns NULL if the creation fails.
 */
CernDeviceContext *
cern_device_context_new_from_compatible_dc(gpointer h_dc);

/**
 * @brief Creates a new CernDeviceContext object from a window handle.
 *
 * This function initializes and returns a new CernDeviceContext object
 * associated with the specified window handle. It allows for device
 * context operations to be performed on the given window.
 *
 * @param h_window A gpointer representing the handle to the window (HWND)
 *                 for which the device context is to be created.
 *
 * @return A pointer to the newly created CernDeviceContext object.
 *         Returns NULL if the creation fails.
 */
CernDeviceContext *
cern_device_context_new(gpointer h_window);

/**
 * @brief Creates a new CernDeviceContext object from an existing HDC.
 *
 * This function initializes a new CernDeviceContext object using a provided
 * handle to a device context (HDC) and a specified device context type.
 *
 * @param h_dc A gpointer representing the handle to an existing device context (HDC).
 * @param type The CernDeviceContextType specifying the type of the device context.
 *
 * @return A pointer to the newly created CernDeviceContext object.
 *         Returns NULL if the creation fails.
 */
CernDeviceContext *
cern_device_context_new_hdc(gpointer h_dc, CernDeviceContextType type);


/**
 * @brief Specifies whether a modification has been applied to the dc,
 * like setting the clipping area or a coordinate transform.
 * The device type the context refers to.
 *
 * @param self Pointer to a #CernDeviceContext object.
 * @return CernDeviceContextType
 */
CernDeviceContextType
cern_device_context_get_device_context_type(CernDeviceContext *self);

/**
 * @brief Deletes a GDI object associated with the specified device context.
 *
 * This function is responsible for deleting a GDI object of a specified type
 * that is associated with the given CernDeviceContext. It ensures proper
 * resource management by releasing the resources held by the GDI object.
 *
 * @param self Pointer to a #CernDeviceContext object from which the GDI object
 *             is to be deleted.
 * @param obj A gpointer representing the GDI object to be deleted. This object
 *            should be of the type specified by the 'type' parameter.
 * @param type The type of the GDI object to be deleted, specified as a
 *             #CernGdiObjectType. This parameter indicates the specific type
 *             of GDI object that 'obj' represents.
 */
void
cern_device_context_delete_object(CernDeviceContext *self,
                                  gpointer obj, CernGdiObjectType type);


/**
 * @brief Retrieves the handle to the device context (HDC) associated with the given CernDeviceContext.
 *
 * This function provides access to the underlying HDC for performing GDI operations.
 *
 * @param self Pointer to a #CernDeviceContext object from which the HDC is to be retrieved.
 * @return A gpointer representing the handle to the device context (HDC).
 *         This handle can be used for GDI operations. If the operation fails, the return value is NULL.
 */
gpointer
cern_device_context_get_hdc(CernDeviceContext *self);

/**
 * @brief Releases the handle to the device context (HDC) associated with the given CernDeviceContext.
 *
 * This function is used to release the HDC that was previously retrieved using
 * cern_device_context_get_hdc(). It is important to call this function to ensure
 * proper resource management and avoid resource leaks.
 *
 * @param self Pointer to a #CernDeviceContext object from which the HDC is to be released.
 */
void
cern_device_context_release_hdc(CernDeviceContext *self);

/**
 * @brief Retrieves the current graphics mode of the specified device context.
 *
 * This function returns the graphics mode that is currently set for the given
 * CernDeviceContext. The graphics mode determines how graphics operations are
 * performed on the device context.
 *
 * @param self Pointer to a #CernDeviceContext object from which the graphics mode
 *             is to be retrieved.
 * @return The current #CernDeviceContextGraphicsMode of the specified device context.
 *         This value indicates the graphics mode that is currently active.
 */
CernDeviceContextGraphicsMode
cern_device_context_get_graphics_mode(CernDeviceContext *self);

/**
 * @brief Sets the graphics mode for the specified device context.
 *
 * This function changes the graphics mode of the given CernDeviceContext to the specified new mode.
 * The graphics mode determines how graphics operations are performed on the device context.
 *
 * @param self Pointer to a #CernDeviceContext object for which the graphics mode is to be set.
 * @param new_mode The new #CernDeviceContextGraphicsMode to be set for the device context.
 *                 This value specifies the graphics mode that will be applied.
 */
CernDeviceContextGraphicsMode
cern_device_context_set_graphics_mode(CernDeviceContext *self,
                                      CernDeviceContextGraphicsMode new_mode);

/**
 * @brief Restores the previously saved device context (HDC) state.
 *
 * This function restores the state of the device context (HDC) associated with
 * the given CernDeviceContext object to the last saved state. It is used to
 * revert any changes made to the device context since the last save operation.
 *
 * @param self Pointer to a #CernDeviceContext object whose HDC state is to be restored.
 */
void
cern_device_context_restore_hdc(CernDeviceContext *self);


/**
 * @brief Saves the current state of the device context (HDC) associated with the CernDeviceContext.
 *
 * This function saves the current state of the device context, including graphics mode,
 * colors, objects, and other attributes. The saved state can later be restored using
 * cern_device_context_restore_hdc().
 *
 * @param self Pointer to the CernDeviceContext object whose HDC state is to be saved.
 *             This parameter must not be NULL.
 *
 * @return This function does not return a value.
 */
void
cern_device_context_save_hdc(CernDeviceContext *self);

/**
 * @brief Sets the clipping region for the device context.
 *
 * This function defines a new clipping region for the specified device context.
 * The clipping region determines the area within which drawing operations can occur.
 *
 * @param self Pointer to the CernDeviceContext object for which the clipping region is to be set.
 *             This parameter must not be NULL.
 * @param region Pointer to a CernWindowsRegion object that defines the new clipping region.
 *               If NULL, the clipping region is reset to the default (entire device surface).
 *
 * @return This function does not return a value.
 */
void
cern_device_context_set_clip(CernDeviceContext *self,
                             CernWindowsRegion *region);


/**
 * @brief Intersects the current clipping region of the device context with the specified region.
 *
 * This function modifies the current clipping region of the device context by intersecting it with the
 * specified CernWindowsRegion. The resulting clipping region will be the area common to both the
 * current clipping region and the specified region.
 *
 * @param self Pointer to the CernDeviceContext object whose clipping region is to be modified.
 *             This parameter must not be NULL.
 * @param region Pointer to the CernWindowsRegion object that defines the region to intersect with the
 *               current clipping region. If NULL, the clipping region is reset to the default (entire device surface).
 */
void
cern_device_context_intersect_clip(CernDeviceContext *self,
                                   CernWindowsRegion *region);

/**
 * @brief Translates the coordinate system of the device context by the specified offsets.
 *
 * This function modifies the coordinate system of the device context by applying a translation
 * transformation. The translation is specified by the horizontal offset (dx) and vertical offset (dy).
 *
 * @param self Pointer to the CernDeviceContext object whose coordinate system is to be translated.
 *             This parameter must not be NULL.
 * @param dx The horizontal offset, in device units, by which to translate the coordinate system.
 * @param dy The vertical offset, in device units, by which to translate the coordinate system.
 *
 * @return This function does not return a value.
 */
void
cern_device_context_translate_transform(CernDeviceContext *self,
                                        gint32 dx, gint32 dy);

/**
 * @brief Compares two CernDeviceContext objects for equality.
 *
 * This function compares the contents of two CernDeviceContext objects to determine if they are equal.
 * The comparison is based on the internal state of the objects, including the clipping region, coordinate
 * system transformations, and other device-specific properties.
 *
 * @param self Pointer to the first CernDeviceContext object to compare.
 * @param other Pointer to the second CernDeviceContext object to compare.
 *
 * @return TRUE if the two CernDeviceContext objects are equal, FALSE otherwise.
 */
gboolean
cern_device_context_equals(CernDeviceContext *self, CernDeviceContext *other);

CernWindowsFont *
cern_device_context_get_active_font(CernDeviceContext *self);

CernColor
cern_device_context_get_background_color(CernDeviceContext *self);

CernColor
cern_device_context_set_background_color(CernDeviceContext *self, CernColor color);

CernDeviceContextBackgroundMode
cern_device_context_get_background_mode(CernDeviceContext *self);

CernDeviceContextBackgroundMode
cern_device_context_set_background_mode(CernDeviceContext *self,
                                        CernDeviceContextBackgroundMode mode);

CernDeviceContextBinaryRasterOperationFlags
cern_device_context_get_binary_raster_operation(CernDeviceContext *self);

CernDeviceContextBinaryRasterOperationFlags
cern_device_context_set_binary_raster_operation(CernDeviceContext *self,
                                                CernDeviceContextBinaryRasterOperationFlags flags);

CernSize
cern_device_context_get_dpi(CernDeviceContext *self);

gint32
cern_device_context_get_dpi_x(CernDeviceContext *self);

gint32
cern_device_context_get_dpi_y(CernDeviceContext *self);

CernWindowsFont *
cern_device_context_get_font(CernDeviceContext *self);

void
cern_device_context_set_font(CernDeviceContext *self, 
                             CernWindowsFont *new_font);

CernDeviceContext *
cern_device_context_get_screen_dc(void);

gpointer
cern_device_context_select_font(CernDeviceContext *self,
                                CernWindowsFont *font);

void
cern_device_context_reset_font(CernDeviceContext *self);

gint32
cern_device_context_get_device_capabilities(CernDeviceContext *self,
                                            CernDeviceCapabilities capability_index);

CernDeviceContextMapMode
cern_device_context_get_map_mode(CernDeviceContext *self);

CernDeviceContextMapMode
cern_device_context_set_map_mode(CernDeviceContext *self,
                                 CernDeviceContextMapMode value);

gboolean
cern_device_context_get_is_font_on_context_stack(CernDeviceContext *self,
                                                 CernWindowsFont *font);

gpointer
cern_device_context_select_object(CernDeviceContext *self,
                                  gpointer h_object,
                                  CernGdiObjectType type);
CernDeviceContextTextAlignment
cern_device_context_get_text_alignment(CernDeviceContext *self);

CernDeviceContextTextAlignment
cern_device_context_set_text_alignment(CernDeviceContext *self, 
                                       CernDeviceContextTextAlignment value);

CernColor
cern_device_context_get_text_color(CernDeviceContext *self);

CernColor
cern_device_context_set_text_color(CernDeviceContext *self,
                                   CernColor color);

CernSize
cern_device_context_get_viewport_extent(CernDeviceContext *self);

CernSize
cern_device_context_set_viewport_extent(CernDeviceContext *self,
                                        CernSize value);

CernPoint
cern_device_context_get_viewport_origin(CernDeviceContext *self);

CernPoint
cern_device_context_set_viewport_origin(CernDeviceContext *self,
                                        CernPoint value);

G_END_DECLS

#endif /* CERN_DRAWING_DEVICE_CONTEXT_H */
