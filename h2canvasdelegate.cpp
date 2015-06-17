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
#include "h2canvasdelegate.h"
#include "actionhandler.h"

H2CanvasDelegate::H2CanvasDelegate(int sizeX, int sizeY, ActionHandler *handler) : CanvasDelegate(sizeX, sizeY, handler)
{
    //std::cout << "Entering H2CanvasDelegate::CanvasDelegate" << std::endl;
    delegateType = H2DELEGATE;
    mobius.setIdentity();
    showTranslatesAroundAllVertices = true;
    drawCircle(0, 1);
    resetHighlighted();

    //std::cout << "Leaving H2CanvasDelegate::CanvasDelegate" << std::endl;
}

H2Point H2CanvasDelegate::pixelToH2coordinate(int x, int y) const
{
    H2Point point;
    point.setDiskCoordinate(PixelToComplexCoordinates(x,y));
    return mobius.inverse()*point;
}

void H2CanvasDelegate::drawH2Point(const H2Point &p, const QColor &color, int width, bool back)
{
    Complex z = (mobius*p).getDiskCoordinate();
    drawPoint(z, color, width, back);
    return;
}

void H2CanvasDelegate::drawComplexPointInDiskModel(const Complex &z, const QColor &color, int width, bool back)
{
    H2Point P;
    P.setDiskCoordinate(z);
    drawH2Point(P, color, width, back);
    return;
}

void H2CanvasDelegate::drawH2Geodesic(const H2Geodesic &L, const QColor &color, int width, bool back)
{
    H2Geodesic geodesic =  mobius*L;
    if (geodesic.isCircleInDiskModel())
    {
        Circle C;
        double angle1, angle2;
        geodesic.getCircleAndAnglesInDiskModel(C, angle1, angle2);
        Complex endpoint1, endpoint2;
        geodesic.getEndpointsInDiskModel(endpoint1, endpoint2);
        drawSmallerArc(C, angle1, angle2, endpoint1, endpoint2, color, width, back);
    }
    else
    {
        Complex z1, z2;
        geodesic.getEndpointsInDiskModel(z1, z2);
        drawSegment(z1, z2, color, width, back);
    }
}

void H2CanvasDelegate::drawH2GeodesicArc(const H2GeodesicArc &L, const QColor &color, int width, bool back)
{
    ++nbArcs;
    H2GeodesicArc arc =  mobius*L;
    if(!arc.isLineSegmentInDiskModel())
    {
        Circle C;
        double angle1, angle2;
        arc.getCircleAndAnglesInDiskModel(C, angle1, angle2);
        Complex endpoint1, endpoint2;
        arc.getEndpointsInDiskModel(endpoint1, endpoint2);
        drawSmallerArc(C, angle1, angle2, endpoint1, endpoint2, color, width, back);
    }
    else
    {
        ++nbStraightArcs;
        Complex z1, z2;
        arc.getEndpointsInDiskModel(z1, z2);
        drawSegment(z1, z2, color, width, back);
    }
}

void H2CanvasDelegate::drawH2Triangle(const H2Triangle &triangle, const QColor &color, int width, bool back)
{
    std::vector<H2GeodesicArc> sides = triangle.getSides();
    for (const auto &side : sides)
    {
        drawH2GeodesicArc(side, color, width, back);
    }
}

void H2CanvasDelegate::redrawBuffer(bool back, bool top, const H2Isometry &mobius)
{
    clock_t start = clock();
    nbArcs = 0;
    nbStraightArcs = 0;
    nbAlmostStraightArcs = 0;

    this->mobius = mobius*this->mobius;

    if (back)
    {
        redrawBufferBack();
        subRedrawBufferBack();
    }
    if (top)
    {
        redrawBufferTop();
        subRedrawBufferTop();
    }

    enableRedrawBufferBack = false;
    enableRedrawBufferTop = false;

    //std::cout << "redraw buffer for delegate type " << delegateType << " :" << (clock() - start)*1.0/CLOCKS_PER_SEC << "s" << std::endl;
    //std::cout << "nbArcs = " << nbArcs << ", nbStraightArcs = " << nbStraightArcs << ", nbAlmostStraightArcs = " << nbAlmostStraightArcs << std::endl;
}

