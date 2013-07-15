#include "h2buffer.h"

H2Buffer::H2Buffer()
{
}

void H2Buffer::reset()
{
    points.clear();
    pointsColors.clear();
    pointsWidths.clear();

    geodesics.clear();
    geodesicsColors.clear();
    geodesicsWidths.clear();
    return;
}

void H2Buffer::addElement(const H2Point &point, const QColor &color, int width)
{
    points.push_back(point);
    pointsColors.push_back(color);
    pointsWidths.push_back(width);
    return;
}

void H2Buffer::addElement(const H2Geodesic &geodesic, const QColor &color, int width)
{
    geodesics.push_back(geodesic);
    geodesicsColors.push_back(color);
    geodesicsWidths.push_back(width);
    return;
}

void H2Buffer::addElement(const H2Polygon &polygon, const QColor &color, int width)
{
    polygons.push_back(polygon);
    polygonsColors.push_back(color);
    polygonsWidths.push_back(width);
    return;
}

void H2Buffer::addElement(const H2Isometry &isometry, const QColor &color, int width)
{
    H2Geodesic L;
    if (isometry.axis(L))
    {
        geodesics.push_back(L);
        geodesicsColors.push_back(color);
        geodesicsWidths.push_back(width);
    }
    return;
}



void H2Buffer::addElement(const IsomH2Representation &rho, const QColor &color, int width)
{
    std::vector<H2Isometry> generatorImages = rho.getGeneratorImages();
    for (unsigned int i=0; i<generatorImages.size(); i++)
    {
        addElement(generatorImages[i], color, width);
    }
    return;
}

void H2Buffer::addElement(const std::vector<H2Isometry> V, const QColor &color, int width)
{
    for (unsigned int i=0; i<V.size(); i++)
    {
        addElement(V[i], color, width);
    }
    return;
}
