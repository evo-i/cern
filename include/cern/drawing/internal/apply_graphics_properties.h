#ifndef CERN_DRAWING_INTERNAL_APPLY_GRAPHICS_PROPERTIES_H
#define CERN_DRAWING_INTERNAL_APPLY_GRAPHICS_PROPERTIES_H 1

/**
 * @enum CernApplyGraphicsProperties
 * @brief Flags to specify which graphics properties should be applied.
 *
 * This enumeration defines flags that can be used to control which properties
 * of a Graphics object should be applied to a device context (DC).
 */
typedef enum _CernApplyGraphicsProperties {
    /** No properties to be applied to the DC obtained from the Graphics object. */
    CernApplyGraphicsProperties_None                = 0x00000000,
    /** Apply clipping region. */
    CernApplyGraphicsProperties_Clipping            = 0x00000001,
    /** Apply coordinate transformation. */
    CernApplyGraphicsProperties_TranslateTransform  = 0x00000002,
    /** Apply all supported Graphics properties. */
    CernApplyGraphicsProperties_All                 =
        CernApplyGraphicsProperties_Clipping
        | CernApplyGraphicsProperties_TranslateTransform
} CernApplyGraphicsProperties;


#endif /* CERN_DRAWING_INTERNAL_APPLY_GRAPHICS_PROPERTIES_H */