void H2CanvasDelegate::redrawBufferBack()
{
    imageBack->fill("white");

    subRedrawBufferBack();

    if (buffer.isMeshEmpty != buffer.isFunctionEmpty)
    {
        redrawMeshOrFunction();
    }
    else if (!buffer.isMeshEmpty)
    {
        throw(QString("Error in H2CanvasDelegate::redrawBufferBack(): mesh and function are both nonempty?"));
    }

    if (!buffer.isRhoEmpty)
    {
        for (const auto & geodesic : buffer.rhoAxes)
        {
            drawH2Geodesic(geodesic, buffer.rhoColor, 2);
        }
    }

    drawCircle(0, 1);
}

void H2CanvasDelegate::redrawMeshOrFunction()
{
    if (buffer.isFunctionEmpty != buffer.isMeshEmpty)
    {
        if (showTranslatesAroundAllVertices)
        {
            for (const auto & meshArc : buffer.meshOrFunctionArcsTranslatesAroundVertices)
            {
                drawH2GeodesicArc(meshArc, "grey", 1);
            }
        }
        if (showTranslatesAroundVertex)
        {
            for (const auto & meshArc : buffer.meshOrFunctionArcsTranslatesAroundVertex)
            {
                drawH2GeodesicArc(meshArc, "grey", 1);
            }
        }

        for (const auto & side : buffer.meshOrFunctionSidesTranslatesAroundVertices)
        {
            drawH2GeodesicArc(side, "black", 1);
        }

        for (const auto & meshArc : buffer.meshOrFunctionArcs)
        {
            drawH2GeodesicArc(meshArc, buffer.meshOrFunctionColor, 1);
        }

        for (const auto & side : buffer.meshOrFunctionSides)
        {
            drawH2GeodesicArc(side, buffer.meshOrFunctionColor, 2);
        }
    }
    else
    {
        throw(QString("Error in H2CanvasDelegate::redrawMeshOrFunction: mesh and function are both empty or nonempty?"));
    }
}

void H2CanvasDelegate::redrawBufferTop()
{
    imageTop->fill("white");
    imageTop->fill(qRgba(0, 0, 0, 0));
    if (isTriangleHighlighted)
    {
        drawH2Triangle(buffer.triangleHighlighted, "red", 2, false);
    }

    if (arePointsHighlightedRed)
    {
        for (const auto &p : buffer.pointsHighlightedRed)
        {
            drawH2Point(p, "red", 8, false);
        }
    }

    if (arePointsHighlightedGreen)
    {
        for (const auto &p : buffer.pointsHighlightedGreen)
        {
            drawH2Point(p, "green", 5, false);
        }
    }
    if (arePointsHighlightedBlue)
    {
        for (const auto &p : buffer.pointsHighlightedBlue)
        {
            drawH2Point(p, "blue", 5, false);
        }
    }
    subRedrawBufferTop();
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
}


void H2CanvasDelegate::setIsFunctionEmpty(bool isFunctionEmpty)
{
    buffer.setIsFunctionEmpty(isFunctionEmpty);
}

void H2CanvasDelegate::setIsMeshEmpty(bool isMeshEmpty)
{
    buffer.setIsMeshEmpty(isMeshEmpty);
}

void H2CanvasDelegate::setIsRhoEmpty(bool isRhoEmpty)
{
    buffer.setIsRhoEmpty(isRhoEmpty);
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
        }
        enableRedrawBuffer();
    }

    subMouseMove(mouseEvent);
}

void H2CanvasDelegate::leave()
{
    resetHighlighted();
}

void H2CanvasDelegate::resetHighlighted()
{
    isTriangleHighlighted = false;
    arePointsHighlightedBlue = false;
    arePointsHighlightedGreen = false;
    arePointsHighlightedRed = false;
}

