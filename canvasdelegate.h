#ifndef CANVASDELEGATE_H
#define CANVASDELEGATE_H

#include "tools.h"

#include <QColor>

class QImage;
class QPainter;
class QPen;

class H2Point;
class Canvas;
class Circle;

class CanvasDelegate
{
public:
    CanvasDelegate(Canvas *canvas);
    ~CanvasDelegate();

    QImage * getImage() const;


    void drawPoint(const complex &z, const QColor &color = "black", int width = 4);
    void drawSegment(const complex &endpoint1, const complex &endpoint2, const QColor &color = "black", int width = 1);
    void drawCircle(const complex &center, double radius, const QColor &color = "black", int width = 1);
    void drawCircle(const Circle &C, const QColor &color = "black", int width = 1);
    void drawArcCounterClockwise(const complex &center, double radius, double angleStart, double angleEnd, const QColor &color = "black", int width = 1);
    void drawSmallerArc(const complex &center, double radius, double angle1, double angle2,
                        const QColor &color = "black", int width = 1);
    void drawSmallerArc(const Circle &C, double angle1, double angle2,
                        const QColor &color = "black", int width = 1);
    void drawArcCounterClockwise(const Circle &C, double angle1, double angle2, const QColor &color = "black", int width = 1);

protected:
    Canvas *canvas;

    int sizeX, sizeY;
    int originX, originY;
    double scaleX, scaleY, zoom;
    double shiftX, shiftY;
    double xmin, xmax, ymin, ymax;


    QPen *pen;
    QImage *image;
    QPainter *painter;

    void rescale(int sizeX, int sizeY);
    void reset();
    complex PixelToComplexCoordinates(int x, int y) const;
    void ComplexToPixelCoordinates(int & xout, int & yout, complex z) const;

    //virtual void paintEvent() {}


};

#endif // CANVASDELEGATE_H
