#ifndef CANVASDELEGATE_H
#define CANVASDELEGATE_H

#include "types.h"

class QImage;
class QPainter;
class QPen;
class QColor;

class H2Point;
class Canvas;

class CanvasDelegate
{
public:
    CanvasDelegate(Canvas *canvas = 0);
    ~CanvasDelegate();

    QImage * getImage() const;


    void drawPoint(const complex &z, const QColor &color);

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
