#ifndef H2TRIANGLE_H
#define H2TRIANGLE_H

#include "types.h"
#include "h2point.h"

class H2Polygon; class H2GeodesicArc; class H2Isometry;

class H2Triangle
{
    friend std::ostream & operator<<(std::ostream &out, const H2Triangle &T);
    friend H2Triangle operator*(const H2Isometry &f, const H2Triangle &T);
    friend class H2TriangleSubdivision;

public:
    H2Triangle();
    H2Triangle(const H2Point & a, const H2Point & b, const H2Point & c);

    void getPoints(H2Point &a, H2Point &b, H2Point &c) const;
    std::vector<H2Point> getPoints() const;
    std::vector<H2GeodesicArc> getSides() const;
    double area() const;


    H2Point getVertex(uint index) const;
    void getVertices(H2Point &Aout, H2Point &Bout, H2Point &Cout) const;
    void getSideLengths(double & A, double & B, double & C) const;
    void getAngles(double & angA, double & angleB, double & angleC) const;

    bool contains(const H2Point & point) const;
    void getAsPolygon(H2Polygon & outputPolygon) const;
    bool isVertexCloseInDiskModel(const H2Point &point, double detectionRadiusSquared, uint &vertexIndex) const;


private:
    H2Point a, b, c;
};

#endif // H2TRIANGLE_H
