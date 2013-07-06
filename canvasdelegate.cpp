#include "canvasdelegate.h"

#include <QPen>
#include <QPainter>
#include <QImage>
#include <QColor>

#include "canvas.h"
#include "circle.h"

CanvasDelegate::CanvasDelegate(Canvas *canvas) : canvas(canvas)
{
    //std::cout << "Entering CanvasDelegate::CanvasDelegate" << std::endl;

    pen = new QPen;

    sizeX = canvas->width();
    sizeY = canvas->height();

    image = new QImage(sizeX, sizeY, QImage::Format_RGB32);

    painter = new QPainter(image);
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->eraseRect(0, 0, sizeX, sizeY);
    painter->setPen(*pen);

    reset();

    //std::cout << "Leaving CanvasDelegate::CanvasDelegate" << std::endl;
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
    originX = intRound(originX*sizeX/(this->sizeX*1.0) + shiftX*scaleX);
    originY = intRound(originY*sizeY/(this->sizeY*1.0) + shiftY*scaleY);
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
    originX = intRound(sizeX/2.0);
    originY = intRound(sizeY/2.0);
    shiftX = 0.0;
    shiftY = 0.0;
    zoom = 1.0;
    return;
}

void CanvasDelegate::ComplexToPixelCoordinates(int &xout, int &yout, complex z) const
{
    xout = intRound(real(z)*scaleX + originX);
    yout = intRound(-imag(z)*scaleY + originY);
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
    pen->setWidth(4);
    pen->setColor(color);
    painter->setPen(*pen);
    painter->drawPoint(x,y);
    return;
}

void CanvasDelegate::drawSegment(const complex &endpoint1, const complex &endpoint2, const QColor &color)
{
    int x1, y1, x2, y2;
    ComplexToPixelCoordinates(x1, y1, endpoint1);
    ComplexToPixelCoordinates(x2, y2, endpoint2);
    pen->setWidth(1);
    pen->setColor(color);
    painter->setPen(*pen);
    painter->drawLine(x1, y1, x2, y2);
    return;
}

void CanvasDelegate::drawCircle(const complex &center, double radius, const QColor &color)
{
    int x1, y1, x2, y2;
    complex firstCorner = center + radius*(-1.0 + 1.0*I);
    complex secondCorner = center + radius*(1.0 - 1.0*I);
    ComplexToPixelCoordinates(x1, y1, firstCorner);
    ComplexToPixelCoordinates(x2, y2, secondCorner);
    pen->setColor(color);
    pen->setWidth(1);
    painter->setPen(*pen);
    painter->drawEllipse(x1, y1, x2 - x1, y2 - y1);
    return;
}

void CanvasDelegate::drawCircle(const Circle &C, const QColor &color)
{
    complex center;
    double radius;
    C.getCenterAndRadius(center, radius);
    drawCircle(center, radius, color);
    return;
}

void CanvasDelegate::drawArc(const complex &center, double radius, double angle1, double angle2, const QColor &color)
{
    int x1, y1, x2, y2;
    complex firstCorner = center + radius*(-1.0 + 1.0*I);
    complex secondCorner = center + radius*(1.0 - 1.0*I);
    ComplexToPixelCoordinates(x1, y1, firstCorner);
    ComplexToPixelCoordinates(x2, y2, secondCorner);
    pen->setColor(color);
    pen->setWidth(1);
    painter->setPen(*pen);
    double QtAngle1 = intRound(angle1*16*360/(2*M_PI));
    double QtSpan = intRound((angle2 - angle1)*16*360/(2*M_PI));
    painter->drawArc(x1, y1, x2 - x1, y2 - y1, QtAngle1, QtSpan);
    return;
}

void CanvasDelegate::drawArc(const Circle &C, double angle1, double angle2, const QColor &color)
{
    //std::cout << "Entering CanvasDelegate::drawArc" << std::endl;
    complex center;
    double radius;
    C.getCenterAndRadius(center, radius);
    drawArc(center, radius, angle1, angle2, color);
    return;
}
