#include "sl2cmatrix.h"
#include "sl2rmatrix.h"

SL2CMatrix::SL2CMatrix()
{
    setIdentity();
}

SL2CMatrix::SL2CMatrix(const complex &a, const complex &b, const complex &c, const complex &d) : a(a), b(b), c(c), d(d)
{
    if (norm(det() - complex(1.0,0.0)) > ERROR)
    {
        std::cout << "WARNING in SL2Cmatrix::SL2Cmatrix: the determinant is not 1 (it is equal to " << det() <<" )" << std::endl;
        //throw(0);
    }
}

void SL2CMatrix::getCoefficients(complex & a, complex & b, complex & c, complex & d) const
{
    a = this->a;
    b = this->b;
    c = this->c;
    d = this->d;
    return;
}

void SL2CMatrix::setIdentity()
{
    a = 1.0;
    b = 0.0;
    c = 0.0;
    d = 1.0;
    return;
}


complex SL2CMatrix::det() const
{
    return a*d - b*c;
}

complex SL2CMatrix::trace() const
{
    return a + d;
}

void SL2CMatrix::eigenvalues(complex & lambda1, complex & lambda2) const
{
    complex t = trace();
    complex delta = sqrt(t*t - 4.0);
    lambda1 = .5*(t + delta);
    lambda2 = .5*(t - delta);

    return;
}


bool SL2CMatrix::isReal() const
{
    return std::abs(imag(a))<ERROR && std::abs(imag(b))<ERROR && std::abs(imag(c))<ERROR && std::abs(imag(d))<ERROR;
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
    if(isReal())
    {
        output = SL2RMatrix(real(a),real(b),real(c),real(d));
        return;
    }
    else std::cout << "That's not a real matrix." << std::endl;
    throw(0);
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

bool operator ==(const SL2CMatrix & A1, const SL2CMatrix & A2)
{
    bool test =
            std::abs(A1.a - A2.a) < ERROR &&
            std::abs(A1.b - A2.b) < ERROR &&
            std::abs(A1.c - A2.c) < ERROR &&
            std::abs(A1.d - A2.d) < ERROR;
    return test;
}

std::ostream & operator<<(std::ostream & out, const SL2CMatrix & A)
{
    out << "[ " << A.a << ", " << A.b << "; " << A.c << ", " << A.d << " ]";
    return out;
}




