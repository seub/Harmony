#include "h2canvasdelegateliftedgraph.h"

#include <QMouseEvent>
#include <QPen>
#include <QPainter>

#include "h2canvasdelegate.h"
#include "canvasdelegate.h"
#include "liftedgraph.h"
#include "actionhandler.h"


H2CanvasDelegateLiftedGraph::H2CanvasDelegateLiftedGraph(uint sizeX, uint sizeY, bool leftCanvas, bool rightCanvas, ActionHandler *handler) :
    H2CanvasDelegate(sizeX, sizeY, leftCanvas, rightCanvas, handler)
{
    graphColor = "green";

    initializeColors(graphColor);

    setFilledTriangles(false);
    setShowTranslates(false, false);
}

void H2CanvasDelegateLiftedGraph::initializeColors(const QColor &graphColor)
{
    QColor graphTranslatesBase("lightgrey");
    QColor graphSidesTranslatesBase(90, 90, 90);
    int r0, g0, b0, r1, g1, b1;

    this->graphColor = graphColor;

    graphColor.getRgb(&r1, &g1, &b1);

    graphTranslatesBase.getRgb(&r0, &g0, &b0);
    graphTranslatesColor = QColor(g(r0, r1), g(g0, g1), g(b0, b1));

    graphSidesTranslatesBase.getRgb(&r0, &g0, &b0);
    graphSidesTranslatesColor = QColor(g(r0, r1), g(g0, g1), g(b0, b1));
}

void H2CanvasDelegateLiftedGraph::resetView()
{
    H2CanvasDelegate::resetView();
    resetHighlighted();
}

void H2CanvasDelegateLiftedGraph::enter()
{
    H2CanvasDelegate::enter();
}

void H2CanvasDelegateLiftedGraph::leave()
{
    H2CanvasDelegate::leave();
    resetHighlighted();
}

void H2CanvasDelegateLiftedGraph::mouseMove(QMouseEvent *mouseEvent)
{
    H2CanvasDelegate::mouseMove(mouseEvent);
    if (leftCanvas || rightCanvas)
    {
        decideHighlighting(pixelToH2coordinate(mouseEvent->x(), mouseEvent->y()));
    }
}

void H2CanvasDelegateLiftedGraph::setRhoPointer(GroupRepresentation<H2Isometry> *rho)
{
    this->rho = rho;
}

void H2CanvasDelegateLiftedGraph::setGraphPointer(LiftedGraphFunctionTriangulated<H2Point, H2Isometry> *graph)
{
    this->graph = graph;
}

void H2CanvasDelegateLiftedGraph::setIsGraphEmpty(bool isGraphEmpty)
{
    this->isGraphEmpty = isGraphEmpty;
    enableRedrawBuffer();
}

void H2CanvasDelegateLiftedGraph::setIsRhoEmpty(bool isRhoEmpty)
{
    this->isRhoEmpty = isRhoEmpty;
    enableRedrawBuffer();
}

void H2CanvasDelegateLiftedGraph::resetHighlighted()
{
    isTriangleHighlighted = false;
    arePointsHighlightedBlue = false;
    arePointsHighlightedGreen = false;
    arePointsHighlightedRed = false;
    enableRedrawBuffer(enableRedrawBufferBack, true);
}

void H2CanvasDelegateLiftedGraph::getGraphIndexHighlighted(bool &highlighted, uint &graphIndexHighted) const
{
    highlighted = arePointsHighlightedRed;
    graphIndexHighted = this->graphIndexHighlighted;
}

void H2CanvasDelegateLiftedGraph::getGraphTriangleIndicesHighlighted(bool &highlighted, uint &index1, uint &index2, uint &index3) const
{
    highlighted = isTriangleHighlighted;
    index1 = triangleGraphIndex1;
    index2 = triangleGraphIndex2;
    index3 = triangleGraphIndex3;
}



