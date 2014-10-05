#include "canvasdelegate.h"

#include <QPen>
#include <QPainter>
#include <QImage>
#include <QColor>
#include <QMouseEvent>

#include "canvas.h"
#include "circle.h"
#include "tools.h"

CanvasDelegate::CanvasDelegate(const Canvas *canvas) : canvas(canvas)
{
    //std::cout << "Entering CanvasDelegate::CanvasDelegate" << std::endl;

    pen = new QPen;

    image = new QImage(canvas->width(), canvas->height(), QImage::Format_RGB32);

    painter = new QPainter(image);
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->eraseRect(0, 0, canvas->width(), canvas->height());
    painter->setPen(*pen);

    detectionRadius = 20;
    resetView();

    //std::cout << "Leaving CanvasDelegate::CanvasDelegate" << std::endl;
}

CanvasDelegate::~CanvasDelegate()
{
    delete image;
    delete painter;
    delete pen;
}

const QImage *CanvasDelegate::getImage() const
{
    return image;
}

void CanvasDelegate::rescale(int sizeX, int sizeY)
{
    //std::cout << "Entering CanvasDelegate::rescale" << std::endl;

    delete painter;
    *image = QImage(sizeX, sizeY, QImage::Format_RGB32);
    painter = new QPainter(image);
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->eraseRect(0, 0, sizeX, sizeY);
    painter->setPen(*pen);


    double xFactor = (sizeX *1.0/ this->sizeX);
    double yFactor = (sizeY *1.0/ this->sizeY);

    this->sizeX = sizeX;
    this->sizeY = sizeY;

    scaleX = xFactor * scaleX;
    scaleY = yFactor * scaleY;

    //std::cout << "Leaving CanvasDelegate::rescale" << std::endl;
}

void CanvasDelegate::resetView()
{
    xMin = -1.1;
    yMax = 1.1;

    sizeX = canvas->width();
    sizeY = canvas->height();

    scaleX = sizeX/2.2;
    scaleY = sizeY/2.2;
}

void CanvasDelegate::ComplexToPixelCoordinates(int &xOut, int &yOut, Complex z) const
{
    double x = real(z), y = imag(z);
    xOut = Tools::intRound((x - xMin)*scaleX);
    yOut = Tools::intRound((yMax - y)*scaleY);
}

Complex CanvasDelegate::PixelToComplexCoordinates(int x, int y) const
{
    double a = xMin + (x/scaleX);
    double b = yMax - (y/scaleY);
    return Complex(a,b);
}


void CanvasDelegate::zoom(double coeff)
{
    int centerX = Tools::intRound(sizeX/2.0);
    int centerY = Tools::intRound(sizeY/2.0);
    zoom(coeff, centerX, centerY);
}

void CanvasDelegate::zoom(double coeff, int centerX, int centerY)
{
    double x = xMin + centerX/scaleX;
    double y = yMax - centerY/scaleY;

    xMin = x - (x - xMin)/coeff;
    yMax = y + (yMax - y)/coeff;

    scaleX *= coeff;
    scaleY *= coeff;

    //redrawBuffer();
}

void CanvasDelegate::mouseShift(int x, int y)
{
    //std::cout << "Entering CanvasDelegate::shift" << std::endl;
    xMin = xMinSave + (mouseX - x)/scaleX;
    yMax = yMaxSave - (mouseY - y)/scaleY;
}

void CanvasDelegate::shift(int x, int y)
{
    xMin += x/scaleX;
    yMax += y/scaleY;
}

void CanvasDelegate::drawPoint(const Complex &z, const QColor &color, int width)
{
    int x, y;
    ComplexToPixelCoordinates(x, y, z);
    pen->setWidth(width);
    pen->setColor(color);
    painter->setPen(*pen);
    painter->drawPoint(x,y);
}

void CanvasDelegate::drawSegment(const Complex &endpoint1, const Complex &endpoint2, const QColor &color, int width)
{
    int x1, y1, x2, y2;
    ComplexToPixelCoordinates(x1, y1, endpoint1);
    ComplexToPixelCoordinates(x2, y2, endpoint2);
    pen->setWidth(width);
    pen->setColor(color);
    painter->setPen(*pen);
    painter->drawLine(x1, y1, x2, y2);
}

