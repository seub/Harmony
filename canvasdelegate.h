#ifndef CANVASDELEGATE_H
#define CANVASDELEGATE_H

#include "types.h"

#include <QColor>

class QImage;
class QPainter;
class QPen;

class H2Point;
class Canvas;

class CanvasDelegate
{
public:
    CanvasDelegate(Canvas *canvas);
    ~CanvasDelegate();

    QImage * getImage() const;


    void drawPoint(const complex &z, const QColor &color = QColor("black"));
    void drawSegment(const complex &endpoint1, const complex &endpoint2, const QColor &color = QColor("black"));
    void drawCircle(const complex &center, double radius, const QColor &color = QColor("black"));
    void drawArc(const complex &center, double radius, double angle1, double angle2, const QColor &color = QColor("black"));

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

    virtual void paintEvent() {}


};

#endif // CANVASDELEGATE_H
