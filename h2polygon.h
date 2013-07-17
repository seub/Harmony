#ifndef H2POLYGON_H
#define H2POLYGON_H
#include "tools.h"

class H2Point;
class H2GeodesicArc;
class H2Geodesic;

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
    std::vector<double> getEuclideanSideLengths() const;

    double normReciprocalSmallestEuclideanSideLength() const;
    double normReciprocalEuclideanSideLengthsL1() const;
    double normReciprocalEuclideanSideLengthsL2() const;
    double normLargestEuclideanSideLength() const;
    double normFurthestEuclideanDistanceFromOrigin() const;

    H2GeodesicArc getSide(int index) const;
    std::vector<H2GeodesicArc> getSides() const;
    std::vector<H2Geodesic> getCompletedSides() const;

private:
    std::vector<H2Point> vertices;
};

#endif // H2POLYGON_H
