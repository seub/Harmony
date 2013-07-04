#include "sl2cmatrix.h"

SL2Cmatrix::SL2Cmatrix()
{
}

SL2Cmatrix::SL2Cmatrix(const complex &a, const complex &b, const complex &c, const complex &d) : a(a), b(b), c(c), d(d)
{
    if (det() != 1.0)
    {
        std::cout << "Warning : the determinant is not 1 (it is equal to" << det() <<")" << std::endl;
        throw(0);
    }
}


void SL2Cmatrix::getCoefficients(complex & a1, complex & a2, complex & a3, complex & a4) const
{
    a1 = a;
    a2 = b;
    a3 = c;
    a4 = d;
    return;
}


complex SL2Cmatrix::det() const
{
    return a*d - b*c;
}

complex SL2Cmatrix::trace() const
{
    return a + d;
}

void SL2Cmatrix::eigenvalues(complex & lambda1, complex & lambda2) const
{
    complex t = trace();
    complex delta = sqrt(t*t - 4.0);
    lambda1 = .5*(t + delta);
    lambda2 = .5*(t - delta);

    return;
}

void SL2Cmatrix::fixedPoints(complex & z1, complex & z2) const
{
    complex delta = sqrt(trace()*trace() - 4.0);
    z1 = (a - d + delta) / (2.0*c);
    z2 = (a - d - delta) / (2.0*c);
}


bool SL2Cmatrix::isReal() const
{
    return imag(a)==0 && imag(b)==0 && imag(c)==0 && imag(d)==0;
}

bool SL2Cmatrix::isParabolic() const
{
    return trace()*trace() == 4.0;
}

bool SL2Cmatrix::isElliptic() const
{
    return imag(trace())==0 && std::abs(real(trace())) < 2.0;
}

bool SL2Cmatrix::isLoxodromic() const
{
    return imag(trace()) != 0 || std::norm(trace()) > 4.0;
}


SL2Cmatrix SL2Cmatrix::inverse() const
{
    return SL2Cmatrix(d,-b,-c,a);
}

SL2Cmatrix SL2Cmatrix::transpose() const
{
    return SL2Cmatrix(a,c,b,d);
}

SL2Cmatrix SL2Cmatrix::conjugate() const
{
    return SL2Cmatrix(conj(a),conj(b),conj(c),conj(d));
}

SL2Cmatrix SL2Cmatrix::adjoint() const
{
    return transpose().conjugate();
}

SL2Cmatrix operator *(const SL2Cmatrix & A1, const SL2Cmatrix & A2)
{
    return SL2Cmatrix(
                A1.a*A2.a + A1.b*A2.c,
                A1.a*A2.b + A1.b*A2.d,
                A1.c*A2.a + A1.d*A2.c,
                A1.c*A2.b + A1.d*A2.d);
}

CP1point operator *(const SL2Cmatrix & A, const CP1point & z)
{
    return CP1point(A.a*z.z1+A.b*z.z2,A.c*z.z1+A.d*z.z2);
}

bool operator ==(const SL2Cmatrix & A1, const SL2Cmatrix & A2)
{
    return A1.a == A2.a && A1.b == A2.b && A1.c == A2.c && A1.d == A2.d;
}


H3point operator *(const SL2Cmatrix & A, const H3point & p)
{
    SL2Cmatrix X;
    H3point q;
    p.getHermitianCoordinates(X);
    q.setHermitianCoordinates(A*X*A.adjoint());
    return q;
}

std::ostream & operator<<(std::ostream & out, const SL2Cmatrix & A)
{
    out << "[ " << A.a << ", " << A.b << "; " << A.c << ", " << A.d << " ]";
    return out;
}




