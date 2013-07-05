#include "sl2rmatrix.h"

SL2Rmatrix::SL2Rmatrix()
{
}


SL2Rmatrix::SL2Rmatrix(double a, double b, double c, double d) : a(a), b(b), c(c), d(d)
{
}

void SL2Rmatrix::getCoefficients(double &a, double &b, double &c, double &d) const
{
    a = this->a;
    b = this->b;
    c = this->c;
    d = this->d;
    return;
}

SL2Cmatrix SL2Rmatrix::complexCast() const
{
    return SL2Cmatrix(a, b, c, d);
}


H2point operator*(const SL2Rmatrix &A, const H2point &p)
{
    complex z = p.getUpperHalfPlaneCoordinate();
    double a, b, c, d;
    A.getCoefficients(a, b, c, d);
    complex Z = (a*z + b)/(c*z + d);
    H2point res;
    res.setUpperHalfPlaneCoordiante(Z);
    return res;
}