void H2CanvasDelegate::getMeshIndexHighlighted(bool &highlighted, int &meshIndexHighted) const
{
    highlighted = arePointsHighlightedRed;
    meshIndexHighted = this->meshIndexHighlighted;
}

void H2CanvasDelegate::getMeshTriangleIndicesHighlighted(bool &highlighted, int &index1, int &index2, int &index3) const
{
    highlighted = isTriangleHighlighted;
    index1 = triangleMeshIndex1;
    index2 = triangleMeshIndex2;
    index3 = triangleMeshIndex3;
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
        break;

    case Qt::Key_Right :
        xMinSave = xMin;
        yMaxSave = yMax;
        shift(20, 0);
        break;

    case Qt::Key_Up :
        xMinSave = xMin;
        yMaxSave = yMax;
        shift(0, 20);
        break;

    case Qt::Key_Down :
        xMinSave = xMin;
        yMaxSave = yMax;
        shift(0, -20);
        break;

    case Qt::Key_Plus :
        zoom(2);
        break;

    case Qt::Key_Minus :
        zoom(0.5);
        break;
    }
    subKeyPress(keyEvent);
    enableRedrawBuffer();
}



H2CanvasDelegateDomain::H2CanvasDelegateDomain(int sizeX, int sizeY, ActionHandler * handler) : H2CanvasDelegate(sizeX, sizeY, handler)
{
    delegateType = H2DELEGATEDOMAIN;
    setShowTranslates(false, false);
}

H2CanvasDelegateTarget::H2CanvasDelegateTarget(int sizeX, int sizeY, ActionHandler *handler) : H2CanvasDelegate(sizeX, sizeY, handler)
{
    delegateType = H2DELEGATETARGET;
    setShowTranslates(false, false);
}

void H2CanvasDelegateDomain::decideHighlightingMeshPoints(bool highlighted, bool &update, int meshIndexHighlighted)
{
    if (((highlighted==false) && (arePointsHighlightedRed==false)) || ((highlighted == arePointsHighlightedRed) && (meshIndexHighlighted == this->meshIndexHighlighted)))
    {
        update = false;
    }
    else
    {
        if (highlighted)
        {
            this->meshIndexHighlighted = meshIndexHighlighted;

            arePointsHighlightedRed = true;
            buffer.pointsHighlightedRed = {buffer.mesh->getH2Point(meshIndexHighlighted)};

            arePointsHighlightedBlue = true;
            buffer.pointsHighlightedBlue = buffer.mesh->getH2Neighbors(meshIndexHighlighted);

            if (!buffer.mesh->isInteriorPoint(meshIndexHighlighted))
            {
                arePointsHighlightedGreen = true;
                buffer.pointsHighlightedGreen = buffer.mesh->getKickedH2Neighbors(meshIndexHighlighted);
                buffer.pointsHighlightedRed = buffer.mesh->getPartnerPoints(meshIndexHighlighted);
            }
            else
            {
                arePointsHighlightedGreen = false;
            }
        }
        else
        {
            arePointsHighlightedBlue = false;
            arePointsHighlightedGreen = false;
            arePointsHighlightedRed = false;
        }
        update = true;
    }
}

void H2CanvasDelegateDomain::decideHighlightingTriangle(bool highlighted, bool &update, int triangleMeshIndex1, int triangleMeshIndex2, int triangleMeshIndex3)
{
    if (((highlighted==false) && (isTriangleHighlighted==false)) || ((highlighted==isTriangleHighlighted) && (triangleMeshIndex1 == this->triangleMeshIndex1) &&
                                                                     (triangleMeshIndex2 == this->triangleMeshIndex2) && (triangleMeshIndex3 == this->triangleMeshIndex3)))
    {
        update = false;
    }
    else
    {
        if (highlighted)
        {
            isTriangleHighlighted = true;
            this->triangleMeshIndex1 = triangleMeshIndex1;
            this->triangleMeshIndex2 = triangleMeshIndex2;
            this->triangleMeshIndex3 = triangleMeshIndex3;
            buffer.triangleHighlighted = buffer.mesh->getH2Triangle(triangleMeshIndex1, triangleMeshIndex2, triangleMeshIndex3);
        }
        else
        {
            isTriangleHighlighted = false;
            arePointsHighlightedRed = false;
            arePointsHighlightedGreen = false;
            arePointsHighlightedBlue = false;
        }
        update = true;
    }
}

