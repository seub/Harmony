#include "h2polygon.h"
#include "h2point.h"
#include "h2geodesic.h"

H2Polygon::H2Polygon()
{
    vertices.clear();
    vertices.reserve(12);
}

void H2Polygon::addVertex(const H2Point &p)
{
    vertices.push_back(p);
    return;
}

void H2Polygon::removeLastVertex()
{
    vertices.pop_back();
    return;
}

int H2Polygon::getNumberOfVertices() const
{
    return vertices.size();
}

H2Point H2Polygon::getVertex(int index) const
{
    return vertices[index];
}

std::vector<H2Point> H2Polygon::getVertices() const
{
    return vertices;
}

std::vector<complex> H2Polygon::getVerticesInDiskModel() const
{
    int sizeList = getNumberOfVertices();
    std::vector<complex> complexList(sizeList);
    for(int j=0;j<=sizeList - 1;j++)
    {
        complexList[j] = vertices[j].getDiskCoordinate();
    }
    return complexList;
}

H2GeodesicArc H2Polygon::getSide(int index) const
{
    int sizeList = getNumberOfVertices();
    if(index < sizeList - 1)
    {
        return H2GeodesicArc(vertices[index],vertices[index + 1]);
    }
    else if(index == sizeList - 1)
    {
        return H2GeodesicArc(vertices[index],vertices[0]);
    }
    else
    {
        std::cout << "Whoa this polygon doesn't have that many sides!" << std::endl;
        throw(0);
    }
}

std::vector<H2GeodesicArc> H2Polygon::getSides() const
{
    int sizeList = getNumberOfVertices();
    std::vector<H2GeodesicArc> arcList(sizeList);
    for(int j=0;j<=sizeList - 1;j++)
    {
        arcList[j] = getSide(j);
    }
    return arcList;
}

std::vector<H2Geodesic> H2Polygon::getCompletedSides() const
{
    int sizeList = getNumberOfVertices();
    std::vector<H2Geodesic> arcList(sizeList);
    for(int j=0;j<=sizeList - 1;j++)
    {
        arcList[j] = getSide(j).getGeodesic();
    }
    return arcList;
}






