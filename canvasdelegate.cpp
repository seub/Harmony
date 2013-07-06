#include "canvasdelegate.h"

#include <QPen>
#include <QPainter>
#include <QImage>
#include <QColor>

#include "canvas.h"

CanvasDelegate::CanvasDelegate(Canvas *canvas) : canvas(canvas)
{
    pen = new QPen;

    sizeX = canvas->width();
    sizeY = canvas->height();

    image = new QImage(sizeX, sizeY, QImage::Format_RGB32);

    painter = new QPainter(image);
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->eraseRect(0, 0, sizeX, sizeY);
    painter->setPen(*pen);

    reset();

}

CanvasDelegate::~CanvasDelegate()
{
    delete image;
    delete painter;
    delete pen;
}

QImage * CanvasDelegate::getImage() const
{
    return image;
}

void CanvasDelegate::rescale(int sizeX, int sizeY)
{
    originX = int_round(originX*sizeX/(this->sizeX*1.0) + shiftX*scaleX);
    originY = int_round(originY*sizeY/(this->sizeY*1.0) + shiftY*scaleY);
    this->sizeX = sizeX;
    this->sizeY = sizeY;
    scaleX = zoom*sizeX/(xmax - xmin);
    scaleY = zoom*sizeY/(ymax - ymin);
    return;
}

void CanvasDelegate::reset()
{
    scaleX = sizeX/2.2;
    scaleY = sizeY/2.2;
    originX = int_round(sizeX/2.0);
    originY = int_round(sizeY/2.0);
    shiftX = 0.0;
    shiftY = 0.0;
    zoom = 1.0;
    return;
}

void CanvasDelegate::ComplexToPixelCoordinates(int &xout, int &yout, complex z) const
{
    xout = int_round(real(z)*scaleX + originX);
    yout = int_round(-imag(z)*scaleY + originY);
    return;
}

complex CanvasDelegate::PixelToComplexCoordinates(int x, int y) const
{
    double a = (x - originX)*1.0/scaleX;
    double b = -(y - originY)*1.0/scaleY;
    return complex(a,b);
}

void CanvasDelegate::drawPoint(const complex &z, const QColor &color)
{
    int x, y;
    ComplexToPixelCoordinates(x, y, z);
    pen->setWidth(5);
    pen->setColor(color);
    painter->setPen(*pen);
    painter->drawPoint(x,y);
    return;
}
