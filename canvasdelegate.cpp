#include "canvasdelegate.h"

#include <QPen>
#include <QPainter>
#include <QImage>
#include <QColor>
#include <QMouseEvent>

#include "canvas.h"
#include "circle.h"
#include "tools.h"

CanvasDelegate::CanvasDelegate(int sizeX, int sizeY, ActionHandler *handler) : handler(handler)
{
    //std::cout << "Entering CanvasDelegate::CanvasDelegate" << std::endl;

    penBack = new QPen;
    penTop = new QPen;

    imageBack = new QImage(sizeX, sizeY, QImage::Format_RGB32);
    imageTop = new QImage(sizeX, sizeY, QImage::Format_ARGB32);

    painterBack = new QPainter(imageBack);
    painterBack->setRenderHint(QPainter::Antialiasing, true);
    painterBack->eraseRect(0, 0, sizeX, sizeY);
    painterBack->setPen(*penBack);

    painterTop = new QPainter(imageTop);
    painterTop->setRenderHint(QPainter::Antialiasing, true);
    painterTop->eraseRect(0, 0, sizeX, sizeY);
    imageTop->fill(qRgba(0, 0, 0, 0));
    painterTop->setPen(*penTop);

    enableRedrawBufferBack = false;
    enableRedrawBufferTop = false;

    detectionRadius = 20;
    resetView(sizeX, sizeY);

    //std::cout << "Leaving CanvasDelegate::CanvasDelegate" << std::endl;
}

CanvasDelegate::~CanvasDelegate()
{
    delete penBack;
    delete painterBack;
    delete imageBack;

    delete penTop;
    delete painterTop;
    delete imageTop;
}

const QImage *CanvasDelegate::getImageBack() const
{
    return imageBack;
}

const QImage *CanvasDelegate::getImageTop() const
{
    return imageTop;
}

void CanvasDelegate::rescale(int sizeX, int sizeY)
{
    delete painterBack;
    *imageBack = QImage(sizeX, sizeY, QImage::Format_RGB32);
    painterBack = new QPainter(imageBack);
    painterBack->setRenderHint(QPainter::Antialiasing, true);
    painterBack->eraseRect(0, 0, sizeX, sizeY);
    painterBack->setPen(*penBack);

    delete painterTop;
    *imageTop = QImage(sizeX, sizeY, QImage::Format_ARGB32);
    painterTop = new QPainter(imageTop);
    painterTop->setRenderHint(QPainter::Antialiasing, true);
    painterTop->eraseRect(0, 0, sizeX, sizeY);
    imageTop->fill(qRgba(0, 0, 0, 0));
    painterTop->setPen(*penTop);


    double xFactor = (sizeX *1.0/ this->sizeX);
    double yFactor = (sizeY *1.0/ this->sizeY);

    this->sizeX = sizeX;
    this->sizeY = sizeY;

    scaleX = xFactor * scaleX;
    scaleY = yFactor * scaleY;

    enableRedrawBuffer();
}

