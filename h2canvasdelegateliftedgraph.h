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
    void redrawFilledGraph2Colors();

    virtual void mouseMove(QMouseEvent * mouseEvent) override;
    virtual void enter() override;
    virtual void leave() override;

    void setIsGraphEmpty(bool isGraphEmpty);
    void setIsRhoEmpty(bool isRhoEmpty);

    void setRhoPointer(GroupRepresentation<H2Isometry> *rho);
    void setGraphPointer(LiftedGraphFunctionTriangulated<H2Point, H2Isometry> *graph);

    void updateGraph(bool refreshSidesTranslates);
    void updateNonFilledGraph(bool refreshSidesTranslates);
    void updateFilledGraph(bool refreshSidesTranslates);
    void updateTrianglesWeights();
    void updateTrianglesColors();
    void updateTrianglesTranslatesColors();
    void refreshRho();
    void setFilledTriangles(bool filledTriangles);

    void setTwoColor(bool twoColors);
    void setGraphColor(const QColor &color);
    void setShowTranslates(bool showTranslatesAroundVertex, bool showTranslatesAroundVertices);

    void getGraphTriangleIndicesHighlighted(bool &highlighted, uint &index1, uint &index2, uint &index3) const;
    void resetHighlighted();
    void decideHighlighting(const H2Point &pointUnderMouse);
    void decideHighlightingTriangle(bool highlighted, bool& update, uint triangleGraphIndex1, uint triangleGraphIndex2, uint triangleGraphIndex3);

    static int rescaleReals(int X0, const double &x);
    static int weightedSum(int x0, int x1, double t);

    bool isTriangleHighlighted;
    uint triangleGraphIndex1, triangleGraphIndex2, triangleGraphIndex3;
    H2Triangle triangleHighlighted;
    QColor highlightColor;

    bool showTranslatesAroundVertex, showTranslatesAroundVertices;
    bool filledTriangles, twoColors;

    GroupRepresentation<H2Isometry> *rho;
    bool isRhoEmpty;
    std::vector<H2Geodesic> rhoAxes;
    std::vector<H2Isometry> translationsAroundVertex;
    std::vector<H2Isometry> translationsAroundVertices;

    LiftedGraphFunctionTriangulated<H2Point, H2Isometry> *graph;
    bool isGraphEmpty;
    std::vector<H2GeodesicArc> graphArcs;
    std::vector<H2GeodesicArc> graphSides;
    std::vector<H2GeodesicArc> graphSidesTranslates;
    std::vector<H2GeodesicArc> graphLargeSides;
    std::vector<H2GeodesicArc> graphLargeSidesTranslates;
    std::vector<H2GeodesicArc> graphArcsTranslatesAroundVertex;
    std::vector<H2GeodesicArc> graphArcsTranslatesAroundVertices;
    QColor graphColor, graphColor2, graphSidesTranslatesColor;

    std::vector<H2Triangle> graphTriangles;
    std::vector<H2Triangle> graphTrianglesTranslatesAroundVertex;
    std::vector<H2Triangle> graphTrianglesTranslatesAroundVertices;
    std::shared_ptr< std::vector<double> > graphTrianglesReferenceAreas;
    std::shared_ptr< std::vector<QColor> > graphTrianglesReferenceColors;
    std::vector<QColor> graphTrianglesColors, graphTrianglesTranslatesColors;
};

#endif // H2LIFTEDGRAPHCANVASDELEGATE_H
