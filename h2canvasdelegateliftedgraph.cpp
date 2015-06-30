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
    setIsGraphEmpty(true);
}

void H2CanvasDelegateLiftedGraph::initializeColors(const QColor &graphColor)
{
    this->graphColor = graphColor;

    int h, s, l;
    graphColor.getHsl(&h, &s, &l);

    highlightColor.setHsl((h+180)%360, s, l);
    graphColor2.setHsl((h+90)%360, s, l);
    graphSidesTranslatesColor.setHsl(h, weightedSum(s, 0, 0.75), l);
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
}

void H2CanvasDelegateLiftedGraph::setIsRhoEmpty(bool isRhoEmpty)
{
    this->isRhoEmpty = isRhoEmpty;
}

void H2CanvasDelegateLiftedGraph::resetHighlighted()
{
    isTriangleHighlighted = false;
    enableRedrawBuffer(enableRedrawBufferBack, true);
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
    int h, s, l;
    graphColor.getHsl(&h, &s, &l);
    QColor graphTranslatesColor(QColor::fromHsl(h, weightedSum(s, 0, 0.5), l));

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
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);

    if (showTranslatesAroundVertices)
    {
        for (uint i=0; i!=graphTrianglesTranslatesAroundVertices.size(); ++i)
        {
            painterBack->setPen(graphTrianglesTranslatesColors[i % graphTriangles.size()]);
            brush.setColor(graphTrianglesTranslatesColors[i % graphTriangles.size()]);
            painterBack->setBrush(brush);
            drawStraightFilledH2Triangle(graphTrianglesTranslatesAroundVertices[i]);
        }
    }

    if (showTranslatesAroundVertex)
    {
        for (uint i=0; i!=graphTrianglesTranslatesAroundVertex.size(); ++i)
        {
            painterBack->setPen(graphTrianglesTranslatesColors[i % graphTriangles.size()]);
            brush.setColor(graphTrianglesTranslatesColors[i % graphTriangles.size()]);
            painterBack->setBrush(brush);
            drawStraightFilledH2Triangle(graphTrianglesTranslatesAroundVertex[i]);
        }

    }

    for (uint i=0; i!=graphTriangles.size(); ++i)
    {
        brush.setColor(graphTrianglesColors[i]);
        painterBack->setPen(graphTrianglesColors[i]);
        painterBack->setBrush(brush);
        drawStraightFilledH2Triangle(graphTriangles[i]);
    }

    painterBack->setBrush(Qt::NoBrush);
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
        if (filledTriangles)
        {
            highlightStraightH2Triangle(triangleHighlighted, highlightColor);
        }
        else
        {
            drawH2Triangle(triangleHighlighted, highlightColor, 2, false);
        }
    }

    enableRedrawBufferTop = true;
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
        }
        update = true;
        enableRedrawBuffer(enableRedrawBufferBack, true);
    }
}

void H2CanvasDelegateLiftedGraph::decideHighlighting(const H2Point &pointUnderMouse)
{
    bool update = false;
    if (!isGraphEmpty)
    {
        uint index1, index2, index3;
        H2Triangle triangle;
        if (graph->triangleContaining(pointUnderMouse, triangle, index1, index2, index3))
        {
            decideHighlightingTriangle(true, update, index1, index2, index3);
        }
        else
        {
            decideHighlightingTriangle(false, update, 0, 0, 0);
        }
        if (update)
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
}

void H2CanvasDelegateLiftedGraph::setFilledTriangles(bool filledTriangles)
{
    this->filledTriangles = filledTriangles;
}

void H2CanvasDelegateLiftedGraph::setTwoColor(bool twoColors)
{
    this->twoColors = twoColors;
}

void H2CanvasDelegateLiftedGraph::setGraphColor(const QColor &color)
{
    graphColor = color;
    initializeColors(color);
}

void H2CanvasDelegateLiftedGraph::updateGraph(bool refreshSidesTranslates)
{
    if (!isGraphEmpty)
    {
        if (filledTriangles)
        {
            updateFilledGraph(refreshSidesTranslates);
        }
        else
        {
            updateNonFilledGraph(refreshSidesTranslates);
        }
    }
}

void H2CanvasDelegateLiftedGraph::updateNonFilledGraph(bool refreshSidesTranslates)
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

    if (showTranslatesAroundVertex)
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

    if (showTranslatesAroundVertices)
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
}


