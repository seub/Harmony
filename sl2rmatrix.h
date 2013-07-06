#ifndef SL2RMATRIX_H
#define SL2RMATRIX_H

#include "types.h"
#include "h2point.h"
#include "sl2cmatrix.h"

class SL2RMatrix
{
    friend H2Point operator*(const SL2RMatrix &A, const H2Point &p);

public:
    SL2RMatrix();
    SL2RMatrix(double a, double b, double c, double d);

    SL2CMatrix complexCast() const;
    void getCoefficients(double &a, double &b, double &c, double &d) const;


private:
    double a, b, c, d;
};

#endif // SL2RMATRIX_H
