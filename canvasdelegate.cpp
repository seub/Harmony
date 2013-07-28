#include "canvasdelegate.h"

#include <QPen>
#include <QPainter>
#include <QImage>
#include <QColor>

#include "canvas.h"
#include "circle.h"
#include "tools.h"

CanvasDelegate::CanvasDelegate(Canvas *canvas) : canvas(canvas)
{
    //std::cout << "Entering CanvasDelegate::CanvasDelegate" << std::endl;

    pen = new QPen;

    image = new QImage(canvas->width(), canvas->height(), QImage::Format_RGB32);

    painter = new QPainter(image);
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->eraseRect(0, 0, canvas->width(), canvas->height());
    painter->setPen(*pen);

    resetView();

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
    //std::cout << "Entering CanvasDelegate::rescale" << std::endl;

    //QImage * oldImage = image;
    QImage * newImage = new QImage(sizeX, sizeY, QImage::Format_RGB32);

    //QPainter * oldPainter = painter;
    QPainter * newPainter = new QPainter(newImage);
    newPainter->setRenderHint(QPainter::Antialiasing, true);
    newPainter->eraseRect(0, 0, sizeX, sizeY);
    newPainter->setPen(*pen);

    image = newImage;
    painter = newPainter;

    // Memory leak!
    //delete oldImage;
    //delete oldPainter;

    double xFactor = (sizeX *1.0/ this->sizeX);
    double yFactor = (sizeY *1.0/ this->sizeY);

    this->sizeX = sizeX;
    this->sizeY = sizeY;

    scaleX = xFactor * scaleX;
    scaleY = yFactor * scaleY;

    //std::cout << "Leaving CanvasDelegate::rescale" << std::endl;

    return;
}

void CanvasDelegate::resetView()
{
    xMin = -1.1;
    yMax = 1.1;

    sizeX = canvas->width();
    sizeY = canvas->height();


    scaleX = sizeX/2.2;
    scaleY = sizeY/2.2;
    return;
}

void CanvasDelegate::ComplexToPixelCoordinates(int &xOut, int &yOut, complex z) const
{
    double x = real(z), y = imag(z);
    xOut = Tools::intRound((x - xMin)*scaleX);
    yOut = Tools::intRound((yMax - y)*scaleY);
    return;
}

complex CanvasDelegate::PixelToComplexCoordinates(int x, int y) const
{
    double a = xMin + (x/scaleX);
    double b = yMax - (y/scaleY);
    return complex(a,b);
}



void CanvasDelegate::setZoom(double coeff, int centerX, int centerY)
{
    double x = xMin + centerX/scaleX;
    double y = yMax - centerY/scaleY;

    xMin = x - (x - xMin)/coeff;
    yMax = y + (yMax - y)/coeff;

    scaleX *= coeff;
    scaleY *= coeff;

    //redrawBuffer();
    return;
}

void CanvasDelegate::setMouse(int mouseX, int mouseY)
{
    this->mouseX = mouseX;
    this->mouseY = mouseY;
    return;
}


void CanvasDelegate::drawPoint(const complex &z, const QColor &color, int width)
{
    int x, y;
    ComplexToPixelCoordinates(x, y, z);
    pen->setWidth(width);
    pen->setColor(color);
    painter->setPen(*pen);
    painter->drawPoint(x,y);
    return;
}

void CanvasDelegate::drawSegment(const complex &endpoint1, const complex &endpoint2, const QColor &color, int width)
{
    int x1, y1, x2, y2;
    ComplexToPixelCoordinates(x1, y1, endpoint1);
    ComplexToPixelCoordinates(x2, y2, endpoint2);
    pen->setWidth(width);
    pen->setColor(color);
    painter->setPen(*pen);
    painter->drawLine(x1, y1, x2, y2);
    return;
}

void CanvasDelegate::drawCircle(const complex &center, double radius, const QColor &color, int width)
{
    int x1, y1, x2, y2;
    complex firstCorner = center + radius*(-1.0 + 1.0*I);
    complex secondCorner = center + radius*(1.0 - 1.0*I);
    ComplexToPixelCoordinates(x1, y1, firstCorner);
    ComplexToPixelCoordinates(x2, y2, secondCorner);
    pen->setColor(color);
    pen->setWidth(width);
    painter->setPen(*pen);
    painter->drawEllipse(x1, y1, x2 - x1, y2 - y1);
    return;
}

void CanvasDelegate::drawCircle(const Circle &C, const QColor &color, int width)
{
    complex center;
    double radius;
    C.getCenterAndRadius(center, radius);
    drawCircle(center, radius, color, width);
    return;
}

void CanvasDelegate::drawArcCounterClockwise(const complex &center, double radius, double angle1, double angle2, const QColor &color, int width)
{
    /*double angleMin = M_PI/(180*16);
    if(radius*scaleX*angleMin>10.0)
    {
        complex z1 = center + radius*exp(angle1*I);
        complex z2 = center + radius*exp(angle2*I);
        drawSegment(z1,z2,color,width);
        return;
    }*/
    int x1, y1, x2, y2;
    complex firstCorner = center + radius*(-1.0 + 1.0*I);
    complex secondCorner = center + radius*(1.0 - 1.0*I);
    ComplexToPixelCoordinates(x1, y1, firstCorner);
    ComplexToPixelCoordinates(x2, y2, secondCorner);
    pen->setColor(color);
    pen->setWidth(width);
    painter->setPen(*pen);
    int QtAngle1 = Tools::intRound(Tools::mod2Pi(angle1)*16*360/(2*M_PI));
    int QtSpan = Tools::intRound(Tools::mod2Pi(angle2 - angle1)*16*360/(2*M_PI));
    painter->drawArc(x1, y1, x2 - x1, y2 - y1, QtAngle1, QtSpan);
    return;
}

void CanvasDelegate::drawSmallerArc(const complex &center, double radius, double angle1,
                                    double angle2, const QColor &color, int width)
{
    double d = angle2 - angle1;
    d = Tools::mod2Pi(d);
    if (d>M_PI)
    {
        drawArcCounterClockwise(center, radius, angle2, angle1, color, width);
    }
    else
    {
        drawArcCounterClockwise(center, radius, angle1, angle2, color, width);
    }
    return;
}

void CanvasDelegate::drawSmallerArc(const Circle &C, double angle1, double angle2, const QColor &color, int width)
{
    complex center;
    double radius;
    C.getCenterAndRadius(center, radius);
    drawSmallerArc(center, radius, angle1, angle2, color, width);
    return;
}

void CanvasDelegate::drawArcCounterClockwise(const Circle &C, double angle1, double angle2, const QColor &color, int width)
{
    //std::cout << "Entering CanvasDelegate::drawArc" << std::endl;
    complex center;
    double radius;
    C.getCenterAndRadius(center, radius);
    drawArcCounterClockwise(center, radius, angle1, angle2, color, width);
    return;
}
