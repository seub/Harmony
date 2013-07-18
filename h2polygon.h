#ifndef H2POLYGON_H
#define H2POLYGON_H

#include "tools.h"


class H2Point;
class H2GeodesicArc;
class H2Geodesic;
class H2Isometry;

class H2Polygon
{
    friend std::ostream & operator<<(std::ostream & out, const H2Polygon &P);
public:
    H2Polygon();
    void addVertex(const H2Point & p);
    void removeLastVertex();

    int getNumberOfVertices() const;
    H2Point getVertex(int index) const;
    std::vector<H2Point> getVertices() const;
    std::vector<complex> getVerticesInDiskModel() const;

    double norm0() const;
    double norm1() const;
    double norm2() const;
    double norm3() const;


    H2GeodesicArc getSide(int index) const;
    std::vector<H2GeodesicArc> getSides() const;
    std::vector<H2Geodesic> getCompletedSides() const;

    void optimalMobius(H2Isometry &output) const;

private:
    std::vector<H2Point> vertices;
};

#endif // H2POLYGON_H
