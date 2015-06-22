#ifndef H2CANVASDELEGATE_H
#define H2CANVASDELEGATE_H

#include "tools.h"
#include "canvasdelegate.h"
#include "h2point.h"
#include "h2geodesic.h"
#include "h2polygon.h"
#include "grouprepresentation.h"
#include "h2buffer.h"

class H2CanvasDelegate : public CanvasDelegate
{
    friend class ActionHandler;
    friend class Canvas;
    friend class FenchelNielsenUser;

public:
    virtual DelegateType getDelegateType() const override {return DelegateType::H2DELEGATE;}
    virtual ~H2CanvasDelegate() {}

protected:
    H2CanvasDelegate(uint sizeX, uint sizeY, ActionHandler* handler = 0);

    H2Point pixelToH2coordinate(int x, int y) const;
    void drawComplexPointInDiskModel(const Complex &z, const QColor &color = "black", int width = 1, bool back = true);
    void drawH2Point(const H2Point &p, const QColor &color = "black", int width = 4, bool back = true);
    void drawH2Geodesic(const H2Geodesic &L, const QColor &color = "black", int width = 1, bool back = true);
    void drawH2GeodesicArc(const H2GeodesicArc &L, const QColor &color = "black", int width = 2, bool back = true);
    void drawH2Triangle(const H2Triangle &triangle, const QColor &color, int width, bool back = true);

    void setIsMeshEmpty(bool isMeshEmpty);
    void setIsRhoEmpty(bool isRhoEmpty);
    void setIsFunctionEmpty(bool isFunctionEmpty);

    void refreshTranslates();
    void refreshTranslates(bool aroundVertex, bool aroundVertices);

    virtual void decideHighlighting(const H2Point &) {}
    void getMeshIndexHighlighted(bool &highlighted, uint &meshIndexHighted) const;
    void getMeshTriangleIndicesHighlighted(bool &highlighted, uint &index1, uint &index2, uint &index3) const;
    virtual void decideHighlightingMeshPoints(bool, bool&, uint) {}
    virtual void decideHighlightingTriangle(bool, bool&, uint, uint, uint) {}
    void resetHighlighted();
    void setShowTranslates(bool showTranslatesAroundVertex, bool showTranslatesAroundAllVertices);
    void getShowTranslates(bool &aroundVertexOut, bool &aroundVerticesOut);

    void redrawBuffer(bool back = true, bool top = true);
    void redrawMeshOrFunction();
    void redrawBufferBack();
    void redrawBufferTop();
    virtual void subRedrawBufferBack() {}
    virtual void subRedrawBufferTop() {}
    virtual void subResetView();

    void mousePress (QMouseEvent * mouseEvent) override final;
    void mouseMove(QMouseEvent * mouseEvent) override final;
    void mouseRelease(QMouseEvent *) override final;
    void keyPress(QKeyEvent * keyEvent) override final;
    virtual void subMouseMove(QMouseEvent *) {}
    virtual void subKeyPress(QKeyEvent *) {}
    void enter() override final;
    void leave() override final;


    bool mobiusing;

    H2Buffer buffer;
    H2Isometry mobius;
    H2Isometry savedMobius;
    bool isTriangleHighlighted;
    uint triangleMeshIndex1, triangleMeshIndex2, triangleMeshIndex3, meshIndexHighlighted;

    bool arePointsHighlightedRed;
    bool arePointsHighlightedGreen;
    bool arePointsHighlightedBlue;
    bool showTranslatesAroundVertex, showTranslatesAroundAllVertices;
};



class H2CanvasDelegateDomain : public H2CanvasDelegate
{
    friend class Canvas;
    friend class ActionHandler;

public:
    DelegateType getDelegateType() const override final {return DelegateType::H2DELEGATEDOMAIN;}

private:
    H2CanvasDelegateDomain(uint sizeX, uint sizeY, ActionHandler *handler = 0);

    void refreshMesh();

    void decideHighlighting(const H2Point &pointUnderMouse) override final;
    void decideHighlightingMeshPoints(bool highlighted, bool &update, uint meshIndexHighlighted) override final;
    void decideHighlightingTriangle(bool highlighted, bool& update, uint triangleMeshIndex1, uint triangleMeshIndex2, uint triangleMeshIndex3) override final;
    void subRedrawBufferBack() override final;
    void subRedrawBufferTop() override final;
    void subMouseMove(QMouseEvent * mouseEvent) override final;
};

class H2CanvasDelegateTarget : public H2CanvasDelegate
{
    friend class Canvas;
    friend class ActionHandler;

public:
     DelegateType getDelegateType() const override final {return DelegateType::H2DELEGATETARGET;}

private:
    H2CanvasDelegateTarget(int sizeX, int sizeY, ActionHandler *handler = 0);

    void refreshFunction();

    void decideHighlighting(const H2Point &pointUnderMouse) override final;
    void decideHighlightingMeshPoints(bool highlighted, bool &update, uint meshIndexHighlighted) override final;
    void decideHighlightingTriangle(bool highlighted, bool &update, uint triangleMeshIndex1, uint triangleMeshIndex2, uint triangleMeshIndex3) override final;
    void subRedrawBufferBack() override final;
    void subRedrawBufferTop() override final;
    void subMouseMove(QMouseEvent *mouseEvent) override final;
    void subKeyPress(QKeyEvent *keyEvent) override final;
};


#endif // H2CANVASDELEGATE_H
