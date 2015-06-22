#ifndef SL2CMATRIX_H
#define SL2CMATRIX_H

#include "tools.h"
#include "cp1point.h"
#include "h3point.h"

class SL2RMatrix;

class SL2CMatrix
{
    friend class H3Isometry;
    friend SL2CMatrix operator *(const SL2CMatrix & A1, const SL2CMatrix & A2);
    friend CP1Point operator *(const SL2CMatrix & A, const CP1Point & z);
    friend std::ostream & operator<<(std::ostream & out, const SL2CMatrix & A);

public:
    SL2CMatrix();
    SL2CMatrix(const Complex & a, const Complex & b, const Complex & c, const Complex & d);
    SL2CMatrix(uint i);

    static bool almostEqual(const SL2CMatrix & A1, const SL2CMatrix & A2);


    void getCoefficients(Complex & a1, Complex & a2, Complex & a3, Complex & a4) const;
    void setIdentity();

    Complex det() const;
    Complex trace() const;
    void eigenvalues(Complex & lambda1,Complex & lambda2) const;
    void fixedPoints(Complex & z1, Complex & z2) const;
    bool isReal() const;
    double error() const;

    SL2CMatrix inverse() const;
    SL2CMatrix transpose() const;
    SL2CMatrix bar() const;
    SL2CMatrix adjoint() const;


    void getRealPart(SL2RMatrix &output) const;

private:
    Complex a, b, c, d;
};

#endif // SL2CMATRIX_H