void CanvasDelegate::drawCircle(const Complex &center, double radius, const QColor &color, int width)
{
    int x1, y1, x2, y2;
    Complex firstCorner = center + radius*Complex(-1.0,1.0);
    Complex secondCorner = center + radius*Complex(1.0, - 1.0);
    ComplexToPixelCoordinates(x1, y1, firstCorner);
    ComplexToPixelCoordinates(x2, y2, secondCorner);
    pen->setColor(color);
    pen->setWidth(width);
    painter->setPen(*pen);
    painter->drawEllipse(x1, y1, x2 - x1, y2 - y1);
}

void CanvasDelegate::drawCircle(const Circle &C, const QColor &color, int width)
{
    Complex center;
    double radius;
    C.getCenterAndRadius(center, radius);
    drawCircle(center, radius, color, width);
}

bool CanvasDelegate::dealWithAlmostStraightArc(const Complex &center, double radius,
                                               const Complex &endpoint1, const Complex &endpoint2, const QColor &color, int width)
{
    double angleMin = M_PI/(180*16);
    double scale = scaleX > scaleY ? scaleX : scaleY;
    int pixelError = 8.0;
    Complex z1, z2;
    if(radius*scale*angleMin > pixelError)
    {
        intersectsCanvasBoundary(center, radius, endpoint1, endpoint2, z1, z2);
        drawSegment(z1,z2,color,width);
        return true;
    }
    else
    {
        return false;
    }
}

void CanvasDelegate::intersectsCanvasBoundary(const Complex &center, double radius,
                                              const Complex &endpoint1, const Complex &endpoint2, Complex &zOut1, Complex &zOut2)
{
    //std::cout << "Entering CanvasDelegate::intersectsCanvasBoundary" << std::endl;

    std::vector<Complex> intersectionsTemp, intersections;
    double xMax = xMin + sizeX/scaleX;
    double yMin = yMax - sizeY/scaleY;

    double c1 = real(center), c2 = imag(center);

    double x, y, D;


    x = xMin;
    D = radius*radius - (x-c1)*(x-c1);
    if (D > 0)
    {
        D = sqrt(D);
        y = c2 + D;
        if (y > yMin && y < yMax)
        {
            intersectionsTemp.push_back(Complex(x,y));
        }
        else
        {
            y = c2 - D;
            if (y > yMin && y < yMax)
            {
                intersectionsTemp.push_back(Complex(x,y));
            }
        }
    }

    x = xMax;
    D = radius*radius - (x-c1)*(x-c1);
    if (D > 0)
    {
        D = sqrt(D);
        y = c2 + D;
        if (y > yMin && y < yMax)
        {
            intersectionsTemp.push_back(Complex(x,y));
        }
        else
        {
            y = c2 - D;
            if (y > yMin && y < yMax)
            {
                intersectionsTemp.push_back(Complex(x,y));
            }
        }
    }

    y = yMin;
    D = radius*radius - (y-c2)*(y-c2);
    if (D > 0)
    {
        D = sqrt(D);
        x = c1 + D;
        if (x > xMin && x < xMax)
        {
            intersectionsTemp.push_back(Complex(x,y));
        }
        else
        {
            x = c1 - D;
            if (x > xMin && x < xMax)
            {
                intersectionsTemp.push_back(Complex(x,y));
            }
        }
    }

    y = yMax;
    D = radius*radius - (y-c2)*(y-c2);
    if (D > 0)
    {
        D = sqrt(D);
        x = c1 + D;
        if (x > xMin && x < xMax)
        {
            intersectionsTemp.push_back(Complex(x,y));
        }
        else
        {
            x = c1 - D;
            if (x > xMin && x < xMax)
            {
                intersectionsTemp.push_back(Complex(x,y));
            }
        }
    }

    Complex u = endpoint1 - center, v = endpoint2 - center, w, w2;

    for (const auto & interTemp : intersectionsTemp)
    {
        w = interTemp - center;
        if (imag(w*conj(u))*imag(w*conj(v)) < 0)
        {
            intersections.push_back(interTemp);
        }
    }


    switch (intersections.size())
    {
    case 0:
        zOut1 = endpoint1;
        zOut2 = endpoint2;
        break;

    case 1:
        if (real(endpoint1) > xMin && real(endpoint1) < xMax && imag(endpoint1) > yMin && imag(endpoint1) < yMax)
        {
            zOut1 = endpoint1;
            zOut2 = endpoint1 + 2.0 * (intersections[0] - endpoint1);
        }
        else if (real(endpoint2) > xMin && real(endpoint2) < xMax && imag(endpoint2) > yMin && imag(endpoint2) < yMax)
        {
            zOut1 = endpoint2 + 2.0 * (intersections[0] - endpoint2);
            zOut2 = endpoint2;
        }
        else
        {
            std::cout << "ERROR in CanvasDelegate::intersectsCanvasBoundary: interior endpoint not found" << std::endl;
        }
        break;

    case 2:
        w = intersections[0]-center;
        w2 = intersections[1]-center;
        if (imag(w2*conj(w))*imag(v*conj(u)) > 0)
        {
            zOut1 = intersections[1] + 2.0*(intersections[0]-intersections[1]);
            zOut2 = intersections[0] + 2.0*(intersections[1]-intersections[0]);
        }
        else
        {
            zOut1 = intersections[0] + 2.0*(intersections[1]-intersections[0]);
            zOut2 = intersections[1] + 2.0*(intersections[0]-intersections[1]);
        }
        break;

    default:
        std::cout << "ERROR in CanvasDelegate::intersectsCanvasBoundary: too many intersections!" << std::endl;
        break;
    }

    //std::cout << "Leaving CanvasDelegate::intersectsCanvasBoundary" << std::endl;
}