void H2CanvasDelegateLiftedGraph::redrawBack()
{
    H2CanvasDelegate::redrawBack();

    if (!isGraphEmpty)
    {
        redrawGraph();
    }

    if (!isRhoEmpty)
    {
        for (const auto & geodesic : rhoAxes)
        {
            drawH2Geodesic(geodesic, "blue", 2);
        }
    }
}

void H2CanvasDelegateLiftedGraph::redrawGraph()
{
    if (filledTriangles)
    {
        redrawFilledGraph();
    }
    else
    {
        redrawNonFilledGraph();
    }
}

void H2CanvasDelegateLiftedGraph::redrawNonFilledGraph()
{
    if (showTranslatesAroundVertices)
    {
        penBack->setColor(graphTranslatesColor);
        penBack->setWidth(1);
        painterBack->setPen(*penBack);
        resetPenBack = false;

        for (const auto & graphArcTranslate : graphArcsTranslatesAroundVertices)
        {
            drawH2GeodesicArc(graphArcTranslate);
        }

        resetPenBack = true;
    }

    if (showTranslatesAroundVertex)
    {
        penBack->setColor(graphTranslatesColor);
        penBack->setWidth(1);
        painterBack->setPen(*penBack);
        resetPenBack = false;

        for (const auto & graphArcTranslate : graphArcsTranslatesAroundVertex)
        {
            drawH2GeodesicArc(graphArcTranslate);
        }

        resetPenBack = true;
    }

    penBack->setColor(graphSidesTranslatesColor);
    penBack->setWidth(1);
    painterBack->setPen(*penBack);
    resetPenBack = false;

    if (rightCanvas)
    {
        for (const auto & sideTranslate : graphSidesTranslates)
        {
            drawH2GeodesicArc(sideTranslate);
        }
    }
    else
    {
        for (const auto & sideTranslate : graphLargeSidesTranslates)
        {
            drawH2GeodesicArc(sideTranslate);
        }
    }

    resetPenBack = true;

    penBack->setColor(graphColor);
    penBack->setWidth(1);
    painterBack->setPen(*penBack);
    resetPenBack = false;

    for (const auto & graphArc : graphArcs)
    {
        drawH2GeodesicArc(graphArc);
    }

    penBack->setColor(graphColor);
    penBack->setWidth(2);
    painterBack->setPen(*penBack);
    resetPenBack = false;

    if (!rightCanvas)
    {
        for (const auto & side : graphLargeSides)
        {
            drawH2GeodesicArc(side);
        }
    }
    else
    {
        for (const auto & side : graphSides)
        {
            drawH2GeodesicArc(side);
        }
    }

    resetPenBack = true;
}

