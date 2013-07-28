#include "h2triangle.h"

H2Triangle::H2Triangle()
{
}

H2Triangle::H2Triangle(const H2Point &a, const H2Point &b, const H2Point &c): a(a), b(b), c(c)
{
}

void H2Triangle::setPoints(H2Point &a, H2Point &b, H2Point &c)
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

void H2Triangle::getSideLengths(double &A, double &B, double &C) const
{
    A = H2distance(b,c);
    B = H2distance(a,c);
    C = H2distance(a,b);
    return;
}

void H2Triangle::getAngles(double &angA, double &angB, double &angC) const
{
    double A,B,C;
    getSideLengths(A,B,C);
    angA = acos((cosh(B)*cosh(C) - cosh(A))/(sinh(B)*sinh(C)));
    angB = acos((cosh(A)*cosh(C) - cosh(B))/(sinh(A)*sinh(C)));
    angC = acos((cosh(B)*cosh(A) - cosh(C))/(sinh(B)*sinh(A)));
    return;
}

std::vector<H2Triangle> H2Triangle::triangulate() const
{
    H2Point midA, midB, midC;
    midA = H2Midpoint(b,c);
    midB = H2Midpoint(a,c);
    midC = H2Midpoint(a,b);
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
