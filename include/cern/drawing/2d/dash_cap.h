#ifndef CERN_DRAWING_2D_DASH_CAP_H
#define CERN_DRAWING_2D_DASH_CAP_H 1

/**
 * @brief Specifies the available dash cap styles with which a Pen can end a line.
 */
typedef enum _CernDashCap {
    /**
     * @brief Flat dash cap style
     */
    CernDashCap_Flat        = 0,
    /**
     * @brief Round dash cap style
     */
    CernDashCap_Round       = 2,
    /**
     * @brief Triangle dash cap style
     */
    CernDashCap_Triangle    = 3
} CernDashCap;

#endif /* CERN_DRAWING_2D_DASH_CAP_H */
