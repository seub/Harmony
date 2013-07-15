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
#include "drawelement.h"

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

void H2CanvasDelegate::setMouse(int mouseX, int mouseY)
{
    this->mouseX = mouseX;
    this->mouseY = mouseY;
    savedmobius = mobius;
    redrawBuffer();
}

void H2CanvasDelegate::moveMouse(int mouseX, int mouseY)
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
    return;
}
