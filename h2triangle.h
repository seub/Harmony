#ifndef H2TRIANGLE_H
#define H2TRIANGLE_H
#include "types.h"
#include "h2point.h"

class H2Triangle
{
public:
    H2Triangle();
    H2Triangle(const H2Point & a, const H2Point & b, const H2Point & c);

    void setPoints(const H2Point &a, const H2Point &b, const H2Point &c);

    void getPoints(H2Point &a, H2Point &b, H2Point &c) const;
    void getSideLengths(double & A, double & B, double & C) const;
    void getAngles(double & angA, double & angleB, double & angleC) const;

    std::vector<H2Triangle> triangulate() const;


private:
    H2Point a,b,c;
};

std::vector<H2Triangle> triangulate(const std::vector<H2Triangle> & triangleList);

#endif // H2TRIANGLE_H
