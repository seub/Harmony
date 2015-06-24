#include "canvasdelegate.h"

#include <QPen>
#include <QPainter>
#include <QImage>
#include <QColor>
#include <QMouseEvent>
#include <QBrush>

#include "canvas.h"
#include "circle.h"
#include "tools.h"

CanvasDelegate::CanvasDelegate(uint sizeX, uint sizeY, ActionHandler *handler) : handler(handler)
{
    penBack = new QPen;
    penTop = new QPen;
    resetPenBack = true;

    imageBack = new QImage(sizeX, sizeY, QImage::Format_RGB32);
    imageTop = new QImage(sizeX, sizeY, QImage::Format_ARGB32);

    painterBack = new QPainter(imageBack);
    painterBack->setRenderHint(QPainter::Antialiasing, true);
    painterBack->eraseRect(0, 0, sizeX, sizeY);
    painterBack->setPen(*penBack);
    painterBack->setClipRect(0, 0, sizeX, sizeY);

    painterTop = new QPainter(imageTop);
    painterTop->setRenderHint(QPainter::Antialiasing, true);
    painterTop->eraseRect(0, 0, sizeX, sizeY);
    imageTop->fill(qRgba(0, 0, 0, 0));
    painterTop->setPen(*penTop);
    painterTop->setClipRect(0, 0, sizeX, sizeY);

    enableRedrawBufferBack = false;
    enableRedrawBufferTop = false;

    detectionRadius = 20;
    resetView(sizeX, sizeY);
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

void CanvasDelegate::rescale(uint sizeX, uint sizeY)
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

void CanvasDelegate::resetView(uint sizeX, uint sizeY)
{
    xMin = -1.1;
    yMax = 1.1;

    this->sizeX = sizeX;
    this->sizeY = sizeY;

    scaleX = sizeX/2.2;
    scaleY = sizeY/2.2;

    subResetView();
    enableRedrawBuffer();
}

void CanvasDelegate::resetView()
{
    resetView(sizeX, sizeY);
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
    xMin = xMinSave + (mouseX - x)/scaleX;
    yMax = yMaxSave - (mouseY - y)/scaleY;
    enableRedrawBuffer();
}

void CanvasDelegate::shift(int x, int y)
{
    xMin += x/scaleX;
    yMax += y/scaleY;
    enableRedrawBuffer();
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
        if (resetPenBack)
        {
            penBack->setWidth(width);
            penBack->setColor(color);
            painterBack->setPen(*penBack);
        }
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
        if (resetPenBack)
        {
            penBack->setWidth(width);
            penBack->setColor(color);
            painterBack->setPen(*penBack);
        }
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

bool CanvasDelegate::dealWithExceptionalArc(const Complex &center, double radius,
                                            const Complex &endpoint1, const Complex &endpoint2, const QColor &color, int width, bool back)
{
    Complex newEndpoint1, newEndpoint2;
    bool noBestApproximation, arcOutsideCanvas, failed;

    bestLineSegmentApproximationOfSmallerArc(center, radius, endpoint1, endpoint2, newEndpoint1, newEndpoint2, noBestApproximation, arcOutsideCanvas, failed);

    if (failed)
    {
        // *** For testing ***
        drawCircle(center, radius, "green", 1, back);
        return true;
        // *** After testing replace above by next line ***
        // return false;
    }
    else if (noBestApproximation)
    {
        return false;
    }
    else if (arcOutsideCanvas)
    {
        return true;
    }
    else if (isAlmostSmallStraightArc(center, radius, endpoint1, endpoint2))
    {
        drawSegment(endpoint1, endpoint2, color, width, back);
        return true;
    }
    else if (isAlmostInfiniteRadius(radius))
    {
        drawSegment(newEndpoint1, newEndpoint2, color, width, back);
        return true;
    }
    else
    {
        return false;
    }

}

bool CanvasDelegate::isAlmostInfiniteRadius(double radius) const
{
    double angleMin = M_PI/(180*16);
    double scale = scaleX > scaleY ? scaleX : scaleY;
    int pixelError = 8.0;
    return radius*scale*angleMin > pixelError;
}

bool CanvasDelegate::isAlmostSmallStraightArc(const Complex &center, double radius, const Complex &endpoint1, const Complex &endpoint2) const
{
    Complex endpoint2nor = (endpoint2 - center)*conj(endpoint1 - center)/(radius*radius);
    double X2 = real(endpoint2nor), Y2 = imag(endpoint2nor);

    double arcMidpointX = sqrt(0.5*(1.0 + X2)), arcMidpointY = sqrt(0.5*(1.0 - X2));
    arcMidpointY = (Y2 > 0) ? arcMidpointY : -arcMidpointY;

    Complex arcMidpoint(arcMidpointX, arcMidpointY);
    Complex lineMidpoint = 0.5*(1.0 + endpoint2nor);

    double scale = scaleX > scaleY ? scaleX : scaleY;
    int pixelDeltaSquared = Tools::intRound(norm(arcMidpoint - lineMidpoint)*radius*radius*scale*scale);
    int pixelTolSquared = 0;

    return pixelDeltaSquared <= pixelTolSquared;
}

bool CanvasDelegate::isInside(const Complex &point) const
{
    double x = real(point), y = imag(point);

    return (x >= xMin) && (x <= xMax()) && (y >= yMin()) && (y <= yMax);
}

double CanvasDelegate::xMax() const
{
    return xMin + sizeX/scaleX;
}

double CanvasDelegate::yMin() const
{
    return yMax - sizeY/scaleY;
}

bool CanvasDelegate::liesOnSmallerArc(const Complex &point, const Complex &center, const Complex &endpoint1, const Complex &endpoint2)
{
    Complex u1 = endpoint1 - center, u2 = endpoint2 - center, u = point - center;

    Complex v = u*conj(u1), v2 = u2*conj(u1);

    return (imag(v2) > 0) ? ((imag(v) >= 0) && (real(v) >= real(v2))) : ((imag(v) <= 0) && (real(v) >= real(v2)));
}

void CanvasDelegate::bestLineSegmentApproximationOfSmallerArc(const Complex &center, double radius, const Complex &endpoint1, const Complex &endpoint2,
                                                              Complex &newEndpoint1, Complex &newEndpoint2, bool &noBestApproximation, bool &arcOutsideCanvas, bool &failed)
{
    ++nbBestLineCalls;
    noBestApproximation = false;
    arcOutsideCanvas = false;
    failed = false;

    newEndpoint1 = endpoint1;
    newEndpoint2 = endpoint2;

    bool isInside1 = isInside(endpoint1);
    bool isInside2 = isInside(endpoint2);

    if (isInside1 && isInside2)
    {
        return;
    }

    std::vector<Complex> intersections;
    Complex intersection1, intersection2;
    circleIntersectsCanvasBoundary(center, radius, intersections);
    uint nbIntersections = intersections.size();

    if (nbIntersections == 0)
    {
        if ((!isInside1) && (!isInside2))
        {
            arcOutsideCanvas = true;
            return;
        }
        else
        {
            qDebug() << "Warning in CanvasDelegate::bestLineSegmentApproximationOfArc: "
                     << "exactly one of the two endpoints is in the canvas, but the circle does not intersect the canvas boundary?!";
            ++nbBestLineFails;
            failed = true;
            return;
        }
    }
    else if (nbIntersections == 2)
    {
        intersection1 = intersections[0];
        intersection2 = intersections[1];
        if (isInside1 && (!isInside2))
        {
            if (liesOnSmallerArc(intersection1, center, endpoint1, endpoint2))
            {
                newEndpoint2 = intersection1;
                return;
            }
            else if (liesOnSmallerArc(intersection2, center, endpoint1, endpoint2))
            {
                newEndpoint2 = intersection2;
                return;
            }
            else
            {
                qDebug() << "Warning in CanvasDelegate::bestLineSegmentApproximationOfArc: "
                         << "exactly one of the two endpoints is in the canvas, circle intersects boundary twice, "
                         << "but neither intersections lie on arc?!";
                ++nbBestLineFails;
                failed = true;
                return;
            }
        }
        else if (isInside2 && (!isInside1))
        {
            if (liesOnSmallerArc(intersection1, center, endpoint2, endpoint1))
            {
                newEndpoint1 = intersection1;
                return;
            }
            else if (liesOnSmallerArc(intersection2, center, endpoint2, endpoint1))
            {
                newEndpoint1 = intersection2;
                return;
            }
            else
            {
                qDebug() << "Warning in CanvasDelegate::bestLineSegmentApproximationOfArc: "
                         << "exactly one of the two endpoints is in the canvas, circle intersects boundary twice, "
                         << "but neither intersections lie on arc?!";
                ++nbBestLineFails;
                failed = true;
                return;
            }
        }
        else
        {
            if (liesOnSmallerArc(intersection1, center, endpoint1, endpoint2) && liesOnSmallerArc(intersection2, center, endpoint1, endpoint2))
            {

                if (liesOnSmallerArc(intersection1, center, endpoint1, intersection2))
                {
                    newEndpoint1 = intersection1;
                    newEndpoint2 = intersection2;
                    return;
                }
                else if (liesOnSmallerArc(intersection2, center, endpoint1, intersection1))
                {
                    newEndpoint1 = intersection2;
                    newEndpoint2 = intersection1;
                    return;
                }
                else
                {
                    qDebug() << "Warning in CanvasDelegate::bestLineSegmentApproximationOfArc: "
                             << "unexpected situation that I can't be bothered describing";
                    ++nbBestLineFails;
                    failed = true;
                    return;
                }
            }
            else
            {
                arcOutsideCanvas = true;
                return;
            }
        }
    }
    else if ((nbIntersections % 2) == 1)
    {
        qDebug() << "Warning in CanvasDelegate::bestLineSegmentApproximationOfArc: "
                 << "Circle and Canvas boundary have an odd number of intersections?!";
        failed = true;
        ++nbBestLineFails;
        return;
    }
    else
    {
        noBestApproximation = true;
        return;
    }
}


bool CanvasDelegate::circleIntersectsCanvasBoundary(const Complex &center, double radius, std::vector<Complex> &intersectionsOut) const
{
    intersectionsOut.clear();

    double xMax = xMin + sizeX/scaleX;
    double yMin = yMax - sizeY/scaleY;

    double c1 = real(center), c2 = imag(center);

    double x, y, D;


    x = xMin;
    D = radius*radius - (x-c1)*(x-c1);
    if (D >= 0)
    {
        D = sqrt(D);
        y = c2 + D;
        if (y >= yMin && y <= yMax)
        {
            intersectionsOut.push_back(Complex(x,y));
        }
        y = c2 - D;
        if (y >= yMin && y <= yMax)
        {
            intersectionsOut.push_back(Complex(x,y));
        }
    }

    x = xMax;
    D = radius*radius - (x-c1)*(x-c1);
    if (D >= 0)
    {
        D = sqrt(D);
        y = c2 + D;
        if (y >= yMin && y <= yMax)
        {
            intersectionsOut.push_back(Complex(x,y));
        }
        y = c2 - D;
        if (y >= yMin && y <= yMax)
        {
            intersectionsOut.push_back(Complex(x,y));
        }
    }

    y = yMin;
    D = radius*radius - (y-c2)*(y-c2);
    if (D >= 0)
    {
        D = sqrt(D);
        x = c1 + D;
        if (x >= xMin && x <= xMax)
        {
            intersectionsOut.push_back(Complex(x,y));
        }
        x = c1 - D;
        if (x >= xMin && x <= xMax)
        {
            intersectionsOut.push_back(Complex(x,y));
        }
    }

    y = yMax;
    D = radius*radius - (y-c2)*(y-c2);
    if (D >= 0)
    {
        D = sqrt(D);
        x = c1 + D;
        if (x >= xMin && x <= xMax)
        {
            intersectionsOut.push_back(Complex(x,y));
        }
        x = c1 - D;
        if (x >= xMin && x <= xMax)
        {
            intersectionsOut.push_back(Complex(x,y));
        }
    }

    return (intersectionsOut.size() != 0);
}


void CanvasDelegate::drawSmallerArc(const Complex &center, double radius, const Complex &endpoint1, const Complex &endpoint2, const QColor &color, int width, bool back)
{
    if (dealWithExceptionalArc(center, radius, endpoint1, endpoint2, color, width, back))
    {
        ++nbAlmostStraightArcs;
    }
    else
    {
        int x1, y1, x2, y2;
        double angle1 = arg(endpoint1 - center), angle2 = arg(endpoint2 - center);
        if (imag((endpoint2-center)*conj(endpoint1-center)) < 0)
        {
            std::swap(angle1, angle2);
        }
        Complex firstCorner = center + radius*Complex(-1.0, 1.0);
        Complex secondCorner = center + radius*Complex(1.0, -1.0);
        ComplexToPixelCoordinates(x1, y1, firstCorner);
        ComplexToPixelCoordinates(x2, y2, secondCorner);
        int QtAngle = Tools::intRound(Tools::mod2Pi(angle1)*16*360/(2*M_PI));
        int QtSpan = Tools::intRound(Tools::mod2Pi(angle2 - angle1)*16*360/(2*M_PI));

        if (back)
        {
            if (resetPenBack)
            {
                penBack->setColor(color);
                penBack->setWidth(width);
                painterBack->setPen(*penBack);
            }
            painterBack->drawArc(x1, y1, x2 - x1, y2 - y1, QtAngle, QtSpan);
        }
        else
        {
            penTop->setColor(color);
            penTop->setWidth(width);
            painterTop->setPen(*penTop);
            painterTop->drawArc(x1, y1, x2 - x1, y2 - y1, QtAngle, QtSpan);
        }
    }
}
