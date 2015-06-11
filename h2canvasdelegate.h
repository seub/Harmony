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
public:
    explicit H2CanvasDelegate(int sizeX, int sizeY);

    void drawComplexPointInDiskModel(const Complex &z, const QColor &color = "black", int width = 1);
    void drawH2Point(const H2Point &p, const QColor &color = "black", int width = 4);
    void drawH2Geodesic(const H2Geodesic &L, const QColor &color = "black", int width = 1);
    void drawH2GeodesicArc(const H2GeodesicArc &L, const QColor &color = "black", int width = 2);

    virtual void redrawBuffer(const H2Isometry &mobius = H2Isometry::identity());
    virtual void mousePress(QMouseEvent * mouseEvent);
    virtual void mouseMove(QMouseEvent * mouseEvent);
    virtual void keyPress(QKeyEvent * keyEvent);

    virtual void subRedrawBuffer() {}
    virtual void subMouseMove(QMouseEvent *) {}

    //protected:
    H2Buffer buffer;
    H2Isometry mobius;
    H2Isometry savedMobius;
    bool isTriangleHighlighted;
    bool isPointHighlighted;
    bool arePointsHighlighted;
};



class H2CanvasDelegateDomain : public H2CanvasDelegate
{
public:
    explicit H2CanvasDelegateDomain(int sizeX, int sizeY);

    void subRedrawBuffer();
    void subMouseMove(QMouseEvent * mouseEvent);
    //private:
};

class H2CanvasDelegateTarget : public H2CanvasDelegate
{
public:
    explicit H2CanvasDelegateTarget(int sizeX, int sizeY);

    void subRedrawBuffer();
    void subMouseMove(QMouseEvent *);
    //private:
};


#endif // H2CANVASDELEGATE_H
