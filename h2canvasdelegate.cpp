#include "h2canvasdelegate.h"

#include <QPen>

#include "h2point.h"
#include "h2geodesic.h"
#include "circle.h"
#include "planarline.h"
#include "h2polygon.h"
#include "sl2rmatrix.h"
#include "h2isometry.h"
#include "grouprepresentation.h"

H2CanvasDelegate::H2CanvasDelegate(Canvas *canvas) : CanvasDelegate(canvas)
{
    //std::cout << "Entering H2CanvasDelegate::CanvasDelegate" << std::endl;
    mobius.setIdentity();
    drawCircle(0, 1);

    //std::cout << "Leaving H2CanvasDelegate::CanvasDelegate" << std::endl;
}

void H2CanvasDelegate::drawH2Point(const H2Point &p, const QColor &color, int width)
{
    complex z = (mobius*p).getDiskCoordinate();
    drawPoint(z, color, width);
    return;
}

void H2CanvasDelegate::drawH2Geodesic(const H2Geodesic &L, const QColor &color, int width)
{
    H2Geodesic tempGeodesic =  mobius*L;
    if (tempGeodesic.isCircleInDiskModel())
    {
        Circle C;
        double angle1, angle2;
        tempGeodesic.getCircleAndAnglesInDiskModel(C, angle1, angle2);
        drawSmallerArc(C, angle1, angle2, color, width);
    }
    else
    {
        complex z1, z2;
        tempGeodesic.getEndpointsInDiskModel(z1, z2);
        drawSegment(z1, z2, color, width);
    }
    return;
}

void H2CanvasDelegate::drawH2GeodesicArc(const H2GeodesicArc &L, const QColor &color, int width)
{
    H2GeodesicArc tempGeodesic =  mobius*L;
    if(tempGeodesic.isCircleArcInDiskModel())
    {
        Circle C;
        double angle1, angle2;
        tempGeodesic.getCircleAndAnglesInDiskModel(C, angle1, angle2);
        drawSmallerArc(C, angle1, angle2, color, width);
    }
    else
    {
        complex z1, z2;
        tempGeodesic.getEndpointsInDiskModel(z1, z2);
        drawSegment(z1, z2, color, width);
    }
    return;
}

void H2CanvasDelegate::drawH2Polygon(const H2Polygon &P, const QColor &color, int width)
{
    int sizePoly = P.getNumberOfVertices();
    for (int i=0; i<sizePoly ; i++)
    {
        drawH2GeodesicArc(P.getSide(i),color,width);
        drawH2Point(P.getVertex(i));
    }
    return;
}

void H2CanvasDelegate::redrawBuffer(const H2Isometry &mobius)
{
    image->fill("white");
    drawCircle(0, 1);

    this->mobius = mobius*this->mobius;


    int i;

    int nbPolygons = buffer.polygons.size();
    for (i = 0; i < nbPolygons; i++)
    {
        drawH2Polygon(buffer.polygons[i], buffer.polygonsColors[i], buffer.polygonsWidths[i]);
    }

    int nbGeodesics = buffer.geodesics.size();
    for (i = 0; i < nbGeodesics; i++)
    {
        drawH2Geodesic(buffer.geodesics[i], buffer.geodesicsColors[i], buffer.geodesicsWidths[i]);
    }

    int nbPoints = buffer.points.size();
    for (i = 0; i < nbPoints; i++)
    {
        drawH2Point(buffer.points[i], buffer.pointsColors[i], buffer.pointsWidths[i]);
    }


    return;
}


void H2CanvasDelegate::setMouse(int mouseX, int mouseY)
{
    this->mouseX = mouseX;
    this->mouseY = mouseY;
    savedMobius = mobius;
    redrawBuffer();
}

void H2CanvasDelegate::mouseMove(int mouseX, int mouseY)
{
    H2Isometry mobiusChange;
    complex mouseOld(PixelToComplexCoordinates(this->mouseX,this->mouseY));
    complex mouseNew(PixelToComplexCoordinates(mouseX,mouseY));

    mobiusChange.setByMappingPointInDiskModelNormalized(mouseOld, mouseNew);
    mobius = mobiusChange*savedMobius;
    redrawBuffer();
    return;
}
