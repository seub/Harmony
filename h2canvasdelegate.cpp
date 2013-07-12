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
    drawCircle(0, 1);
    //std::cout << "Leaving H2CanvasDelegate::CanvasDelegate" << std::endl;
}

void H2CanvasDelegate::drawH2Point(const H2Point &p, const QColor &color, int width)
{
    complex z = p.getDiskCoordinate();
    drawPoint(z, color, width);
    return;
}

void H2CanvasDelegate::drawH2Geodesic(const H2Geodesic &L, const QColor &color, int width)
{
    if (L.isCircleInDiskModel())
    {
        Circle C;
        double angle1, angle2;
        L.getCircleAndAnglesInDiskModel(C, angle1, angle2);
        drawSmallerArc(C, angle1, angle2, color, width);
    }
    else
    {
        complex z1, z2;
        L.getEndpointsInDiskModel(z1, z2);
        drawSegment(z1, z2, color, width);
    }
    return;
}

void H2CanvasDelegate::drawH2GeodesicArc(const H2GeodesicArc &L, const QColor &color, int width)
{
    if (L.isCircleArcInDiskModel())
    {
        Circle C;
        double angle1, angle2;
        L.getCircleAndAnglesInDiskModel(C, angle1, angle2);
        drawSmallerArc(C, angle1, angle2, color, width);
    }
    else
    {
        complex z1, z2;
        L.getEndpointsInDiskModel(z1, z2);
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

void H2CanvasDelegate::drawGeneratorAxes(const IsomH2Representation &rho, const QColor &color, int width)
{
    std::vector<H2Isometry> generators = rho.getGeneratorImages();
    H2Geodesic L;
    for(unsigned int j=0;j<generators.size();j++)
    {
        generators[j].axis(L);
        drawH2Geodesic(L, color, width);
    }
    return;
}

void H2CanvasDelegate::drawExample()
{
    H2Point p1, p2, p3, p4, p5, p6;
    p1.setDiskCoordinate(-0.2+0.7*I);
    p3.setDiskCoordinate(-.8*I);
    p4.setDiskCoordinate(.5);
    p2.setDiskCoordinate(-.5-.5*I);
    p6.setDiskCoordinate(-.4);
    p5.setDiskCoordinate(.3+.5*I);

    H2Geodesic L1(p2,p1), L2(p2, p3), L3(p3, p4), L4(p4, p1);
/*    drawH2Geodesic(L1, "blue");
    drawH2Geodesic(L2, "blue");
    drawH2Geodesic(L3, "blue");
    drawH2Geodesic(L4, "blue");

    H2GeodesicArc A1(p1, p2), A2(p2, p3), A3(p3, p4), A4(p4, p1);
    drawH2GeodesicArc(A1, "red");
    drawH2GeodesicArc(A2, "red");
    drawH2GeodesicArc(A3, "red");
    drawH2GeodesicArc(A4, "red");

    H2Polygon P;
    P.addVertex(p1);
    P.addVertex(p2);
    P.addVertex(p3);
    P.addVertex(p4);
    P.addVertex(p5);
    drawH2Polygon(P,"red");

    SL2RMatrix A(0.5, 0.2, 5, 4);
    drawH2Polygon(A*P, "blue");
    drawH2Polygon(A.inverse()*P, "green");

    H2Isometry f1,f2,f;
    f1.setTranslationAxisAndLength(L1.swapOrientation(),2.0);
    f2.setTranslationAxisAndLength(L3,3.0);
    f.setByNormalizingPairWithRepulsivePointAtOne(f1,f2);
    H2Geodesic emptyL;
    f1.axis(emptyL);
    drawH2Geodesic(emptyL);
    f2.axis(emptyL);
    drawH2Geodesic(emptyL);
    H2Isometry f1new,f2new;
    f1new = f*f1*f.inverse();
    f2new = f*f2*f.inverse();
    f1new.axis(emptyL);
    drawH2Geodesic(emptyL,"blue");
    f2new.axis(emptyL);
    drawH2Geodesic(emptyL,"blue");


    H2Point p0;
    p0.setDiskCoordinate(.4*I);
    drawH2Point(p0,"black",8);
    H2Isometry F;
    F.setByNormalizingPairWithChosenNearestPointToAxis(f1,f2,imag(p0.getDiskCoordinate()));
    f1new = F*f1*F.inverse();
    f2new = F*f2*F.inverse();
    f1new.axis(emptyL);
    drawH2Geodesic(emptyL,"red");
    f2new.axis(emptyL);
    drawH2Geodesic(emptyL,"red");
    H2Geodesic perp;
    commonPerpendicular(emptyL,H2Geodesic(-I,I),perp);
    drawH2Geodesic(perp,"green");
 */



    return;
}