void H2CanvasDelegateLiftedGraph::updateFilledGraph(bool refreshSidesTranslates)
{
    graphSides = H2Polygon(graph->getBoundary()).getSides();
    graphTriangles = graph->getAllH2Triangles();

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

    if (showTranslatesAroundVertex)
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

    if (showTranslatesAroundVertices)
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

    updateTrianglesWeights();
    updateTrianglesColors();
    if (showTranslatesAroundVertex || showTranslatesAroundVertices)
    {
        updateTrianglesTranslatesColors();
    }
}

void H2CanvasDelegateLiftedGraph::updateTrianglesWeights()
{
    if (leftCanvas)
    {
        graphTrianglesReferenceAreas->clear();
        graphTrianglesReferenceAreas->reserve(graphTriangles.size());

        for (const auto &triangle : graphTriangles)
        {
            graphTrianglesReferenceAreas->push_back(triangle.area());
        }
    }
}

void H2CanvasDelegateLiftedGraph::updateTrianglesColors()
{
    if (leftCanvas)
    {
        if (twoColors)
        {
            graphTrianglesReferenceColors->clear();
            graphTrianglesReferenceColors->reserve(graphTriangles.size());

            H2Point firstTriangleVertex;
            std::vector<double> distancesToVertex, distancesToVertices;

            int r0, g0, b0, r1, g1, b1;
            graphColor.getRgb(&r0, &g0, &b0);
            graphColor2.getRgb(&r1, &g1, &b1);

            std::vector<H2Point> vertices = graph->getFirstVertexOrbit();

            for (const auto &triangle : graphTriangles)
            {
                firstTriangleVertex = triangle.getVertex(0);
                distancesToVertices.clear();
                for (const auto &vertex : vertices)
                {
                    distancesToVertices.push_back(H2Point::distance(firstTriangleVertex, vertex));
                }
                distancesToVertex.push_back(*std::min_element(distancesToVertices.begin(), distancesToVertices.end()));
            }

            double maxDistance = *std::max_element(distancesToVertex.begin(), distancesToVertex.end());
            double t;
            for (const auto &distance : distancesToVertex)
            {
                t = pow(distance/maxDistance, 0.7);
                graphTrianglesReferenceColors->push_back(QColor(weightedSum(r0, r1, t), weightedSum(g0, g1, t), weightedSum(b0, b1, t)));
            }
        }
        else
        {
            graphTrianglesReferenceColors->resize(graphTriangles.size());
            std::fill(graphTrianglesReferenceColors->begin(), graphTrianglesReferenceColors->end(), graphColor);
        }

        graphTrianglesColors = *graphTrianglesReferenceColors;
    }

    if (rightCanvas)
    {
        QColor referenceColor;
        double t;

        int r1, g1, b1;
        graphTrianglesColors.clear();
        graphTrianglesColors.reserve(graphTriangles.size());
        for (uint i=0; i!=graphTriangles.size(); ++i)
        {
            referenceColor = graphTrianglesReferenceColors->at(i);
            referenceColor.getRgb(&r1, &g1, &b1);

            t = graphTriangles[i].area()/(*graphTrianglesReferenceAreas)[i];
            r1 = rescaleReals(r1, t);
            g1 = rescaleReals(g1, t);
            b1 = rescaleReals(b1, t);

            graphTrianglesColors.push_back(QColor(r1, g1, b1));
        }
    }
}

void H2CanvasDelegateLiftedGraph::updateTrianglesTranslatesColors()
{
    graphTrianglesTranslatesColors.clear();
    graphTrianglesTranslatesColors.reserve(graphTrianglesColors.size());

    for (const auto &color : graphTrianglesColors)
    {
        int h, s, l;
        color.getHsl(&h, &s, &l);
        graphTrianglesTranslatesColors.push_back(QColor::fromHsl(h, weightedSum(s, 0, 0.5), l));
    }
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

int H2CanvasDelegateLiftedGraph::rescaleReals(int X0, const double &x)
{
    double x0 = X0/255.0;
    double y = x<1 ? x*x0 : (x0+x-1)/x;
    return Tools::intRound(255.0*y);
}

int H2CanvasDelegateLiftedGraph::weightedSum(int x0, int x1, double t)
{
    assert((t>=0) && (t<=1));
    return Tools::intRound((t*x0 + (1-t)*x1));
}
