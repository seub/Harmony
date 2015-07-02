#ifndef CANVASDELEGATE_H
#define CANVASDELEGATE_H

#include "tools.h"
#include "h2isometry.h"

#include <QColor>

class QImage; class QPainter; class QPen; class QMouseEvent; class QKeyEvent;

class H2Point; class Canvas; class Circle; class ActionHandler;

enum class DelegateType {GENERIC, H2DELEGATE, H2DELEGATE_GRAPH, H3DELEGATE};

class CanvasDelegate
{
    friend class Canvas;
    friend class ActionHandler;

public:
    CanvasDelegate() = delete;
    CanvasDelegate(const CanvasDelegate &) = delete;
    CanvasDelegate & operator=(CanvasDelegate) = delete;
    virtual ~CanvasDelegate();

    virtual DelegateType getDelegateType() const {return DelegateType::GENERIC;}


protected:
    CanvasDelegate(uint sizeX, uint sizeY, bool leftCanvas = false, bool rightCanvas = false, ActionHandler *handler = nullptr);
    Complex PixelToComplexCoordinates(int x, int y) const;

    virtual void resetView();
    virtual void redraw(bool back, bool top);
    virtual void redrawBack();
    virtual void redrawTop();

    void enableRedrawBuffer(bool back = true, bool top = true);

    void mouseShift(int x, int y);

    void drawPoint(const Complex &z, const QColor &color = "black", int width = 1, bool back = true);
    void highlightPoint(const Complex &z, const QColor &color = "black", int width = 3);
    void drawSegment(const Complex &endpoint1, const Complex &endpoint2, const QColor &color = "black", int width = 1, bool back = true);

    void drawCircle(const Complex &center, double radius, const QColor &color = "black", int width = 1, bool back = true);

    void drawFilledTriangle(const Complex &zA, const Complex &zB, const Complex &zC, bool back = true);

    void drawSmallerArc(const Complex &center, double radius,
                        const Complex &endpoint1, const Complex &endpoint2, const QColor &color = "black", int width = 1, bool back = true);


    int mouseX, mouseY;

    double xMin, yMax;
    double xMinSave, yMaxSave;
    double scaleX, scaleY;
    uint sizeX, sizeY;
    double xMax() const;
    double yMin() const;

    uint detectionRadius;

    bool resetPenBack;
    bool enableRedrawBufferBack, enableRedrawBufferTop;

    bool leftCanvas, rightCanvas;

    QPen *penBack, *penTop;
    QPainter *painterBack, *painterTop;

    ActionHandler *const handler;

private:
    bool getSendEndRepaint() {return sendEndRepaint;}
    void setSendEndRepaint(bool repeatRepaint) {this->sendEndRepaint = repeatRepaint;}

    const QImage *getImageBack() const;
    const QImage *getImageTop() const;

    void shift(int x, int y);
    void zoom(double coeff);

    virtual void mousePress(int, int, Qt::MouseButton, Qt::MouseButtons) {}
    virtual void mouseMove(int, int, Qt::MouseButton, Qt::MouseButtons) {}
    virtual void mouseRelease(int, int, Qt::MouseButton, Qt::MouseButtons) {}
    virtual void keyPress(QKeyEvent *keyEvent);
    virtual void enter() {}
    virtual void leave() {}

    void zoom(double coeff, int centerX, int centerY);

    bool isInside(const Complex &point) const;
    void rescale(uint sizeX, uint sizeY);
    void resetView(uint sizeX, uint sizeY);
    void ComplexToPixelCoordinates(int & xOut, int & yOut, Complex z) const;

    void bestLineSegmentApproximationOfSmallerArc(const Complex &center, double radius, const Complex &endpoint1, const Complex &endpoint2,
                                                  Complex &newEndpoint1, Complex &newEndpoint2, bool &noBestApproximation, bool &arcOutsideCanvas, bool &failed);
    bool circleIntersectsCanvasBoundary(const Complex &center, double radius, std::vector<Complex> &intersectionsOut) const;
    bool dealWithExceptionalArc(const Complex &center, double radius,
                                const Complex &endpoint1, const Complex &endpoint2, const QColor &color = "black", int width = 1, bool back = true);
    bool isAlmostSmallStraightArc(const Complex &center, double radius, const Complex &endpoint1, const Complex &endpoint2) const;
    bool isAlmostInfiniteRadius(double radius) const;
    static bool liesOnSmallerArc(const Complex &point, const Complex &center, const Complex &endpoint1, const Complex &endpoint2);

    QImage *imageBack, *imageTop;

    uint nbArcs, nbStraightArcs, nbAlmostStraightArcs; // For testing
    uint nbBestLineFails, nbBestLineCalls; // For testing

    bool sendEndRepaint;
};

#endif // CANVASDELEGATE_H
