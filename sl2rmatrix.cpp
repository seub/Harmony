#include "sl2rmatrix.h"

SL2RMatrix::SL2RMatrix()
{
    setIdentity();
}


SL2RMatrix::SL2RMatrix(double a, double b, double c, double d) : a(a), b(b), c(c), d(d)
{
    if (det() != 1.0)
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


H2Point operator*(const SL2RMatrix &A, const H2Point &p)
{
    complex z = p.getUpperHalfPlaneCoordinate();
    double a, b, c, d;
    A.getCoefficients(a, b, c, d);
    complex Z = (a*z + b)/(c*z + d);
    H2Point res;
    res.setUpperHalfPlaneCoordiante(Z);
    return res;
}

H2Polygon operator*(const SL2RMatrix &A, const H2Polygon &P)
{
    H2Polygon res;
    std::vector<H2Point> vertices = P.getVertices();
    for(unsigned int i=0; i<vertices.size(); i++)
    {
        res.addVertex(A*vertices[i]);
    }
    return res;
}

SL2RMatrix operator *(const SL2RMatrix & A1, const SL2RMatrix & A2)
{

    return SL2RMatrix(
                A1.a*A2.a + A1.b*A2.c,
                A1.a*A2.b + A1.b*A2.d,
                A1.c*A2.a + A1.d*A2.c,
                A1.c*A2.b + A1.d*A2.d);
}
