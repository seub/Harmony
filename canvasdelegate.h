#ifndef CANVASDELEGATE_H
#define CANVASDELEGATE_H

#include "tools.h"
#include "h2isometry.h"

#include <QColor>

class QImage;
class QPainter;
class QPen;

class H2Point;
class Canvas;
class Circle;


class CanvasDelegate
{
    friend class Canvas;

public:
    CanvasDelegate(Canvas *canvas);
    ~CanvasDelegate();

    QImage * getImage() const;


    void drawPoint(const complex &z, const QColor &color = "black", int width = 3);
    void drawSegment(const complex &endpoint1, const complex &endpoint2, const QColor &color = "black", int width = 1);

    void drawCircle(const complex &center, double radius, const QColor &color = "black", int width = 1);
    void drawCircle(const Circle &C, const QColor &color = "black", int width = 1);

    void drawArcCounterClockwise(const complex &center, double radius, double angleStart, double angleEnd, const QColor &color = "black", int width = 1);
    void drawArcCounterClockwise(const Circle &C, double angle1, double angle2, const QColor &color = "black", int width = 1);

    void drawSmallerArc(const complex &center, double radius, double angle1, double angle2,
                        const QColor &color = "black", int width = 1);
    void drawSmallerArc(const Circle &C, double angle1, double angle2,
                        const QColor &color = "black", int width = 1);

    virtual void redrawBuffer(const H2Isometry &mobius = H2Isometry::identity()) =0;

    void setZoom(double coeff, int centerX, int centerY);
    virtual void setMouse(int mouseX, int mouseY);
    virtual void mouseMove(const int mouseX, const int mouseY) = 0;


protected:
    Canvas *canvas;

    int sizeX, sizeY;
    double scaleX, scaleY;
    double xMin, yMax;
    int mouseX, mouseY;

    QPen *pen;
    QImage *image;
    QPainter *painter;

    void rescale(int sizeX, int sizeY);
    void resetView();
    complex PixelToComplexCoordinates(int x, int y) const;
    void ComplexToPixelCoordinates(int & xOut, int & yOut, complex z) const;

};

#endif // CANVASDELEGATE_H
