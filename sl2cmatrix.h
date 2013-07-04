#ifndef SL2CMATRIX_H
#define SL2CMATRIX_H

#include "types.h"
#include "cp1point.h"
#include "h3point.h"

class SL2Cmatrix
{
    friend SL2Cmatrix operator *(const SL2Cmatrix & A1, const SL2Cmatrix & A2);
    friend CP1point operator *(const SL2Cmatrix & A, const CP1point & z);
    friend std::ostream & operator<<(std::ostream & out, const SL2Cmatrix & A);
    friend H3point operator *(const SL2Cmatrix & A, const H3point & p);
    friend bool operator ==(const SL2Cmatrix & A1, const SL2Cmatrix & A2);

public:
    SL2Cmatrix();
    SL2Cmatrix(const complex & a, const complex & b, const complex & c, const complex & d);

    void getCoefficients(complex & a1, complex & a2, complex & a3, complex & a4) const;

    complex det() const;
    complex trace() const;
    void eigenvalues(complex & lambda1,complex & lambda2) const;
    void fixedPoints(complex & z1, complex & z2) const;
    bool isReal() const;
    bool isParabolic() const;
    bool isElliptic() const;
    bool isLoxodromic() const;
    SL2Cmatrix inverse() const;
    SL2Cmatrix transpose() const;
    SL2Cmatrix conjugate() const;
    SL2Cmatrix adjoint() const;

private:
    complex a, b, c, d;
};

#endif // SL2CMATRIX_H
