#ifndef H2POLYGON_H
#define H2POLYGON_H

#include "tools.h"
#include "h2point.h"
#include "h2triangle.h"

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
    void clearVertices();

    int getNumberOfVertices() const;
    H2Point getVertex(int index) const;
    std::vector<H2Point> getVertices() const;
    std::vector<complex> getVerticesInDiskModel() const;
    std::vector<complex> getVerticesInKleinModel() const;
    std::vector<complex> getVerticesInHyperboloidProjection() const;

    void getExtremalCoordinatesInDiskModel(double& xMin, double& xMax, double& yMin, double & yMax) const;
    void getExtremalCoordinatesInHyperboloidProjection(double & xMin, double & xMax, double & yMin, double & yMax) const;

    bool isPositivelyOriented() const;

    double norm0() const;
    double norm1() const;
    double norm2() const;
    double norm3() const;
    double norm4() const;

    bool containsInKleinModel(const complex & z) const;
    bool constainsInDiskModel(const complex & z) const;
    bool contains(const H2Point & point) const;

    H2GeodesicArc getSide(int index) const;
    std::vector<H2GeodesicArc> getSides() const;
    std::vector<H2Geodesic> getCompletedSides() const;

    void optimalMobius(H2Isometry &output) const;
    std::vector<double> getAngles() const;
    std::vector<double> getPositiveInteriorAngles() const;

private:
    std::vector<H2Point> vertices;
    std::vector<complex> verticesInKleinModel;
};

#endif // H2POLYGON_H
