#ifndef H2POLYGON_H
#define H2POLYGON_H

#include "tools.h"
#include "h2point.h"
#include "h2triangle.h"

class H2GeodesicArc; class H2Geodesic; class H2Isometry;
template <typename T> class GroupRepresentation;

class H2Polygon
{
    friend std::ostream & operator<<(std::ostream & out, const H2Polygon &P);
    friend class GroupRepresentation<H2Isometry>;
    friend class H2MeshFunction;

public:
    H2Polygon();
    H2Polygon(std::vector<H2Point> vertices);

    void addVertex(const H2Point & p);
    void removeLastVertex();
    void setVertices(const std::vector<H2Point> & newVertices);

    uint nbVertices() const;
    H2Point getVertex(uint index) const;
    std::vector<H2Point> getVertices() const;

    bool isPositivelyOriented() const;
    bool contains(const H2Point & point) const;
    bool isConvex() const;
    double diameter() const;

    std::vector<double> getInteriorAngles() const;
    double smallestAngle() const;

    H2GeodesicArc getSide(uint index) const;
    std::vector<H2GeodesicArc> getSides() const;
    std::vector<H2Geodesic> getCompletedSides() const;

    void optimalMobius(H2Isometry &output) const;

protected:
    std::vector<H2Point> vertices;

private:
    bool containsInKleinModel(const Complex & z) const;
    std::vector<Complex> getVerticesInKleinModel() const;
};

class H2SteinerPolygon : H2Polygon
{

public:
    H2SteinerPolygon() {}
    H2SteinerPolygon(std::vector<H2Point> vertices, std::vector<uint> steinerWeights);

    std::vector<H2Point> getPointsOnSide(uint side) const;
    H2Polygon getFullPolygon() const;
    uint getTotalNbSteinerPoints() const;
    uint getTotalNbPoints() const;
    uint getNbSteinerPointsOnSide(uint side) const;
    uint getIndexOfFullVertex(uint vertexIndex) const;
    bool lieOnSameActualSide(uint vertexInFullPolygon1, uint vertexInFullPolygon2) const;

private:
    std::vector<uint> steinerWeights;

};

#endif // H2POLYGON_H
