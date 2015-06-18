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

protected:
    explicit H2CanvasDelegate(int sizeX, int sizeY, ActionHandler* handler = 0);

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
    void getMeshIndexHighlighted(bool &highlighted, int &meshIndexHighted) const;
    void getMeshTriangleIndicesHighlighted(bool &highlighted, int &index1, int &index2, int &index3) const;
    virtual void decideHighlightingMeshPoints(bool, bool&, int) {}
    virtual void decideHighlightingTriangle(bool, bool&, int, int, int) {}
    void resetHighlighted();
    void setShowTranslates(bool showTranslatesAroundVertex, bool showTranslatesAroundAllVertices);
    void getShowTranslates(bool &aroundVertexOut, bool &aroundVerticesOut);

    void redrawBuffer(bool back = true, bool top = true, const H2Isometry &mobius = H2Isometry::identity());
    void redrawMeshOrFunction();
    void redrawBufferBack();
    void redrawBufferTop();
    virtual void subRedrawBufferBack() {}
    virtual void subRedrawBufferTop() {}

    virtual void mousePress(QMouseEvent * mouseEvent);
    virtual void mouseMove(QMouseEvent * mouseEvent);
    virtual void keyPress(QKeyEvent * keyEvent);
    virtual void subMouseMove(QMouseEvent *) {}
    virtual void subKeyPress(QKeyEvent *) {}
    virtual void leave();

    H2Buffer buffer;
    H2Isometry mobius;
    H2Isometry savedMobius;
    bool isTriangleHighlighted;
    int triangleMeshIndex1, triangleMeshIndex2, triangleMeshIndex3, meshIndexHighlighted;

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

private:
    explicit H2CanvasDelegateDomain(int sizeX, int sizeY, ActionHandler *handler = 0);

    void refreshMesh();

    void decideHighlighting(const H2Point &pointUnderMouse);
    void decideHighlightingMeshPoints(bool highlighted, bool &update, int meshIndexHighlighted = -1);
    void decideHighlightingTriangle(bool highlighted, bool& update, int triangleMeshIndex1 = -1, int triangleMeshIndex2 = -1, int triangleMeshIndex3 = -1);
    void subRedrawBufferBack();
    void subRedrawBufferTop();
    void subMouseMove(QMouseEvent * mouseEvent);
};

class H2CanvasDelegateTarget : public H2CanvasDelegate
{
    friend class Canvas;
    friend class ActionHandler;

public:

private:
    explicit H2CanvasDelegateTarget(int sizeX, int sizeY, ActionHandler *handler = 0);

    void refreshFunction();

    void decideHighlighting(const H2Point &pointUnderMouse);
    void decideHighlightingMeshPoints(bool highlighted, bool &update, int meshIndexHighlighted = -1);
    void decideHighlightingTriangle(bool highlighted, bool &update, int triangleMeshIndex1 = -1, int triangleMeshIndex2 = -1, int triangleMeshIndex3 = -1);
    void subRedrawBufferBack();
    void subRedrawBufferTop();
    void subMouseMove(QMouseEvent *mouseEvent);
    void subKeyPress(QKeyEvent *keyEvent);
};


#endif // H2CANVASDELEGATE_H