void CanvasDelegate::drawArcCounterClockwise(const Complex &center, double radius, double angle1, double angle2,
                                             const Complex &endpoint1, const Complex &endpoint2, const QColor &color, int width)
{
    if (dealWithAlmostStraightArc(center, radius, endpoint1, endpoint2, color, width))
    {

    }
    else
    {
        int x1, y1, x2, y2;
        Complex firstCorner = center + radius*Complex(-1.0, 1.0);
        Complex secondCorner = center + radius*Complex(1.0, -1.0);
        ComplexToPixelCoordinates(x1, y1, firstCorner);
        ComplexToPixelCoordinates(x2, y2, secondCorner);
        pen->setColor(color);
        pen->setWidth(width);
        painter->setPen(*pen);
        int QtAngle1 = Tools::intRound(Tools::mod2Pi(angle1)*16*360/(2*M_PI));
        int QtSpan = Tools::intRound(Tools::mod2Pi(angle2 - angle1)*16*360/(2*M_PI));
        painter->drawArc(x1, y1, x2 - x1, y2 - y1, QtAngle1, QtSpan);
    }
}

void CanvasDelegate::drawSmallerArc(const Complex &center, double radius, double angle1, double angle2,
                                    const Complex &endpoint1, const Complex &endpoint2, const QColor &color, int width)
{
    double d = angle2 - angle1;
    d = Tools::mod2Pi(d);
    if (d>M_PI)
    {
        drawArcCounterClockwise(center, radius, angle2, angle1, endpoint1, endpoint2, color, width);
    }
    else
    {
        drawArcCounterClockwise(center, radius, angle1, angle2, endpoint1, endpoint2, color, width);
    }
}

void CanvasDelegate::drawSmallerArc(const Circle &C, double angle1, double angle2,
                                    const Complex &endpoint1, const Complex &endpoint2, const QColor &color, int width)
{
    Complex center;
    double radius;
    C.getCenterAndRadius(center, radius);
    drawSmallerArc(center, radius, angle1, angle2, endpoint1, endpoint2, color, width);
}

void CanvasDelegate::drawArcCounterClockwise(const Circle &C, double angle1, double angle2,
                                             const Complex &endpoint1, const Complex &endpoint2, const QColor &color, int width)
{
    //std::cout << "Entering CanvasDelegate::drawArc" << std::endl;
    Complex center;
    double radius;
    C.getCenterAndRadius(center, radius);
    drawArcCounterClockwise(center, radius, angle1, angle2, endpoint1, endpoint2, color, width);
}
