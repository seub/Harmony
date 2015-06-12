#include "h2canvasdelegate.h"

#include <QPen>
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

H2CanvasDelegate::H2CanvasDelegate(int sizeX, int sizeY) : CanvasDelegate(sizeX, sizeY)
{
    //std::cout << "Entering H2CanvasDelegate::CanvasDelegate" << std::endl;
    delegateType = H2DELEGATE;
    mobius.setIdentity();
    drawCircle(0, 1);
    isTriangleHighlighted = false;
    isPointHighlighted = false;
    arePointsHighlighted = false;

    //std::cout << "Leaving H2CanvasDelegate::CanvasDelegate" << std::endl;
}

void H2CanvasDelegate::drawH2Point(const H2Point &p, const QColor &color, int width)
{
    Complex z = (mobius*p).getDiskCoordinate();
    drawPoint(z, color, width);
    return;
}

void H2CanvasDelegate::drawComplexPointInDiskModel(const Complex &z, const QColor &color, int width)
{
    H2Point P;
    P.setDiskCoordinate(z);
    drawH2Point(P, color, width);
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
        Complex endpoint1, endpoint2;
        tempGeodesic.getEndpointsInDiskModel(endpoint1, endpoint2);
        drawSmallerArc(C, angle1, angle2, endpoint1, endpoint2, color, width);
    }
    else
    {
        Complex z1, z2;
        tempGeodesic.getEndpointsInDiskModel(z1, z2);
        drawSegment(z1, z2, color, width);
    }
    return;
}

void H2CanvasDelegate::drawH2GeodesicArc(const H2GeodesicArc &L, const QColor &color, int width)
{
    H2GeodesicArc tempGeodesic =  mobius*L;
    if(!tempGeodesic.isLineSegmentInDiskModel())
    {
        Circle C;
        double angle1, angle2;
        tempGeodesic.getCircleAndAnglesInDiskModel(C, angle1, angle2);
        Complex endpoint1, endpoint2;
        tempGeodesic.getEndpointsInDiskModel(endpoint1, endpoint2);
        drawSmallerArc(C, angle1, angle2, endpoint1, endpoint2, color, width);
    }
    else
    {
        Complex z1, z2;
        tempGeodesic.getEndpointsInDiskModel(z1, z2);
        drawSegment(z1, z2, color, width);
    }
    return;
}


void H2CanvasDelegate::redrawBuffer(const H2Isometry &mobius)
{
    image->fill("white");
    drawCircle(0, 1);

    this->mobius = mobius*this->mobius;


    int i;

    int nbGeodesicArcs = buffer.geodesicArcs.size();
    for (i = 0; i < nbGeodesicArcs; i++)
    {
        drawH2GeodesicArc(buffer.geodesicArcs[i], buffer.geodesicArcsColors[i], buffer.geodesicArcsWidths[i]);
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

    if (isTriangleHighlighted)
    {
        H2GeodesicArc L;
        H2Point a, b, c;
        buffer.triangleHighlighted.getPoints(a, b, c);
        L.setPoints(a, b);
        drawH2GeodesicArc(L, "red", 2);
        L.setPoints(a, c);
        drawH2GeodesicArc(L, "red", 2);
        L.setPoints(b, c);
        drawH2GeodesicArc(L, "red", 2);
    }

    if (isPointHighlighted)
    {
        drawH2Point(buffer.pointHighlighted, "red", 8);
    }
    if (arePointsHighlighted)
    {
        for (const auto &p : buffer.pointsHighlightedGreen)
        {
            drawH2Point(p, "green", 6);
        }
        for (const auto &p : buffer.pointsHighlightedBlue)
        {
            drawH2Point(p, "blue", 6);
        }
    }

    subRedrawBuffer();
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
            savedMobius = mobius;
        }
    }

    //std::cout << "Point clicked: " << PixelToComplexCoordinates(mouseX, mouseY) << std::endl;
    redrawBuffer();
}

void H2CanvasDelegate::mouseMove(QMouseEvent *mouseEvent)
{
    //std::cout << "Entering H2CanvasDelegate::mouseMove" << std::endl;

    if (mouseEvent->buttons() == Qt::LeftButton)
    {
        if (QApplication::keyboardModifiers() == Qt::ShiftModifier)
        {
            mouseShift(mouseEvent->x(), mouseEvent->y());
        }
        else
        {
            H2Isometry mobiusChange;
            Complex mouseOld(PixelToComplexCoordinates(this->mouseX,this->mouseY));
            Complex mouseNew(PixelToComplexCoordinates(mouseEvent->x(), mouseEvent->y()));

            mobiusChange.setByMappingPointInDiskModelNormalized(mouseOld, mouseNew);
            mobius = mobiusChange*savedMobius;
            redrawBuffer();
        }
    }

    subMouseMove(mouseEvent);
}

