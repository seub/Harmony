#include "h3isometry.h"


H3Isometry::H3Isometry()
{
    setIdentity();
}

H3Isometry::H3Isometry(int i)
{
    assert(i==1);
    setIdentity();
}

H3Isometry::H3Isometry(const SL2CMatrix &M)
{
    this->M = M;
}

void H3Isometry::fixedPointsOnCP1(CP1Point &p1, CP1Point &p2) const
{
    if (M.c == 0.0)
    {
        p1 = CP1Point(1.0, 0.0);
        p2 = (M.a == M.d) ? p1 : CP1Point(M.b/(M.d - M.a));
    }
    else
    {
        Complex delta = sqrt(M.trace()*M.trace() - 4.0);
        p1 = CP1Point((M.a - M.d + delta) / (2.0*M.c));
        p2 = CP1Point((M.a - M.d - delta) / (2.0*M.c));
    }
    return;
}

bool H3Isometry::isElliptic() const
{
    Complex tr = M.trace();
    return imag(tr)==0 && std::abs(real(tr)) < 2.0;
}

bool H3Isometry::isH2Isometry() const
{
    return M.isReal();
}

double H3Isometry::error() const
{
    return M.error();
}

bool H3Isometry::isLoxodromic() const
{
    Complex tr = M.trace();
    return imag(tr) != 0 || std::norm(tr) > 4.0;
}

bool H3Isometry::isParabolic() const
{
    Complex tr = M.trace();
    return tr*tr == 4.0;
}

void H3Isometry::setByMappingThreeCP1PointsToZeroOneInfinity(const CP1Point &p1, const CP1Point &p2, const CP1Point &p3)
{
    Complex a, b, c, d, z1, z2, z3;
    if (p1==p2 || p2==p3 || p1==p2)
    {
        std::cout << "ERROR in H3Isometry::setByMappingThreeCP1PointsToZeroOneInfinity: the points are not distinct!" << std::endl;
    }
    else
    {
        if (p1.isInfinity())
        {
            z2 = p2.getComplexCoordinate();
            z3 = p3.getComplexCoordinate();
            a = 0.0;
            b = -(z2 - z3);
            c = -1.0;
            d = z3;
        }
        else if (p2.isInfinity())
        {
            z1 = p1.getComplexCoordinate();
            z3 = p3.getComplexCoordinate();
            a = 1.0;
            b = -z1;
            c = 1.0;
            d = -z3;
        }
        else if (p3.isInfinity())
        {
            z1 = p1.getComplexCoordinate();
            z2 = p2.getComplexCoordinate();
            a = -1.0;
            b = z1;
            c = 0.0;
            d = -(z2 - z1);
        }
        else
        {
            z1 = p1.getComplexCoordinate();
            z2 = p2.getComplexCoordinate();
            z3 = p3.getComplexCoordinate();
            a = z2 - z3;
            b = -z1*(z2 - z3);
            c = z2 - z1;
            d = -z3*(z2 - z1);
        }
    }
    Complex delta = sqrt(a*d - b*c);
    M = SL2CMatrix(a/delta, b/delta, c/delta, d/delta);
}

void H3Isometry::setByImagesOfThreeCP1Points(const CP1Point &p1, const CP1Point &p2, const CP1Point &p3,
                                             const CP1Point &p1out, const CP1Point &p2out, const CP1Point &p3out)
{
    H3Isometry f1, f2;
    f1.setByMappingThreeCP1PointsToZeroOneInfinity(p1, p2, p3);
    f2.setByMappingThreeCP1PointsToZeroOneInfinity(p1out, p2out, p3out);
    *this = f2.inverse()*f1;
    return;
}

void H3Isometry::setIdentity()
{
    M.setIdentity();
}

H3Isometry H3Isometry::inverse() const
{
    return H3Isometry(M.inverse());
}

H3Isometry operator *(const H3Isometry &f1, const H3Isometry &f2)
{
    return H3Isometry(f1.M*f2.M);
}

CP1Point operator *(const H3Isometry & f, const CP1Point & p)
{
    return f.M * p;
}

H3Point operator *(const H3Isometry & f, const H3Point & p)
{
    SL2CMatrix X, A = f.M;
    H3Point q;
    p.getHermitianCoordinates(X);
    q.setHermitianCoordinates(A*X*(A.adjoint()));

    return q;
}

bool operator ==(const H3Isometry & f1, const H3Isometry & f2)
{
    return f1.M == f2.M;
}

std::ostream & operator<<(std::ostream & out, const H3Isometry & A)
{
    out << A.M;
    return out;
}







