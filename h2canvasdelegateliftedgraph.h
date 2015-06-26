#ifndef H2LIFTEDGRAPHCANVASDELEGATE_H
#define H2LIFTEDGRAPHCANVASDELEGATE_H

#include "tools.h"
#include "canvasdelegate.h"
#include "h2canvasdelegate.h"


template<typename Point, typename Map> class LiftedGraphFunctionTriangulated;

class H2CanvasDelegateLiftedGraph : public H2CanvasDelegate
{
    friend class Canvas;
    friend class ActionHandler;
    friend class FenchelNielsenUser;

public:
    virtual DelegateType getDelegateType() const override {return DelegateType::H2DELEGATE_GRAPH;}
    virtual ~H2CanvasDelegateLiftedGraph() {}

private:
    H2CanvasDelegateLiftedGraph(uint sizeX, uint sizeY, bool leftCanvas = false, bool rightCanvas = false, ActionHandler *handler = nullptr);

    void initializeColors(const QColor &graphColor);

    virtual void redrawBack() override;
    virtual void redrawTop() override;
    virtual void resetView() override;
    void redrawGraph();
    void redrawNonFilledGraph();
    void redrawFilledGraph();

    virtual void mouseMove(QMouseEvent * mouseEvent) override;
    virtual void enter() override;
    virtual void leave() override;

    void setIsGraphEmpty(bool isGraphEmpty);
    void setIsRhoEmpty(bool isRhoEmpty);

    void setRhoPointer(GroupRepresentation<H2Isometry> *rho);
    void setGraphPointer(LiftedGraphFunctionTriangulated<H2Point, H2Isometry> *graph);
    void updateGraph(bool refreshSidesTranslates, bool refreshFullTranslates);
    void updateNonFilledGraph(bool refreshSidesTranslates, bool refreshFullTranslates);
    void updateFilledGraph(bool refreshSidesTranslates, bool refreshFullTranslates);
    void refreshRho();
    void setFilledTriangles(bool filledTriangles);
    void setShowTranslates(bool showTranslatesAroundVertex, bool showTranslatesAroundVertices);
    void getShowTranslates(bool &aroundVertexOut, bool &aroundVerticesOut);
    const std::vector<double> & getGraphTrianglesReferenceAreas() const;
    void setGraphTrianglesReferenceAreas(const std::vector<double> &graphTrianglesReferenceAreas);


    void getGraphIndexHighlighted(bool &highlighted, uint &graphIndexHighted) const;
    void getGraphTriangleIndicesHighlighted(bool &highlighted, uint &index1, uint &index2, uint &index3) const;
    void resetHighlighted();
    void decideHighlighting(const H2Point &pointUnderMouse);
    void decideHighlightingGraphPoints(bool highlighted, bool &update, uint graphIndexHighlighted);
    void decideHighlightingTriangle(bool highlighted, bool& update, uint triangleGraphIndex1, uint triangleGraphIndex2, uint triangleGraphIndex3);

    static int f(int X0, const double &x);
    static int g(int x0, int x1);

    bool isTriangleHighlighted;
    uint triangleGraphIndex1, triangleGraphIndex2, triangleGraphIndex3;
    H2Triangle triangleHighlighted;
    uint graphIndexHighlighted;
    bool arePointsHighlightedRed;
    bool arePointsHighlightedGreen;
    bool arePointsHighlightedBlue;
    std::vector<H2Point> pointsHighlightedRed;
    std::vector<H2Point> pointsHighlightedGreen;
    std::vector<H2Point> pointsHighlightedBlue;

    bool showTranslatesAroundVertex, showTranslatesAroundVertices;
    bool filledTriangles;

    GroupRepresentation<H2Isometry> *rho;
    bool isRhoEmpty;
    std::vector<H2Geodesic> rhoAxes;

    LiftedGraphFunctionTriangulated<H2Point, H2Isometry> *graph;
    bool isGraphEmpty;
    std::vector<H2GeodesicArc> graphArcs;
    std::vector<H2GeodesicArc> graphSides;
    std::vector<H2GeodesicArc> graphSidesTranslates;
    std::vector<H2GeodesicArc> graphLargeSides;
    std::vector<H2GeodesicArc> graphLargeSidesTranslates;
    std::vector<H2GeodesicArc> graphArcsTranslatesAroundVertex;
    std::vector<H2GeodesicArc> graphArcsTranslatesAroundVertices;
    QColor graphColor, graphTranslatesColor, graphSidesTranslatesColor;

    std::vector<H2Triangle> graphTriangles;
    std::vector<H2Triangle> graphTrianglesTranslatesAroundVertex;
    std::vector<H2Triangle> graphTrianglesTranslatesAroundVertices;
    std::vector<double> graphTrianglesReferenceAreas;
    std::vector<double> graphTrianglesWeights;


    std::vector<H2Isometry> translationsAroundVertex;
    std::vector<H2Isometry> translationsAroundVertices;
};

#endif // H2LIFTEDGRAPHCANVASDELEGATE_H
