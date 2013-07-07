#ifndef SL2CMATRIX_H
#define SL2CMATRIX_H

#include "types.h"
#include "cp1point.h"
#include "h3point.h"
#include "sl2rmatrix.h"

class SL2CMatrix
{
    friend SL2CMatrix operator *(const SL2CMatrix & A1, const SL2CMatrix & A2);
    friend CP1Point operator *(const SL2CMatrix & A, const CP1Point & z);
    friend std::ostream & operator<<(std::ostream & out, const SL2CMatrix & A);
    friend H3Point operator *(const SL2CMatrix & A, const H3Point & p);
    friend bool operator ==(const SL2CMatrix & A1, const SL2CMatrix & A2);

public:
    SL2CMatrix();
    SL2CMatrix(const complex & a, const complex & b, const complex & c, const complex & d);
    SL2CMatrix(const CP1Point & z1in,const CP1Point & z2in,const CP1Point & z3in,
               const CP1Point & z1out,const CP1Point & z2out,const CP1Point & z3out);


    void getCoefficients(complex & a1, complex & a2, complex & a3, complex & a4) const;
    void setIdentity();

    complex det() const;
    complex trace() const;
    void eigenvalues(complex & lambda1,complex & lambda2) const;
    void fixedPoints(complex & z1, complex & z2) const;
    bool isReal() const;
    bool isParabolic() const;
    bool isElliptic() const;
    bool isLoxodromic() const;
    SL2CMatrix inverse() const;
    SL2CMatrix transpose() const;
    SL2CMatrix conjugate() const;
    SL2CMatrix adjoint() const;


    SL2RMatrix getRealPart() const;

private:
    complex a, b, c, d;
};

#endif // SL2CMATRIX_H