void H2CanvasDelegateLiftedGraph::redrawFilledGraph()
{
    painterBack->setPen(Qt::NoPen);
    QBrush *brush = new QBrush;

    brush->setStyle(Qt::SolidPattern);

    int r0, g0, b0, r, g, b;

    if (showTranslatesAroundVertices)
    {
        if (rightCanvas)
        {
            graphTranslatesColor.getRgb(&r0, &g0, &b0);

            for (uint i=0; i!=graphTrianglesTranslatesAroundVertices.size(); ++i)
            {
                r = f(r0, graphTrianglesWeights[i % graphTriangles.size()]);
                g = f(g0, graphTrianglesWeights[i % graphTriangles.size()]);
                b = f(b0, graphTrianglesWeights[i % graphTriangles.size()]);
                brush->setColor(QColor(r, g, b));
                painterBack->setPen(QColor(r, g, b));
                painterBack->setBrush(*brush);
                drawStraightFilledH2Triangle(graphTrianglesTranslatesAroundVertices[i]);
            }
        }
        else
        {
            brush->setColor(graphTranslatesColor);
            painterBack->setPen(graphTranslatesColor);
            painterBack->setBrush(*brush);

            for (const auto & graphTriangleTranslate : graphTrianglesTranslatesAroundVertices)
            {
                drawStraightFilledH2Triangle(graphTriangleTranslate);
            }
        }
    }

    if (showTranslatesAroundVertex)
    {
        if (rightCanvas)
        {
            graphTranslatesColor.getRgb(&r0, &g0, &b0);

            for (uint i=0; i!=graphTrianglesTranslatesAroundVertex.size(); ++i)
            {
                r = f(r0, graphTrianglesWeights[i % graphTriangles.size()]);
                g = f(g0, graphTrianglesWeights[i % graphTriangles.size()]);
                b = f(b0, graphTrianglesWeights[i % graphTriangles.size()]);
                brush->setColor(QColor(r, g, b));
                painterBack->setPen(QColor(r, g, b));
                painterBack->setBrush(*brush);
                drawStraightFilledH2Triangle(graphTrianglesTranslatesAroundVertex[i]);
            }
        }
        else
        {
            brush->setColor(graphTranslatesColor);
            painterBack->setPen(graphTranslatesColor);
            painterBack->setBrush(*brush);

            for (const auto & graphTriangleTranslate : graphTrianglesTranslatesAroundVertex)
            {
                drawStraightFilledH2Triangle(graphTriangleTranslate);
            }
        }

    }

    if (rightCanvas)
    {
        graphColor.getRgb(&r0, &g0, &b0);

        for (uint i=0; i!=graphTriangles.size(); ++i)
        {
            r = f(r0, graphTrianglesWeights[i]);
            g = f(g0, graphTrianglesWeights[i]);
            b = f(b0, graphTrianglesWeights[i]);

            brush->setColor(QColor(r, g, b));
            painterBack->setPen(QColor(r, g, b));
            painterBack->setBrush(*brush);
            drawStraightFilledH2Triangle(graphTriangles[i]);
        }
    }
    else
    {
        brush->setColor(graphColor);
        painterBack->setPen(graphColor);
        painterBack->setBrush(*brush);

        for (const auto & graphTriangle : graphTriangles)
        {
            drawStraightFilledH2Triangle(graphTriangle);
        }
    }

    painterBack->setBrush(Qt::NoBrush);
    delete brush;


    penBack->setColor(graphSidesTranslatesColor);
    penBack->setWidth(1);
    painterBack->setPen(*penBack);
    resetPenBack = false;

    for (const auto & straightSideTranslate : graphSidesTranslates)
    {
        drawStraightH2GeodesicArc(straightSideTranslate);
    }

    penBack->setColor(graphColor);
    penBack->setWidth(1);
    painterBack->setPen(*penBack);
    resetPenBack = false;

    for (const auto & side : graphSides)
    {
        drawStraightH2GeodesicArc(side);
    }

    resetPenBack = true;
}


void H2CanvasDelegateLiftedGraph::redrawTop()
{
    H2CanvasDelegate::redrawTop();

    if (isTriangleHighlighted)
    {
        drawH2Triangle(triangleHighlighted, "red", 2, false);
    }

    if (arePointsHighlightedRed)
    {
        for (const auto &p : pointsHighlightedRed)
        {
            drawH2Point(p, "red", 8, false);
        }
    }

    if (arePointsHighlightedGreen)
    {
        for (const auto &p : pointsHighlightedGreen)
        {
            drawH2Point(p, "green", 5, false);
        }
    }
    if (arePointsHighlightedBlue)
    {
        for (const auto &p : pointsHighlightedBlue)
        {
            drawH2Point(p, "blue", 5, false);
        }
    }
}