void H2CanvasDelegateDomain::decideHighlighting(const H2Point &pointUnderMouse)
{
    bool update1 = false, update2 = false;
    if (!buffer.isMeshEmpty)
    {
        int index1, index2, index3;
        H2Triangle triangle;
        if (buffer.mesh->triangleContaining(pointUnderMouse, triangle, index1, index2, index3))
        {
            decideHighlightingTriangle(true, update1, index1, index2, index3);
            int vertexIndex;
            double detectionRadiusSquared = detectionRadius/scaleX;
            detectionRadiusSquared *= detectionRadiusSquared;
            if ((mobius*triangle).isVertexCloseInDiskModel(mobius*pointUnderMouse, detectionRadiusSquared, vertexIndex))
            {
                decideHighlightingMeshPoints(true, update2, triangleMeshIndex1*(vertexIndex == 0) + triangleMeshIndex2*(vertexIndex == 1) + triangleMeshIndex3*(vertexIndex == 2));
            }
            else
            {
                decideHighlightingMeshPoints(false, update2);
            }
        }
        else
        {
            decideHighlightingTriangle(false, update1);
        }
        if (update1 || update2)
        {
            handler->processMessage(HIGHLIGHTED_LEFT);
        }
    }
}

void H2CanvasDelegateDomain::subMouseMove(QMouseEvent *mouseEvent)
{
    decideHighlighting(pixelToH2coordinate(mouseEvent->x(), mouseEvent->y()));
}

void H2CanvasDelegateDomain::subRedrawBufferBack()
{
}

void H2CanvasDelegateDomain::subRedrawBufferTop()
{
}

void H2CanvasDelegateTarget::decideHighlighting(const H2Point &pointUnderMouse)
{
    bool update1 = false, update2 = false;
    if (!buffer.isFunctionEmpty)
    {
        int index1, index2, index3;
        H2Triangle triangle;
        if (buffer.function->triangleContaining(pointUnderMouse, triangle, index1, index2, index3))
        {
            decideHighlightingTriangle(true, update1, index1, index2, index3);
            int vertexIndex;
            double detectionRadiusSquared = detectionRadius/scaleX;
            detectionRadiusSquared *= detectionRadiusSquared;
            if ((mobius*triangle).isVertexCloseInDiskModel(mobius*pointUnderMouse, detectionRadiusSquared, vertexIndex))
            {
                decideHighlightingMeshPoints(true, update2, triangleMeshIndex1*(vertexIndex == 0) + triangleMeshIndex2*(vertexIndex == 1) + triangleMeshIndex3*(vertexIndex == 2));
            }
            else
            {
                decideHighlightingMeshPoints(false, update2);
            }
        }
        else
        {
            decideHighlightingTriangle(false, update1);
        }
        if (update1 || update2)
        {
            handler->processMessage(HIGHLIGHTED_RIGHT);
        }
    }
}

