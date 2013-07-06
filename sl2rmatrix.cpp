#include "sl2rmatrix.h"

SL2RMatrix::SL2RMatrix()
{
}


SL2RMatrix::SL2RMatrix(double a, double b, double c, double d) : a(a), b(b), c(c), d(d)
{
}

void SL2RMatrix::getCoefficients(double &a, double &b, double &c, double &d) const
{
    a = this->a;
    b = this->b;
    c = this->c;
    d = this->d;
    return;
}

SL2CMatrix SL2RMatrix::complexCast() const
{
    return SL2CMatrix(a, b, c, d);
}


H2Point operator*(const SL2RMatrix &A, const H2Point &p)
{
    complex z = p.getUpperHalfPlaneCoordinate();
    double a, b, c, d;
    A.getCoefficients(a, b, c, d);
    complex Z = (a*z + b)/(c*z + d);
    H2Point res;
    res.setUpperHalfPlaneCoordiante(Z);
    return res;
}
