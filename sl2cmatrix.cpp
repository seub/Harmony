#include "sl2cmatrix.h"
#include "sl2rmatrix.h"

SL2CMatrix::SL2CMatrix()
{
    setIdentity();
}

SL2CMatrix::SL2CMatrix(uint i)
{
    assert(i==1);
    setIdentity();
}

SL2CMatrix::SL2CMatrix(const Complex &a, const Complex &b, const Complex &c, const Complex &d) : a(a), b(b), c(c), d(d)
{
    double tol = 0.000000001;
    if (norm(det() - Complex(1.0,0.0)) > tol)
    {
        std::stringstream errorMessage;
        errorMessage << "WARNING in SL2Cmatrix::SL2Cmatrix: the determinant is not 1 (it is equal to " << det() <<" )";
        qDebug() << QString::fromStdString(errorMessage.str());
    }
}

void SL2CMatrix::getCoefficients(Complex & a, Complex & b, Complex & c, Complex & d) const
{
    a = this->a;
    b = this->b;
    c = this->c;
    d = this->d;
}

void SL2CMatrix::setIdentity()
{
    a = 1.0;
    b = 0.0;
    c = 0.0;
    d = 1.0;
}


Complex SL2CMatrix::det() const
{
    return a*d - b*c;
}

Complex SL2CMatrix::trace() const
{
    return a + d;
}

void SL2CMatrix::eigenvalues(Complex & lambda1, Complex & lambda2) const
{
    Complex t = trace();
    Complex delta = sqrt(t*t - 4.0);
    lambda1 = .5*(t + delta);
    lambda2 = .5*(t - delta);
}


bool SL2CMatrix::isReal() const
{
    double tol = 0.000000001;
    return std::abs(imag(a))<tol && std::abs(imag(b))<tol && std::abs(imag(c))<tol && std::abs(imag(d))<tol;
}

double SL2CMatrix::error() const
{
    return norm(a - 1.0) + norm(b) + norm(c) + norm(d - 1.0);
}

SL2CMatrix SL2CMatrix::inverse() const
{
    return SL2CMatrix(d,-b,-c,a);
}

SL2CMatrix SL2CMatrix::transpose() const
{
    return SL2CMatrix(a,c,b,d);
}

SL2CMatrix SL2CMatrix::bar() const
{
    return SL2CMatrix(conj(a),conj(b),conj(c),conj(d));
}

SL2CMatrix SL2CMatrix::adjoint() const
{
    return this->transpose().bar();
}

void SL2CMatrix::getRealPart(SL2RMatrix & output) const
{
    if(!isReal())
    {
        qDebug() << "Warning in getRealPart: some entries are far from real";
    }
    output = SL2RMatrix(real(a),real(b),real(c),real(d));
}


SL2CMatrix operator *(const SL2CMatrix & A1, const SL2CMatrix & A2)
{

    return SL2CMatrix(
                A1.a*A2.a + A1.b*A2.c,
                A1.a*A2.b + A1.b*A2.d,
                A1.c*A2.a + A1.d*A2.c,
                A1.c*A2.b + A1.d*A2.d);
}

CP1Point operator *(const SL2CMatrix & A, const CP1Point & z)
{
    return CP1Point(A.a*z.z1+A.b*z.z2,A.c*z.z1+A.d*z.z2);
}

bool SL2CMatrix::almostEqual(const SL2CMatrix & A1, const SL2CMatrix & A2)
{
    double tol = 0.000000001;
    bool test =
            std::abs(A1.a - A2.a) < tol &&
            std::abs(A1.b - A2.b) < tol &&
            std::abs(A1.c - A2.c) < tol &&
            std::abs(A1.d - A2.d) < tol;
    return test;
}

std::ostream & operator<<(std::ostream & out, const SL2CMatrix & A)
{
    out << "[ " << A.a << ", " << A.b << "; " << A.c << ", " << A.d << " ]";
    return out;
}




