#include "h2triangle.h"
#include "h2polygon.h"


H2Triangle::H2Triangle()
{
}

H2Triangle::H2Triangle(const H2Point &a, const H2Point &b, const H2Point &c): a(a), b(b), c(c)
{
}

void H2Triangle::getPoints(H2Point &a, H2Point &b, H2Point &c) const
{
    a = this->a;
    b = this->b;
    c = this->c;
    return;
}

void H2Triangle::getSideLengths(double &A, double &B, double &C) const
{
    A = H2Point::distance(b,c);
    B = H2Point::distance(a,c);
    C = H2Point::distance(a,b);
    return;
}

bool H2Triangle::contains(const H2Point &point) const
{
    H2Polygon P;
    getAsPolygon(P);
    return P.contains(point);
}

void H2Triangle::getAsPolygon(H2Polygon & outputPolygon) const
{
    outputPolygon.clearVertices();
    outputPolygon.addVertex(a);
    outputPolygon.addVertex(b);
    outputPolygon.addVertex(c);
}

void H2Triangle::getAngles(double &angleA, double &angleB, double &angleC) const
{
    complex z1 = a.getDiskCoordinate();
    complex z2 = b.getDiskCoordinate();
    complex z3 = c.getDiskCoordinate();

    complex u,v;

    u = (z3 - z1) / (1.0 - conj(z1)*z3);
    v = (z2 - z1) / (1.0 - conj(z1)*z2);
    angleA = Tools::mod2Pi(arg(v*conj(u)));

    u = (z1 - z2) / (1.0 - conj(z2)*z1);
    v = (z3 - z2) / (1.0 - conj(z2)*z3);
    angleB = Tools::mod2Pi(arg(v*conj(u)));

    u = (z2 - z3) / (1.0 - conj(z3)*z2);
    v = (z1 - z3) / (1.0 - conj(z3)*z1);
    angleC = Tools::mod2Pi(arg(v*conj(u)));


    return;
}


std::ostream & operator<<(std::ostream &out, const H2Triangle &T)
{
    out << "Triangle with vertices a = " << T.a << ", b = " << T.b << ", c = " << T.c << std::endl;
    return out;
}