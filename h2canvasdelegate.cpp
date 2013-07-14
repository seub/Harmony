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
    if ((mobius*L).isCircleInDiskModel())
    {
        Circle C;
        double angle1, angle2;
        (mobius*L).getCircleAndAnglesInDiskModel(C, angle1, angle2);
        drawSmallerArc(C, angle1, angle2, color, width);
    }
    else
    {
        complex z1, z2;
        (mobius*L).getEndpointsInDiskModel(z1, z2);
        drawSegment(z1, z2, color, width);
    }
    return;
}

void H2CanvasDelegate::drawH2GeodesicArc(const H2GeodesicArc &L, const QColor &color, int width)
{
    if ((mobius*L).isCircleArcInDiskModel())
    {
        Circle C;
        double angle1, angle2;
        (mobius*L).getCircleAndAnglesInDiskModel(C, angle1, angle2);
        drawSmallerArc(C, angle1, angle2, color, width);
    }
    else
    {
        complex z1, z2;
        (mobius*L).getEndpointsInDiskModel(z1, z2);
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

void H2CanvasDelegate::drawListOfAxesOfIsometries(const std::vector<H2Isometry> &isometries, const QColor &color, int width)
{
    H2Geodesic L;
    for(unsigned int j=0;j<isometries.size();j++)
    {
        isometries[j].axis(L);
        drawH2Geodesic(L, color, width);
    }
    return;
}

void H2CanvasDelegate::redrawBuffer(const H2Isometry &mobius)
{
    image->fill("white");
    drawCircle(0, 1);
    this->mobius = mobius*this->mobius;
    unsigned int i;
    for(i=0;i<buffer.size();i++)
    {
        buffer[i]->drawElement(this);
    }
}

void H2CanvasDelegate::addElementToBuffer(DrawElement &element)
{
    buffer.push_back(&element);
}

void H2CanvasDelegate::setMouse(const int mouseX, const int mouseY)
{
    this->mouseX = mouseX;
    this->mouseY = mouseY;
    savedmobius = mobius;
    redrawBuffer();
}

void H2CanvasDelegate::moveMouse(const int mouseX, const int mouseY)
{
    H2Isometry changemobius;
    complex oldMouse(PixelToComplexCoordinates(this->mouseX,this->mouseY));
    complex newMouse(PixelToComplexCoordinates(mouseX,mouseY));
    /*complex a((oldMouse.real()-newMouse.real())/(oldMouse.real()*newMouse.real()-1),
              (newMouse.imag()-oldMouse.imag())/(oldMouse.imag()*newMouse.imag()+1));
    changemobius.setDiskCoordinates(1.0,a);*/
    changemobius.setByMappingBoundaryAndInteriorPoints(-1.0,-1.0,oldMouse,newMouse);
    mobius = changemobius*savedmobius;
    redrawBuffer();
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

DrawH2Point::DrawH2Point(const H2Point & p, const  QColor  &color, int width, int identifiant):
    DrawElement(color, width, identifiant), p(p)
{

}

void DrawH2Point::drawElement(H2CanvasDelegate *canvas)
{
    canvas->drawH2Point(p, color, width);
}

DrawH2Geodesic::DrawH2Geodesic(const H2Geodesic & L, const QColor & color, int width, int identifiant):
    DrawElement(color, width, identifiant), L(L)
{

}

void DrawH2Geodesic::drawElement(H2CanvasDelegate *canvas)
{
    canvas->drawH2Geodesic(L, color, width);
}

DrawH2GeodesicArc::DrawH2GeodesicArc(const H2GeodesicArc & L, const QColor & color, int width, int identifiant):
    DrawElement(color, width, identifiant), L(L)
{

}

void DrawH2GeodesicArc::drawElement(H2CanvasDelegate *canvas)
{
    canvas->drawH2GeodesicArc(L, color, width);
}

DrawH2Polygon::DrawH2Polygon(const H2Polygon & P, const QColor & color, int width, int identifiant):
    DrawElement(color, width, identifiant), P(P)
{

}

void DrawH2Polygon::drawElement(H2CanvasDelegate *canvas)
{
    canvas->drawH2Polygon(P, color, width);
}

DrawGeneratorAxes::DrawGeneratorAxes(const IsomH2Representation & rho, const QColor & color, int width, int identifiant):
    DrawElement(color, width, identifiant), rho(rho)
{

}

void DrawGeneratorAxes::drawElement(H2CanvasDelegate *canvas)
{
    canvas->drawGeneratorAxes(rho, color, width);
}

DrawListOfAxesOfIsometries::DrawListOfAxesOfIsometries(const std::vector<H2Isometry> &isometries,
                                                       const QColor &color, int width, int identifiant) :
    DrawElement(color, width, identifiant), isometries(isometries)
{

}

void DrawListOfAxesOfIsometries::drawElement(H2CanvasDelegate *canvas)
{
    canvas->drawListOfAxesOfIsometries(isometries,color,width);
}
