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
    graphColor = "red";

    initializeColors(graphColor);

    setFilledTriangles(false);
    setShowTranslates(false, false);
    setIsGraphEmpty(true);
}

void H2CanvasDelegateLiftedGraph::initializeColors(const QColor &graphColor)
{
    this->graphColor = graphColor;

    int r, g, b, h, s, l;
    graphColor.getRgb(&r, &g, &b);
    graphColor.getHsl(&h, &s, &l);

    highlightColor = ((h!=-1) && ((h<120) || (h>300))) ? "blue" : "red";
    graphSidesTranslatesColor.setRgb(weightedSum(r, 50, 0.6),  weightedSum(g, 50, 0.6), weightedSum(b, 50, 0.6));
    graphTranslatesColor.setRgb(weightedSum(r, 200, 0.3),  weightedSum(g, 200, 0.3), weightedSum(b, 200, 0.3));
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

void H2CanvasDelegateLiftedGraph::mouseMove(int x, int y, Qt::MouseButton button, Qt::MouseButtons buttons)
{
    H2CanvasDelegate::mouseMove(x, y, button, buttons);
    if (leftCanvas || rightCanvas)
    {
        if (norm(PixelToComplexCoordinates(x, y)) < 1.0)
        {
            decideHighlighting(pixelToH2coordinate(x, y));
        }
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
    resetPenBack = false;

    if (showTranslatesAroundVertices)
    {
        painterBack->setPen(graphTranslatesColor);
        for (const auto & graphArcTranslate : graphArcsTranslatesAroundVertices)
        {
            drawH2GeodesicArc(graphArcTranslate);
        }
    }

    if (showTranslatesAroundVertex)
    {
        painterBack->setPen(graphTranslatesColor);

        for (const auto & graphArcTranslate : graphArcsTranslatesAroundVertex)
        {
            drawH2GeodesicArc(graphArcTranslate);
        }
    }
    painterBack->setPen(graphSidesTranslatesColor);

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

    painterBack->setPen(graphColor);

    for (const auto & graphArc : graphArcs)
    {
        drawH2GeodesicArc(graphArc);
    }

    penBack->setColor(graphColor);
    penBack->setWidth(2);
    painterBack->setPen(*penBack);

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
    resetPenBack = false;

    QBrush brush;
    brush.setStyle(Qt::SolidPattern);

    if (showTranslatesAroundVertices)
    {
        if (leftCanvas)
        {
            painterBack->setPen(graphTranslatesColor);
            brush.setColor(graphTranslatesColor);
            painterBack->setBrush(brush);

            for (const auto &triangle : graphTrianglesTranslatesAroundVertices)
            {
                drawStraightFilledH2Triangle(triangle);
            }
        }

        if (rightCanvas)
        {
            for (uint i=0; i!=graphTrianglesTranslatesAroundVertices.size(); ++i)
            {
                painterBack->setPen(graphTrianglesTranslatesColors[i % graphTriangles.size()]);
                brush.setColor(graphTrianglesTranslatesColors[i % graphTriangles.size()]);
                painterBack->setBrush(brush);
                drawStraightFilledH2Triangle(graphTrianglesTranslatesAroundVertices[i]);
            }
        }
    }

    if (showTranslatesAroundVertex)
    {
        if (leftCanvas)
        {
            painterBack->setPen(graphTranslatesColor);
            brush.setColor(graphTranslatesColor);
            painterBack->setBrush(brush);

            for (const auto &triangle : graphTrianglesTranslatesAroundVertex)
            {
                drawStraightFilledH2Triangle(triangle);
            }
        }

        if (rightCanvas)
        {
            for (uint i=0; i!=graphTrianglesTranslatesAroundVertex.size(); ++i)
            {
                painterBack->setPen(graphTrianglesTranslatesColors[i % graphTriangles.size()]);
                brush.setColor(graphTrianglesTranslatesColors[i % graphTriangles.size()]);
                painterBack->setBrush(brush);
                drawStraightFilledH2Triangle(graphTrianglesTranslatesAroundVertex[i]);
            }
        }

    }

    if (leftCanvas)
    {
        painterBack->setPen(graphColor);
        brush.setColor(graphColor);
        painterBack->setBrush(brush);
        for (const auto &triangle : graphTriangles)
        {
            drawStraightFilledH2Triangle(triangle);
        }
    }

    if (rightCanvas)
    {
        for (uint i=0; i!=graphTriangles.size(); ++i)
        {
            brush.setColor(graphTrianglesColors[i]);
            painterBack->setPen(graphTrianglesColors[i]);
            painterBack->setBrush(brush);
            drawStraightFilledH2Triangle(graphTriangles[i]);
        }
    }

    painterBack->setBrush(Qt::NoBrush);
    painterBack->setPen(graphSidesTranslatesColor);

    for (const auto & straightSideTranslate : graphSidesTranslates)
    {
        drawStraightH2GeodesicArc(straightSideTranslate);
    }

    painterBack->setPen(graphColor);
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

    updateDomainTrianglesAreas();
    updateTriangleWeights();
    updateTrianglesColors();

    if (showTranslatesAroundVertex || showTranslatesAroundVertices)
    {
        updateTrianglesTranslatesColors();
    }
}

void H2CanvasDelegateLiftedGraph::updateDomainTrianglesAreas()
{
    if (leftCanvas)
    {
        domainTrianglesAreas->clear();
        domainTrianglesAreas->reserve(graphTriangles.size());

        for (const auto &triangle : graphTriangles)
        {
            domainTrianglesAreas->push_back(triangle.area());
        }
    }
}

void H2CanvasDelegateLiftedGraph::updateTriangleWeights()
{
    if (rightCanvas)
    {
        weights.clear();
        weights.reserve(graphTriangles.size());

        for (uint i=0; i!=graphTriangles.size(); ++i)
        {
            weights.push_back(graphTriangles[i].area()/(*domainTrianglesAreas)[i]);
        }
    }
}

void H2CanvasDelegateLiftedGraph::updateTrianglesColors()
{
    if (rightCanvas)
    {
        int h, s, l;
        graphColor.getHsl(&h, &s, &l);

        graphTrianglesColors.clear();
        graphTrianglesColors.reserve(graphTriangles.size());
        for (uint i=0; i!=graphTriangles.size(); ++i)
        {
            graphTrianglesColors.push_back(QColor::fromHsl(h, s, rescaleReals(l, weights[i])));
        }
    }
}

void H2CanvasDelegateLiftedGraph::updateTrianglesTranslatesColors()
{    
    if (rightCanvas)
    {
        int h, s, l;
        graphTranslatesColor.getHsl(&h, &s, &l);

        graphTrianglesTranslatesColors.clear();
        graphTrianglesTranslatesColors.reserve(graphTriangles.size());
        for (uint i=0; i!=graphTriangles.size(); ++i)
        {
            graphTrianglesTranslatesColors.push_back(QColor::fromHsl(h, s, rescaleReals(l, weights[i])));
        }
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

    translationsAroundVertex = rho->getPairingsAroundVertex();
    Tools::pop_front(translationsAroundVertex);
    translationsAroundVertices = rho->getPairingsAroundVertices();
    Tools::pop_front(translationsAroundVertices);
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
