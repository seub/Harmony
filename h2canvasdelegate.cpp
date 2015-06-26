#include "h2canvasdelegate.h"

#include <QPen>
#include <QPainter>
#include <QMouseEvent>
#include <QApplication>

#include "h2point.h"
#include "h2geodesic.h"
#include "circle.h"
#include "planarline.h"
#include "h2polygon.h"
#include "sl2rmatrix.h"
#include "h2isometry.h"
#include "grouprepresentation.h"
#include "h2canvasdelegate.h"
#include "actionhandler.h"

H2CanvasDelegate::H2CanvasDelegate(uint sizeX, uint sizeY, bool leftCanvas, bool rightCanvas, ActionHandler *handler) :
    CanvasDelegate(sizeX, sizeY, leftCanvas, rightCanvas, handler)
{
    resetView();
}

H2Point H2CanvasDelegate::pixelToH2coordinate(int x, int y) const
{
    H2Point point;
    point.setDiskCoordinate(PixelToComplexCoordinates(x,y));
    return mobius.inverse()*point;
}

void H2CanvasDelegate::resetView()
{
    CanvasDelegate::resetView();
    mobius.setIdentity();
    mobiusing = false;
}

void H2CanvasDelegate::drawH2Point(const H2Point &p, const QColor &color, int width, bool back)
{
    Complex z = (mobius*p).getDiskCoordinate();
    drawPoint(z, color, width, back);
}

void H2CanvasDelegate::drawH2Geodesic(const H2Geodesic &L, const QColor &color, int width, bool back)
{
    Complex center, endpoint1, endpoint2;
    double radius;
    if ((mobius*L).getCircleAndEndpointsInDiskModel(center, radius, endpoint1, endpoint2))
    {

        drawSmallerArc(center, radius, endpoint1, endpoint2, color, width, back);
    }
    else
    {
        drawSegment(endpoint1, endpoint2, color, width, back);
    }
}

void H2CanvasDelegate::drawH2GeodesicArc(const H2GeodesicArc &L, const QColor &color, int width, bool back)
{
    Complex center, endpoint1, endpoint2;
    double radius;
    if((mobius*L).getCircleAndEndpointsInDiskModel(center, radius, endpoint1, endpoint2))
    {
        drawSmallerArc(center, radius, endpoint1, endpoint2, color, width, back);
    }
    else
    {
        drawSegment(endpoint1, endpoint2, color, width, back);
    }
}

void H2CanvasDelegate::drawStraightH2GeodesicArc(const H2GeodesicArc &L, const QColor &color, int width, bool back)
{
    Complex zA, zB;
    (mobius*L).getEndpointsInDiskModel(zA, zB);
    drawSegment(zA, zB, color, width, back);
}

void H2CanvasDelegate::drawH2Triangle(const H2Triangle &triangle, const QColor &color, int width, bool back)
{
    std::vector<H2GeodesicArc> sides = triangle.getSides();
    for (const auto &side : sides)
    {
        drawH2GeodesicArc(side, color, width, back);
    }
}

void H2CanvasDelegate::drawStraightFilledH2Triangle(const H2Triangle &triangle, bool back)
{
    H2Point A, B, C;
    (mobius*triangle).getPoints(A, B, C);
    Complex zA = A.getDiskCoordinate();
    Complex zB = B.getDiskCoordinate();
    Complex zC = C.getDiskCoordinate();
    drawFilledTriangle(zA, zB, zC, back);
}

void H2CanvasDelegate::redrawBack()
{
    CanvasDelegate::redrawBack();
    drawCircle(0, 1);
}

void H2CanvasDelegate::redrawTop()
{
    CanvasDelegate::redrawTop();
}


void H2CanvasDelegate::mousePress(QMouseEvent *mouseEvent)
{
    if(mouseEvent->button()== Qt::LeftButton)
    {
        this->mouseX = mouseEvent->x();
        this->mouseY = mouseEvent->y();
        if (QApplication::keyboardModifiers() == Qt::ShiftModifier)
        {
            xMinSave = xMin;
            yMaxSave = yMax;
        }
        else
        {
            if (norm(PixelToComplexCoordinates(mouseX, mouseY)) < 1.0)
            {
                mobiusing = true;
                savedMobius = mobius;
            }
        }
    }
}

void H2CanvasDelegate::mouseMove(QMouseEvent *mouseEvent)
{
    if (mouseEvent->buttons() == Qt::LeftButton)
    {
        if (QApplication::keyboardModifiers() == Qt::ShiftModifier)
        {
            mouseShift(mouseEvent->x(), mouseEvent->y());
        }
        else if (mobiusing)
        {
            Complex mouseNew(PixelToComplexCoordinates(mouseEvent->x(), mouseEvent->y()));
            if (norm(mouseNew)< 1.0)
            {
                H2Isometry mobiusChange;
                Complex mouseOld(PixelToComplexCoordinates(this->mouseX,this->mouseY));

                mobiusChange.setByMappingPointInDiskModelNormalized(mouseOld, mouseNew);
                mobius = mobiusChange*savedMobius;
            }
        }
        enableRedrawBuffer();
    }
}

void H2CanvasDelegate::mouseRelease(QMouseEvent *)
{
    mobiusing = false;
}

void H2CanvasDelegate::enter()
{
    mobiusing = false;
}

void H2CanvasDelegate::leave()
{
    mobiusing = false;
}