void CanvasDelegate::resetView(int sizeX, int sizeY)
{
    xMin = -1.1;
    yMax = 1.1;

    this->sizeX = sizeX;
    this->sizeY = sizeY;

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

    enableRedrawBuffer();
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

void CanvasDelegate::enableRedrawBuffer(bool back, bool top)
{
    enableRedrawBufferBack = back;
    enableRedrawBufferTop = top;
}

void CanvasDelegate::drawPoint(const Complex &z, const QColor &color, int width, bool back)
{
    int x, y;
    ComplexToPixelCoordinates(x, y, z);

    if (back)
    {
        penBack->setWidth(width);
        penBack->setColor(color);
        painterBack->setPen(*penBack);
        painterBack->drawPoint(x,y);
    }
    else
    {
        penTop->setWidth(width);
        penTop->setColor(color);
        painterTop->setPen(*penTop);
        painterTop->drawPoint(x,y);
    }
}

void CanvasDelegate::drawSegment(const Complex &endpoint1, const Complex &endpoint2, const QColor &color, int width, bool back)
{
    int x1, y1, x2, y2;
    ComplexToPixelCoordinates(x1, y1, endpoint1);
    ComplexToPixelCoordinates(x2, y2, endpoint2);

    if (back)
    {
        penBack->setWidth(width);
        penBack->setColor(color);
        painterBack->setPen(*penBack);
        painterBack->drawLine(x1, y1, x2, y2);
    }
    else
    {
        penTop->setWidth(width);
        penTop->setColor(color);
        painterTop->setPen(*penTop);
        painterTop->drawLine(x1, y1, x2, y2);
    }
}

void CanvasDelegate::drawCircle(const Complex &center, double radius, const QColor &color, int width, bool back)
{
    int x1, y1, x2, y2;
    Complex firstCorner = center + radius*Complex(-1.0,1.0);
    Complex secondCorner = center + radius*Complex(1.0, - 1.0);
    ComplexToPixelCoordinates(x1, y1, firstCorner);
    ComplexToPixelCoordinates(x2, y2, secondCorner);

    if (back)
    {
        penBack->setColor(color);
        penBack->setWidth(width);
        painterBack->setPen(*penBack);
        painterBack->drawEllipse(x1, y1, x2 - x1, y2 - y1);
    }
    else
    {
        penTop->setColor(color);
        penTop->setWidth(width);
        painterTop->setPen(*penTop);
        painterTop->drawEllipse(x1, y1, x2 - x1, y2 - y1);
    }
}

void CanvasDelegate::drawCircle(const Circle &C, const QColor &color, int width, bool back)
{
    Complex center;
    double radius;
    C.getCenterAndRadius(center, radius);
    drawCircle(center, radius, color, width, back);
}

bool CanvasDelegate::dealWithAlmostStraightArc(const Complex &center, double radius,
                                               const Complex &endpoint1, const Complex &endpoint2, const QColor &color, int width, bool back)
{
    double angleMin = M_PI/(180*16);
    double scale = scaleX > scaleY ? scaleX : scaleY;
    int pixelError = 8.0;
    Complex z1, z2;
    if(radius*scale*angleMin > pixelError)
    {
        intersectsCanvasBoundary(center, radius, endpoint1, endpoint2, z1, z2);
        drawSegment(z1, z2, color, width, back);
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
                                             const Complex &endpoint1, const Complex &endpoint2, const QColor &color, int width, bool back)
{
    if (dealWithAlmostStraightArc(center, radius, endpoint1, endpoint2, color, width, back))
    {
        ++nbAlmostStraightArcs;
    }
    else
    {
        int x1, y1, x2, y2;
        Complex firstCorner = center + radius*Complex(-1.0, 1.0);
        Complex secondCorner = center + radius*Complex(1.0, -1.0);
        ComplexToPixelCoordinates(x1, y1, firstCorner);
        ComplexToPixelCoordinates(x2, y2, secondCorner);
        int QtAngle1 = Tools::intRound(Tools::mod2Pi(angle1)*16*360/(2*M_PI));
        int QtSpan = Tools::intRound(Tools::mod2Pi(angle2 - angle1)*16*360/(2*M_PI));

        if (back)
        {
            penBack->setColor(color);
            penBack->setWidth(width);
            painterBack->setPen(*penBack);
            painterBack->drawArc(x1, y1, x2 - x1, y2 - y1, QtAngle1, QtSpan);
        }
        else
        {
            penTop->setColor(color);
            penTop->setWidth(width);
            painterTop->setPen(*penTop);
            painterTop->drawArc(x1, y1, x2 - x1, y2 - y1, QtAngle1, QtSpan);
        }
    }
}

void CanvasDelegate::drawSmallerArc(const Complex &center, double radius, double angle1, double angle2,
                                    const Complex &endpoint1, const Complex &endpoint2, const QColor &color, int width, bool back)
{
    double d = angle2 - angle1;
    d = Tools::mod2Pi(d);
    if (d>M_PI)
    {
        drawArcCounterClockwise(center, radius, angle2, angle1, endpoint1, endpoint2, color, width, back);
    }
    else
    {
        drawArcCounterClockwise(center, radius, angle1, angle2, endpoint1, endpoint2, color, width, back);
    }
}

void CanvasDelegate::drawSmallerArc(const Circle &C, double angle1, double angle2,
                                    const Complex &endpoint1, const Complex &endpoint2, const QColor &color, int width, bool back)
{
    Complex center;
    double radius;
    C.getCenterAndRadius(center, radius);
    drawSmallerArc(center, radius, angle1, angle2, endpoint1, endpoint2, color, width, back);
}

void CanvasDelegate::drawArcCounterClockwise(const Circle &C, double angle1, double angle2,
                                             const Complex &endpoint1, const Complex &endpoint2, const QColor &color, int width, bool back)
{
    //std::cout << "Entering CanvasDelegate::drawArc" << std::endl;
    Complex center;
    double radius;
    C.getCenterAndRadius(center, radius);
    drawArcCounterClockwise(center, radius, angle1, angle2, endpoint1, endpoint2, color, width, back);
}
