#ifndef H2CANVASDELEGATE_H
#define H2CANVASDELEGATE_H

#include "tools.h"
#include "canvasdelegate.h"
#include "h2point.h"
#include "h2geodesic.h"
#include "h2polygon.h"
#include "grouprepresentation.h"
#include "h2buffer.h"

class H2CanvasDelegate : public CanvasDelegate
{
public:
    H2CanvasDelegate(Canvas *canvas);

    void drawComplexPointInDiskModel(const complex & z, const QColor & color = "black", int width = 1);
    void drawH2Point(const H2Point & p, const QColor & color = "black", int width = 4);
    void drawH2Geodesic(const H2Geodesic & L, const QColor & color = "black", int width = 1);
    void drawH2GeodesicArc(const H2GeodesicArc &L, const QColor & color = "black", int width = 2);
    void drawH2Polygon(const H2Polygon & P, const QColor & color = "black", int width = 2);

    void drawGeneratorAxes(const IsomH2Representation & rho, const QColor & color = "black", int width = 2);
    void redrawBuffer(const H2Isometry &mobius = H2Isometry::identity());
    void mousePress(QMouseEvent * mouseEvent);
    void mouseMove(QMouseEvent * mouseEvent);
    void keyPress(QKeyEvent * keyEvent);

//private:
    H2Buffer buffer;
    H2Isometry mobius;
    H2Isometry savedMobius;
};


#endif // H2CANVASDELEGATE_H