void H2CanvasDelegateTarget::decideHighlightingMeshPoints(bool highlighted, bool &update, int meshIndexHighlighted)
{
    if (((highlighted==false) && (arePointsHighlightedRed==false)) || ((highlighted == arePointsHighlightedRed) && (meshIndexHighlighted == this->meshIndexHighlighted)))
    {
        update = false;
    }
    else
    {
        if (highlighted)
        {
            this->meshIndexHighlighted = meshIndexHighlighted;

            arePointsHighlightedRed = true;
            buffer.pointsHighlightedRed = {buffer.function->getH2Point(meshIndexHighlighted)};

            arePointsHighlightedBlue = true;
            buffer.pointsHighlightedBlue = buffer.function->getH2Neighbors(meshIndexHighlighted);

            if (!buffer.function->isInteriorPoint(meshIndexHighlighted))
            {
                arePointsHighlightedGreen = true;
                buffer.pointsHighlightedGreen = buffer.function->getKickedH2Neighbors(meshIndexHighlighted);
                buffer.pointsHighlightedRed = buffer.function->getPartnerPoints(meshIndexHighlighted);
            }
            else
            {
                arePointsHighlightedGreen = false;
            }
        }
        else
        {
            arePointsHighlightedBlue = false;
            arePointsHighlightedGreen = false;
            arePointsHighlightedRed = false;
        }
        update = true;
    }
}

void H2CanvasDelegateTarget::decideHighlightingTriangle(bool highlighted, bool &update, int triangleMeshIndex1, int triangleMeshIndex2, int triangleMeshIndex3)
{

    if (((highlighted==false) && (isTriangleHighlighted==false)) || ((highlighted==isTriangleHighlighted) && (triangleMeshIndex1 == this->triangleMeshIndex1) &&
                                                                     (triangleMeshIndex2 == this->triangleMeshIndex2) && (triangleMeshIndex3 == this->triangleMeshIndex3)))
    {
        update = false;
    }
    else
    {
        if (highlighted)
        {
            isTriangleHighlighted = true;
            this->triangleMeshIndex1 = triangleMeshIndex1;
            this->triangleMeshIndex2 = triangleMeshIndex2;
            this->triangleMeshIndex3 = triangleMeshIndex3;
            buffer.triangleHighlighted = buffer.function->getH2Triangle(triangleMeshIndex1, triangleMeshIndex2, triangleMeshIndex3);
        }
        else
        {
            isTriangleHighlighted = false;
            arePointsHighlightedRed = false;
            arePointsHighlightedGreen = false;
            arePointsHighlightedBlue = false;
        }
        update = true;
    }
}

void H2CanvasDelegateTarget::subRedrawBufferBack()
{
}

void H2CanvasDelegateTarget::subRedrawBufferTop()
{
}

void H2CanvasDelegateTarget::subMouseMove(QMouseEvent *mouseEvent)
{
    decideHighlighting(pixelToH2coordinate(mouseEvent->x(), mouseEvent->y()));
}

void H2CanvasDelegateTarget::subKeyPress(QKeyEvent *keyEvent)
{
    switch(keyEvent->key())
    {
    case Qt::Key_Space :
        break;
    }
}

void H2CanvasDelegate::refreshTranslates()
{
    refreshTranslates(showTranslatesAroundVertex, showTranslatesAroundAllVertices);
}

void H2CanvasDelegate::refreshTranslates(bool aroundVertex, bool aroundVertices)
{
    buffer.refreshMeshOrFunctionTranslates(aroundVertex, aroundVertices);
}

void H2CanvasDelegateDomain::refreshMesh()
{
    if (buffer.isMeshEmpty)
    {
        throw(QString("Error in H2CanvasDelegateDomain::refreshMesh: no mesh in buffer?"));
    }
    buffer.refreshMesh();
}

void H2CanvasDelegateTarget::refreshFunction()
{
    if (buffer.isFunctionEmpty)
    {
        throw(QString("Error in H2CanvasDelegateTarget::refreshFunction(): no mesh function in buffer?"));
    }
    buffer.refreshFunction();
}

void H2CanvasDelegate::setShowTranslates(bool showTranslatesAroundVertex, bool showTranslatesAroundAllVertices)
{
    this->showTranslatesAroundVertex = showTranslatesAroundVertex;
    this->showTranslatesAroundAllVertices = showTranslatesAroundAllVertices;
}

void H2CanvasDelegate::getShowTranslates(bool &aroundVertexOut, bool &aroundVerticesOut)
{
    aroundVertexOut = showTranslatesAroundVertex;
    aroundVerticesOut = showTranslatesAroundAllVertices;
}
