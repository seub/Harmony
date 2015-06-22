#ifndef CANVASDELEGATE_H
#define CANVASDELEGATE_H

#include "tools.h"
#include "h2isometry.h"

#include <QColor>

class QImage; class QPainter; class QPen; class QMouseEvent; class QKeyEvent;

class H2Point; class Canvas; class Circle; class ActionHandler;

enum class DelegateType {GENERIC, H2DELEGATE, H2DELEGATEDOMAIN, H2DELEGATETARGET, H3DELEGATE};

class CanvasDelegate
{
    friend class Canvas;

public:
    CanvasDelegate() = delete;
    CanvasDelegate(const CanvasDelegate &) = delete;
    CanvasDelegate & operator=(CanvasDelegate) = delete;
    virtual ~CanvasDelegate();

protected:
    CanvasDelegate(uint sizeX, uint sizeY, ActionHandler* handler = 0);

    virtual DelegateType getDelegateType() const {return DelegateType::GENERIC;}
    const QImage * getImageBack() const;
    const QImage * getImageTop() const;

    Complex PixelToComplexCoordinates(int x, int y) const;

    void drawPoint(const Complex &z, const QColor &color = "black", int width = 3, bool back = true);
    void drawSegment(const Complex &endpoint1, const Complex &endpoint2, const QColor &color = "black", int width = 1, bool back = true);

    void drawCircle(const Complex &center, double radius, const QColor &color = "black", int width = 1, bool back = true);

    void drawSmallerArc(const Complex &center, double radius,
                        const Complex &endpoint1, const Complex &endpoint2, const QColor &color = "black", int width = 1, bool back = true);


    void shift(int x, int y);
    void mouseShift(int x, int y);
    void zoom(double coeff);

    virtual void mousePress(QMouseEvent *) {}
    virtual void mouseMove(QMouseEvent *) {}
    virtual void mouseRelease(QMouseEvent *) {}
    virtual void keyPress(QKeyEvent *) {}
    virtual void enter() {}
    virtual void leave() {}

    void enableRedrawBuffer(bool back = true, bool top = true);


    QImage *imageBack, *imageTop;
    QPen *penBack, *penTop;
    QPainter *painterBack, *painterTop;

    uint sizeX, sizeY;
    double scaleX, scaleY;
    double xMin, yMax;
    uint nbArcs, nbStraightArcs, nbAlmostStraightArcs; // For testing
    uint nbBestLineFails, nbBestLineCalls; // For testing

    double xMinSave, yMaxSave;
    int mouseX, mouseY;
    uint detectionRadius;

    ActionHandler *handler;
    bool enableRedrawBufferBack, enableRedrawBufferTop;
    bool resetPenBack;


private:
    virtual void redrawBuffer(bool, bool) {}

    void zoom(double coeff, int centerX, int centerY);
    void resetView();
    double xMax() const;
    double yMin() const;

    bool isInside(const Complex &point) const;
    void rescale(uint sizeX, uint sizeY);
    void resetView(uint sizeX, uint sizeY);
    virtual void subResetView() {}
    void ComplexToPixelCoordinates(int & xOut, int & yOut, Complex z) const;

    void bestLineSegmentApproximationOfSmallerArc(const Complex &center, double radius, const Complex &endpoint1, const Complex &endpoint2,
                                                  Complex &newEndpoint1, Complex &newEndpoint2, bool &noBestApproximation, bool &arcOutsideCanvas, bool &failed);
    bool circleIntersectsCanvasBoundary(const Complex &center, double radius, std::vector<Complex> &intersectionsOut) const;
    bool dealWithExceptionalArc(const Complex &center, double radius,
                                const Complex &endpoint1, const Complex &endpoint2, const QColor &color = "black", int width = 1, bool back = true);
    bool isAlmostSmallStraightArc(const Complex &center, double radius, const Complex &endpoint1, const Complex &endpoint2) const;
    bool isAlmostInfiniteRadius(double radius) const;
    static bool liesOnSmallerArc(const Complex &point, const Complex &center, const Complex &endpoint1, const Complex &endpoint2);

};

#endif // CANVASDELEGATE_H