void H2CanvasDelegateLiftedGraph::decideHighlightingGraphPoints(bool highlighted, bool &update, uint graphIndexHighlighted)
{
    if (((highlighted==false) && (arePointsHighlightedRed==false)) || ((highlighted == arePointsHighlightedRed) && (graphIndexHighlighted == this->graphIndexHighlighted)))
    {
        update = false;
    }
    else
    {
        if (highlighted)
        {
            this->graphIndexHighlighted = graphIndexHighlighted;

            arePointsHighlightedRed = true;
            pointsHighlightedRed = {graph->getValue(graphIndexHighlighted)};

            arePointsHighlightedBlue = true;
            pointsHighlightedBlue = graph->getNeighborsValues(graphIndexHighlighted);

            if (graph->isBoundaryPoint(graphIndexHighlighted))
            {
                arePointsHighlightedGreen = true;
                pointsHighlightedGreen = graph->getNeighborsValuesKicked(graphIndexHighlighted);
                pointsHighlightedRed = graph-> getPartnersValues(graphIndexHighlighted);
                pointsHighlightedRed.push_back(graph->getValue(graphIndexHighlighted));
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
        enableRedrawBuffer(enableRedrawBufferBack, true);
    }
}

void H2CanvasDelegateLiftedGraph::decideHighlightingTriangle(bool highlighted, bool &update, uint triangleGraphIndex1, uint triangleGraphIndex2, uint triangleGraphIndex3)
{
    if (((highlighted==false) && (isTriangleHighlighted==false)) || ((highlighted==isTriangleHighlighted) && (triangleGraphIndex1 == this->triangleGraphIndex1) &&
                                                                     (triangleGraphIndex2 == this->triangleGraphIndex2) && (triangleGraphIndex3 == this->triangleGraphIndex3)))
    {
        update = false;
    }
    else
    {
        if (highlighted)
        {
            isTriangleHighlighted = true;
            this->triangleGraphIndex1 = triangleGraphIndex1;
            this->triangleGraphIndex2 = triangleGraphIndex2;
            this->triangleGraphIndex3 = triangleGraphIndex3;
            triangleHighlighted = graph->getH2Triangle(triangleGraphIndex1, triangleGraphIndex2, triangleGraphIndex3);
        }
        else
        {
            isTriangleHighlighted = false;
            arePointsHighlightedRed = false;
            arePointsHighlightedGreen = false;
            arePointsHighlightedBlue = false;
        }
        update = true;
        enableRedrawBuffer(enableRedrawBufferBack, true);
    }
}

void H2CanvasDelegateLiftedGraph::decideHighlighting(const H2Point &pointUnderMouse)
{
    bool update1 = false, update2 = false;
    if (!isGraphEmpty)
    {
        uint index1, index2, index3;
        H2Triangle triangle;
        if (graph->triangleContaining(pointUnderMouse, triangle, index1, index2, index3))
        {
            decideHighlightingTriangle(true, update1, index1, index2, index3);
            uint vertexIndex;
            double detectionRadiusSquared = detectionRadius/scaleX;
            detectionRadiusSquared *= detectionRadiusSquared;
            if ((mobius*triangle).isVertexCloseInDiskModel(mobius*pointUnderMouse, detectionRadiusSquared, vertexIndex))
            {
                decideHighlightingGraphPoints(true, update2, triangleGraphIndex1*(vertexIndex == 0) + triangleGraphIndex2*(vertexIndex == 1) + triangleGraphIndex3*(vertexIndex == 2));
            }
            else
            {
                decideHighlightingGraphPoints(false, update2, 0);
            }
        }
        else
        {
            decideHighlightingTriangle(false, update1, 0, 0, 0);
        }
        if (update1 || update2)
        {
            enableRedrawBuffer(enableRedrawBufferBack, true);
            if (leftCanvas)
            {
                handler->processMessage(ActionHandlerMessage::HIGHLIGHTED_LEFT);
            }
            if (rightCanvas)
            {
                handler->processMessage(ActionHandlerMessage::HIGHLIGHTED_RIGHT);
            }
        }
    }
}


void H2CanvasDelegateLiftedGraph::setShowTranslates(bool showTranslatesAroundVertex, bool showTranslatesAroundAllVertices)
{
    this->showTranslatesAroundVertex = showTranslatesAroundVertex;
    this->showTranslatesAroundVertices = showTranslatesAroundAllVertices;
    enableRedrawBuffer();
}


const std::vector<double> & H2CanvasDelegateLiftedGraph::getGraphTrianglesReferenceAreas() const
{
    return graphTrianglesReferenceAreas;
}

void H2CanvasDelegateLiftedGraph::setGraphTrianglesReferenceAreas(const std::vector<double> &graphTrianglesReferenceAreas)
{
    this->graphTrianglesReferenceAreas = graphTrianglesReferenceAreas;
}


void H2CanvasDelegateLiftedGraph::setFilledTriangles(bool filledTriangles)
{
    this->filledTriangles = filledTriangles;
    enableRedrawBuffer();
}

void H2CanvasDelegateLiftedGraph::getShowTranslates(bool &aroundVertexOut, bool &aroundVerticesOut)
{
    aroundVertexOut = showTranslatesAroundVertex;
    aroundVerticesOut = showTranslatesAroundVertices;
}


void H2CanvasDelegateLiftedGraph::updateGraph(bool refreshSidesTranslates, bool refreshFullTranslates)
{
    if (!isGraphEmpty)
    {
        if (filledTriangles)
        {
            updateFilledGraph(refreshSidesTranslates, refreshFullTranslates);
        }
        else
        {
            updateNonFilledGraph(refreshSidesTranslates, refreshFullTranslates);
        }
    }
}

void H2CanvasDelegateLiftedGraph::updateNonFilledGraph(bool refreshSidesTranslates, bool refreshFullTranslates)
{    
    if (!rightCanvas)
    {
        graphLargeSides = H2Polygon(graph->getFirstVertexOrbit()).getSides();
    }

    graphSides = H2Polygon(graph->getBoundary()).getSides();


    std::vector< std::vector<H2Point> > triangles = graph->getTrianglesUp();
    graphArcs.clear();
    graphArcs.reserve(3*triangles.size());
    std::vector<H2GeodesicArc> sides;
    for (const auto & triangle : triangles)
    {
        sides = H2Triangle(triangle[0], triangle[1], triangle[2]).getSides();
        graphArcs.insert(graphArcs.end(), sides.begin(), sides.end());
    }

    if (refreshSidesTranslates)
    {
        std::vector<H2GeodesicArc> sideTranslates;

        if (!rightCanvas)
        {
            graphLargeSidesTranslates.clear();
            graphLargeSidesTranslates.reserve(translationsAroundVertices.size()*graphLargeSides.size());

            for (const auto & translation : translationsAroundVertices)
            {
                sideTranslates = translation*graphLargeSides;
                graphLargeSidesTranslates.insert(graphLargeSidesTranslates.end(), sideTranslates.begin(), sideTranslates.end());
            }
        }

        graphSidesTranslates.clear();
        graphSidesTranslates.reserve(translationsAroundVertices.size()*graphSides.size());

        for (const auto & translation : translationsAroundVertices)
        {
            sideTranslates = translation*graphSides;
            graphSidesTranslates.insert(graphSidesTranslates.end(), sideTranslates.begin(), sideTranslates.end());
        }
    }

    if (showTranslatesAroundVertex && refreshFullTranslates)
    {
        std::vector<H2GeodesicArc> arcsTranslates;
        graphArcsTranslatesAroundVertex.clear();
        graphArcsTranslatesAroundVertex.reserve(translationsAroundVertex.size()*graphArcs.size());

        for (const auto & translation : translationsAroundVertex)
        {
            arcsTranslates = translation*graphArcs;
            graphArcsTranslatesAroundVertex.insert(graphArcsTranslatesAroundVertex.end(), arcsTranslates.begin(), arcsTranslates.end());
        }
    }

    if (showTranslatesAroundVertices && refreshFullTranslates)
    {
        std::vector<H2GeodesicArc> arcsTranslates;
        graphArcsTranslatesAroundVertices.clear();
        graphArcsTranslatesAroundVertices.reserve(translationsAroundVertices.size()*graphArcs.size());

        for (const auto & translation : translationsAroundVertices)
        {
            arcsTranslates = translation*graphArcs;
            graphArcsTranslatesAroundVertices.insert(graphArcsTranslatesAroundVertices.end(), arcsTranslates.begin(), arcsTranslates.end());
        }
    }

    enableRedrawBuffer(true, true);
}


void H2CanvasDelegateLiftedGraph::updateFilledGraph(bool refreshSidesTranslates, bool refreshFullTranslates)
{
    graphSides = H2Polygon(graph->getBoundary()).getSides();
    graphTriangles = graph->getAllH2Triangles();


    if (leftCanvas)
    {
        graphTrianglesReferenceAreas.clear();
        graphTrianglesReferenceAreas.reserve(graphTriangles.size());

        for (const auto &triangle : graphTriangles)
        {
            graphTrianglesReferenceAreas.push_back(triangle.area());
        }

        handler->processMessage(ActionHandlerMessage::TRIANGLES_REFERENCE_AREAS);
    }


    if (rightCanvas)
    {
        graphTrianglesWeights.clear();
        graphTrianglesWeights.reserve(graphTriangles.size());

        for (uint i=0; i<graphTriangles.size(); ++i)
        {
            graphTrianglesWeights.push_back(graphTriangles[i].area()/graphTrianglesReferenceAreas[i]);
        }
    }

    if (refreshSidesTranslates)
    {
        std::vector<H2GeodesicArc> straightSidesTranslates;
        graphSidesTranslates.clear();
        graphSidesTranslates.reserve(translationsAroundVertices.size()*graphSides.size());

        for (const auto & translation : translationsAroundVertices)
        {
            straightSidesTranslates = translation*graphSides;
            graphSidesTranslates.insert(graphSidesTranslates.end(), straightSidesTranslates.begin(), straightSidesTranslates.end());
        }
    }

    if (showTranslatesAroundVertex && refreshFullTranslates)
    {
        std::vector<H2Triangle> triangleTranslates;
        graphTrianglesTranslatesAroundVertex.clear();
        graphTrianglesTranslatesAroundVertex.reserve(translationsAroundVertex.size()*graphTriangles.size());

        for (const auto & translation : translationsAroundVertex)
        {
            triangleTranslates = translation*graphTriangles;
            graphTrianglesTranslatesAroundVertex.insert(graphTrianglesTranslatesAroundVertex.end(), triangleTranslates.begin(), triangleTranslates.end());
        }
    }

    if (showTranslatesAroundVertices && refreshFullTranslates)
    {
        std::vector<H2Triangle> triangleTranslates;
        graphTrianglesTranslatesAroundVertices.clear();
        graphTrianglesTranslatesAroundVertices.reserve(translationsAroundVertices.size()*graphTriangles.size());

        for (const auto & translation : translationsAroundVertices)
        {
            triangleTranslates = translation*graphTriangles;
            graphTrianglesTranslatesAroundVertices.insert(graphTrianglesTranslatesAroundVertices.end(), triangleTranslates.begin(), triangleTranslates.end());
        }
    }

    enableRedrawBuffer(true, true);
}

void H2CanvasDelegateLiftedGraph::refreshRho()
{
    if (isRhoEmpty)
    {
        throw(QString("Error in H2CanvasDelegateLiftedGraph::refreshRho::refreshRho: rho empty?"));
    }

    H2Geodesic axis;
    std::vector<H2Isometry> generatorImages = rho->getGeneratorImages();
    rhoAxes.clear();
    for (const auto &generatorImage : generatorImages)
    {
        if (generatorImage.axis(axis))
        {
            rhoAxes.push_back(axis);
        }
        else
        {
            qDebug() << "Warning in H2CanvasDelegateLiftedGraph::refreshRho(): some generators are not hyperbolic";
        }
    }

    translationsAroundVertex = rho->getSidePairingsNormalizedAroundVertex();
    translationsAroundVertices = rho->getSidePairingsNormalizedAroundVertices();
}

int H2CanvasDelegateLiftedGraph::f(int X0, const double &x)
{
    double x0 = X0/255.0;
    double y = x<1 ? x*x0 : (x0+x-1)/x;
    return Tools::intRound(255.0*y);
}

int H2CanvasDelegateLiftedGraph::g(int x0, int x1)
{
    return Tools::intRound((4.0*x0 + 1.0*x1)/5.0);
}


