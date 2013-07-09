#ifndef H2CANVASDELEGATE_H
#define H2CANVASDELEGATE_H

#include "types.h"
#include "canvasdelegate.h"


class H2Geodesic;
class H2Point;
class H2GeodesicArc;
class H2Polygon;

class H2CanvasDelegate : public CanvasDelegate
{
public:
    H2CanvasDelegate(Canvas *canvas);

    void drawH2Point(const H2Point & p, const QColor & color = "black", int width = 4);
    void drawH2Geodesic(const H2Geodesic & L, const QColor & color = "black", int width = 1);
    void drawH2GeodesicArc(const H2GeodesicArc &L, const QColor & color = "black", int width = 2);
    void drawH2Polygon(const H2Polygon & P, const QColor & color = "black", int width = 2);

    void drawExample();
    void drawGeneratorsOfIsomH2Representation(const IsomH2Representation & rho, const QColor & color = "black", int width = 2);

private:
};

#endif // H2CANVASDELEGATE_H
