#include "h2triangle.h"
#include "h2polygon.h"

H2Triangle::H2Triangle()
{
}

H2Triangle::H2Triangle(const H2Point &a, const H2Point &b, const H2Point &c): a(a), b(b), c(c)
{
}

H2Triangle::H2Triangle(double p, double q, double r)
{
    double angA = M_PI / p;
    double angB = M_PI / q;
    double angC = M_PI / r;
    double cosSum = cos(.5*(angA + angB + angC));
    double cosSumA = cos(.5*(-angA + angB + angC));
    double cosSumB = cos(.5*(angA - angB + angC));
    double cosSumC = cos(.5*(angA + angB - angC));

    a.setDiskCoordinate(0.0);
    b.setDiskCoordinate(sqrt(cosSum*cosSumC / (cosSumA*cosSumB)));
    c.setDiskCoordinate((cos(angA) + I*sin(angA))*sqrt(cosSum*cosSumB / (cosSumA*cosSumC)));
    return;
}

void H2Triangle::setPoints(const H2Point &a, const H2Point &b, const H2Point &c)
{
    this->a = a;
    this->b = b;
    this->c = c;
    return;
}

void H2Triangle::getPoints(H2Point &a, H2Point &b, H2Point &c) const
{
    a = this->a;
    b = this->b;
    c = this->c;
    return;
}

bool H2Triangle::isInside(const H2Point &p) const
{
    H2Polygon P;
    P.addVertex(a);
    P.addVertex(b);
    P.addVertex(c);
    return P.isInside(p);
}

void H2Triangle::getSideLengths(double &A, double &B, double &C) const
{
    A = H2Point::distance(b,c);
    B = H2Point::distance(a,c);
    C = H2Point::distance(a,b);
    return;
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

std::vector<H2Triangle> H2Triangle::triangulate() const
{
    H2Point midA, midB, midC;
    midA = H2Point::midpoint(b,c);
    midB = H2Point::midpoint(a,c);
    midC = H2Point::midpoint(a,b);
    H2Triangle Ta(a,midB,midC);
    H2Triangle Tb(b,midA,midC);
    H2Triangle Tc(c,midA,midB);
    H2Triangle Tmid(midA,midB,midC);
    std::vector<H2Triangle> output;
    output.push_back(Ta);
    output.push_back(Tb);
    output.push_back(Tc);
    output.push_back(Tmid);
    return output;
}

std::vector<H2Triangle> triangulate(const std::vector<H2Triangle> &triangleList)
{
    std::vector<H2Triangle> output;
    std::vector<H2Triangle> temp;
    for (unsigned int j=0; j<triangleList.size(); j++)
    {
        temp = triangleList[j].triangulate();
        output.insert(output.end(),temp.begin(),temp.end());
    }
    return output;
}
