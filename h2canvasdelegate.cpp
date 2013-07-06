#include "h2canvasdelegate.h"
#include "h2point.h"
#include "h2geodesic.h"
#include "circle.h"
#include "planarline.h"

H2CanvasDelegate::H2CanvasDelegate(Canvas *canvas) : CanvasDelegate(canvas)
{
    //std::cout << "Entering H2CanvasDelegate::CanvasDelegate" << std::endl;

    //std::cout << "Leaving H2CanvasDelegate::CanvasDelegate" << std::endl;
}

void H2CanvasDelegate::drawH2Point(const H2Point &p, const QColor &color)
{
    complex z = p.getDiskCoordinate();
    drawPoint(z, color);
    return;
}

void H2CanvasDelegate::drawH2Geodesic(const H2Geodesic &L, const QColor &color)
{
    if (L.isCircleInDiskModel())
    {
        Circle C;
        double angle1, angle2;
        L.getCircleAndAnglesInDiskModel(C, angle1, angle2);
        drawArc(C, angle1, angle2);
    }
    else
    {
        complex z1, z2;
        L.getEndpointsInDiskModel(z1, z2);
        drawSegment(z1, z2, color);
    }
    return;
}
