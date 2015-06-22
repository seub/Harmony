#ifndef SL2RMATRIX_H
#define SL2RMATRIX_H

#include "tools.h"
#include "h2point.h"
#include "sl2cmatrix.h"
#include "h2polygon.h"

class SL2RMatrix
{    
    friend SL2RMatrix operator *(const SL2RMatrix & A1, const SL2RMatrix & A2);
    friend H2Point operator *(const SL2RMatrix & A, const H2Point & p);
    friend std::ostream & operator<<(std::ostream & out, const SL2RMatrix & A);

public:
    SL2RMatrix();
    SL2RMatrix(double a, double b, double c, double d);
    SL2RMatrix(uint i);

    static bool almostEqual(const SL2RMatrix & A1, const SL2RMatrix & A2);

    double det() const;
    double error() const;

    SL2CMatrix complexCast() const;
    void getCoefficients(double &a, double &b, double &c, double &d) const;
    void setIdentity();
    SL2RMatrix inverse() const;

private:
    double a, b, c, d;
};

#endif // SL2RMATRIX_H
