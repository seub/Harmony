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
    void clearVertices();
    void setVertices(const std::vector<H2Point> & newVertices);

    uint nbVertices() const;
    H2Point getVertex(uint index) const;
    std::vector<H2Point> getVertices() const;
    std::vector<Complex> getVerticesInDiskModel() const;
    std::vector<Complex> getVerticesInKleinModel() const;

    void getExtremalCoordinatesInDiskModel(double& xMin, double& xMax, double& yMin, double & yMax) const;

    bool isPositivelyOriented() const;

    double norm0() const;
    double norm1() const;
    double norm2() const;
    double norm3() const;
    double norm4() const;

    bool containsInKleinModel(const Complex & z) const;
    bool constainsInDiskModel(const Complex & z) const;
    bool contains(const H2Point & point) const;
    bool isConvex() const;

    H2GeodesicArc getSide(uint index) const;
    std::vector<H2GeodesicArc> getSides() const;
    std::vector<H2Geodesic> getCompletedSides() const;

    void optimalMobius(H2Isometry &output) const;
    std::vector<double> getAngles() const;
    std::vector<double> getPositiveInteriorAngles() const;

    void insertMidpoints();

protected:
    std::vector<H2Point> vertices;
};

class H2SteinerPolygon : H2Polygon
{

public:
    H2SteinerPolygon() {}
    H2SteinerPolygon(std::vector<H2Point> vertices, std::vector<uint> nbSteinerPoints);

    std::vector<H2Point> getPointsOnSide(uint side) const;
    H2Polygon getFullPolygon() const;
    std::vector<uint> getVectorNbSteinerPoints() const;
    uint getTotalNbSteinerPoints() const;
    uint getNbSteinerPointsOnSide(uint side) const;
    uint getIndexOfFullVertex(uint vertexIndex) const;
    uint getActualSide(uint vertexInFullPolygon) const;
    bool lieOnSameActualSide(uint vertexInFullPolygon1, uint vertexInFullPolygon2) const;

private:
    std::vector<uint> nbSteinerPoints;

};

#endif // H2POLYGON_H
