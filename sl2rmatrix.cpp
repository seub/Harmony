#include "sl2rmatrix.h"

SL2RMatrix::SL2RMatrix()
{
    setIdentity();
}


SL2RMatrix::SL2RMatrix(double a, double b, double c, double d) : a(a), b(b), c(c), d(d)
{
    if (std::abs(det() - 1.0)>ERROR)
    {
        std::cout << "WARNING in SL2RMatrix::SL2RMatrix: determinant is not 1 (it is " << det() << ")" << std::endl;
    }
}

void SL2RMatrix::getCoefficients(double &a, double &b, double &c, double &d) const
{
    a = this->a;
    b = this->b;
    c = this->c;
    d = this->d;
    return;
}

double SL2RMatrix::det() const
{
    return a*d - b*c;
}

double SL2RMatrix::error() const
{
    return (a - 1.0)*(a - 1.0) + b*b + c*c + (d - 1.0)*(d- 1.0);
}

SL2CMatrix SL2RMatrix::complexCast() const
{
    return SL2CMatrix(a, b, c, d);
}

void SL2RMatrix::setIdentity()
{
    a = 1.0;
    b = 0.0;
    c = 0.0;
    d = 1.0;
    return;
}

SL2RMatrix SL2RMatrix::inverse() const
{
    return SL2RMatrix(d,-b,-c,a);
}

SL2RMatrix operator *(const SL2RMatrix & A1, const SL2RMatrix & A2)
{

    return SL2RMatrix(
                A1.a*A2.a + A1.b*A2.c,
                A1.a*A2.b + A1.b*A2.d,
                A1.c*A2.a + A1.d*A2.c,
                A1.c*A2.b + A1.d*A2.d);
}

bool operator ==(const SL2RMatrix & A1, const SL2RMatrix & A2)
{
    return A1.a==A2.a && A1.b==A2.b && A1.c==A2.c && A1.d==A2.d;
}

std::ostream & operator<<(std::ostream & out, const SL2RMatrix & A)
{
    out << "[ " << A.a << ", " << A.b << "; " << A.c << ", " << A.d << " ]";
    return out;
}
