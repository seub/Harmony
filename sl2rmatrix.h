#ifndef SL2RMATRIX_H
#define SL2RMATRIX_H

#include "types.h"
#include "h2point.h"
#include "sl2cmatrix.h"

class SL2Rmatrix
{
    friend H2point operator*(const SL2Rmatrix &A, const H2point &p);

public:
    SL2Rmatrix();
    SL2Rmatrix(double a, double b, double c, double d);

    SL2Cmatrix complexCast() const;
    void getCoefficients(double &a, double &b, double &c, double &d) const;


private:
    double a, b, c, d;
};

#endif // SL2RMATRIX_H
