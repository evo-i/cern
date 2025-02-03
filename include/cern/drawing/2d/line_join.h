#ifndef CERN_DRAWING_2D_LINE_JOIN_H
#define CERN_DRAWING_2D_LINE_JOIN_H 1

/**
 * Specifies how to join two intersecting lines in a GraphicsPath at their intersection.
 */
typedef enum _CernLineJoin {
    /**
     * Specifies an angled miter join.
     */
    CernLineJoin_Miter = 0,
    /**
     * Specifies a beveled join.
     */
    CernLineJoin_Bevel = 1,
    /**
     * Specifies a smooth, rounded join.
     */
    CernLineJoin_Round = 2,
    /**
     * Specifies a mitered clipped join
     */
    CernLineJoin_MiterClipped = 3
} CernLineJoin;

#endif /* CERN_DRAWING_2D_LINE_JOIN_H */