void H2CanvasDelegate::keyPress(QKeyEvent *keyEvent)
{
    //std::cout << "Entering H2CanvasDelegate::keyPress" << std::endl;

    switch(keyEvent->key())
    {
    /*case Qt::Key_Control :
    delegate->ctrl_pressed();
    break;*/

    case Qt::Key_Left :
        xMinSave = xMin;
        yMaxSave = yMax;
        shift(-20, 0);
        redrawBuffer();
        break;

    case Qt::Key_Right :
        xMinSave = xMin;
        yMaxSave = yMax;
        shift(20, 0);
        redrawBuffer();
        break;

    case Qt::Key_Up :
        xMinSave = xMin;
        yMaxSave = yMax;
        shift(0, 20);
        redrawBuffer();
        break;

    case Qt::Key_Down :
        xMinSave = xMin;
        yMaxSave = yMax;
        shift(0, -20);
        redrawBuffer();
        break;

    case Qt::Key_Plus :
        zoom(2);
        redrawBuffer();
        break;

    case Qt::Key_Minus :
        zoom(0.5);
        redrawBuffer();
        break;
    }
    subKeyPress(keyEvent);
}



H2CanvasDelegateDomain::H2CanvasDelegateDomain(int sizeX, int sizeY) : H2CanvasDelegate(sizeX, sizeY)
{
    delegateType = H2DELEGATEDOMAIN;
}

H2CanvasDelegateTarget::H2CanvasDelegateTarget(int sizeX, int sizeY) : H2CanvasDelegate(sizeX, sizeY)
{
    delegateType = H2DELEGATETARGET;
}

void H2CanvasDelegateDomain::subMouseMove(QMouseEvent *mouseEvent)
{
    if (!buffer.isMeshEmpty)
    {
        H2Point point;
        point.setDiskCoordinate(PixelToComplexCoordinates(mouseEvent->x(), mouseEvent->y()));
        int meshIndex1, meshIndex2, meshIndex3, meshIndex;
        point = mobius.inverse()*point;
        if (buffer.mesh->triangleContaining(point, buffer.triangleHighlighted, meshIndex1, meshIndex2, meshIndex3))
        {
            isTriangleHighlighted = true;
            int vertexIndex;
            double detectionRadiusSquared = detectionRadius/scaleX;
            detectionRadiusSquared *= detectionRadiusSquared;
            if ((mobius*buffer.triangleHighlighted).isVertexCloseInDiskModel(mobius*point, detectionRadiusSquared, vertexIndex))
            {
                buffer.pointHighlighted = buffer.triangleHighlighted.getVertex(vertexIndex);
                isPointHighlighted = true;
                arePointsHighlighted = true;
                meshIndex = meshIndex1*(vertexIndex == 0) + meshIndex2*(vertexIndex == 1) + meshIndex3*(vertexIndex == 2);
                buffer.pointsHighlightedBlue = buffer.mesh->getH2Neighbors(meshIndex);
                buffer.pointsHighlightedGreen = buffer.mesh->getKickedH2Neighbors(meshIndex);
            }
            else
            {
                isPointHighlighted = false;
                arePointsHighlighted = false;
            }
        }
        else
        {
            isTriangleHighlighted = false;
            isPointHighlighted = false;
            arePointsHighlighted = false;
        }
    }
    else
    {
        std::cout << "Error in H2CanvasDelegateDomain::subMouseMove, no mesh in buffer?" << std::endl;
    }
}

void H2CanvasDelegateDomain::subRedrawBuffer()
{

}

void H2CanvasDelegateTarget::subRedrawBuffer()
{
    if (!buffer.isMeshFunctionEmpty)
    {
        for (const auto & arc : buffer.functionArcs)
        {
            drawH2GeodesicArc(arc, buffer.functionColor, buffer.functionWidth);
        }
        for (const auto & point : buffer.functionPoints)
        {
            drawH2Point(point);
        }
    }
}

void H2CanvasDelegateTarget::subMouseMove(QMouseEvent *)
{

}

void H2CanvasDelegateTarget::subKeyPress(QKeyEvent *keyEvent)
{
    switch(keyEvent->key())
    {
    case Qt::Key_Space :
        buffer.function->iterate(10);
        buffer.refreshFunction();
        redrawBuffer();

        break;
    }
}
