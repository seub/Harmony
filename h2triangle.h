#ifndef H2TRIANGLE_H
#define H2TRIANGLE_H
#include "types.h"
#include "h2point.h"

class H2Polygon;

class H2Triangle
{
    friend std::ostream & operator<<(std::ostream &out, const H2Triangle &T);
    friend class H2TriangleSubdivision;

public:
    H2Triangle();
    H2Triangle(const H2Point & a, const H2Point & b, const H2Point & c);

    void getPoints(H2Point &a, H2Point &b, H2Point &c) const;
    void getSideLengths(double & A, double & B, double & C) const;
    void getAngles(double & angA, double & angleB, double & angleC) const;

    bool contains(const H2Point & point) const;
    void getAsPolygon(H2Polygon & outputPolygon) const;


private:
    H2Point a, b, c;
};

#endif // H2TRIANGLE_H
