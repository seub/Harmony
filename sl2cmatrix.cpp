#include "sl2cmatrix.h"

SL2CMatrix::SL2CMatrix()
{
}

SL2CMatrix::SL2CMatrix(const complex &a, const complex &b, const complex &c, const complex &d) : a(a), b(b), c(c), d(d)
{
    if (norm(det() - complex(1.0,0.0)) > ERROR)
    {
        std::cout << "WARNING in SL2Cmatrix::SL2Cmatrix: the determinant is not 1 (it is equal to " << det() <<" )" << std::endl;
        //throw(0);
    }
}


void SL2CMatrix::getCoefficients(complex & a1, complex & a2, complex & a3, complex & a4) const
{
    a1 = a;
    a2 = b;
    a3 = c;
    a4 = d;
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

void SL2CMatrix::fixedPoints(complex & z1, complex & z2) const
{
    complex delta = sqrt(trace()*trace() - 4.0);
    z1 = (a - d + delta) / (2.0*c);
    z2 = (a - d - delta) / (2.0*c);
}


bool SL2CMatrix::isReal() const
{
    return imag(a)==0 && imag(b)==0 && imag(c)==0 && imag(d)==0;
}

bool SL2CMatrix::isParabolic() const
{
    return trace()*trace() == 4.0;
}

bool SL2CMatrix::isElliptic() const
{
    return imag(trace())==0 && std::abs(real(trace())) < 2.0;
}

bool SL2CMatrix::isLoxodromic() const
{
    return imag(trace()) != 0 || std::norm(trace()) > 4.0;
}


SL2CMatrix SL2CMatrix::inverse() const
{
    return SL2CMatrix(d,-b,-c,a);
}

SL2CMatrix SL2CMatrix::transpose() const
{
    return SL2CMatrix(a,c,b,d);
}

SL2CMatrix SL2CMatrix::conjugate() const
{
    return SL2CMatrix(conj(a),conj(b),conj(c),conj(d));
}

SL2CMatrix SL2CMatrix::adjoint() const
{
    return this->transpose().conjugate();
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


H3Point operator *(const SL2CMatrix & A, const H3Point & p)
{
    SL2CMatrix X;
    H3Point q;
    p.getHermitianCoordinates(X);
    q.setHermitianCoordinates(A*X*(A.adjoint()));

    return q;
}

std::ostream & operator<<(std::ostream & out, const SL2CMatrix & A)
{
    out << "[ " << A.a << ", " << A.b << "; " << A.c << ", " << A.d << " ]";
    return out;
}




