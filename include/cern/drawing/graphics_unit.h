#ifndef CERN_GRAPHICS_UNIT_H
#define CERN_GRAPHICS_UNIT_H 1

/// @brief Specifies the unit of measure for the given data.
typedef enum _CernGraphicsUnit {
  /// @brief Specifies the world unit as the unit of measure.
  CernGraphicsUnit_World = 0 ,     // 0 -- World coordinate (non-physical unit)

  /// @brief Specifies 1/75 inch as the unit of measure.
  CernGraphicsUnit_Display = 1,    // 1 -- Variable - for PageTransform only

  /// @brief Specifies a device pixel as the unit of measure.
  CernGraphicsUnit_Pixel = 2,      // 2 -- Each unit is one device pixel.

  /// @brief Specifies a printer's point (1/72 inch) as the unit of measure.
  CernGraphicsUnit_Point = 3,      // 3 -- Each unit is a printer's point, or 1/72 inch.

  /// @brief Specifies the inch as the unit of measure.
  CernGraphicsUnit_Inch = 4,       // 4 -- Each unit is 1 inch.

  /// @brief Specifes the document unit (1/300 inch) as the unit of measure.
  CernGraphicsUnit_Document = 5,   // 5 -- Each unit is 1/300 inch.

  /// @brief Specifies the millimeter as the unit of measure.
  CernGraphicsUnit_Millimeter = 6  // 6 -- Each unit is 1 millimeter.
} CernGraphicsUnit;
#endif /* CERN_GRAPHICS_UNIT_H */
