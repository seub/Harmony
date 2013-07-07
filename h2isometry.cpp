#include "h2isometry.h"
#include "h2point.h"


H2Isometry::H2Isometry()
{
}


void H2Isometry::getDiskCoordinates(complex &u, complex &a) const
{
    u = this->u;
    a = this->a;
    return;
}

void H2Isometry::setDiskCoordinates(const complex &u, const complex &a)
{
    this->u=u;
    this->a=a;
    return;
}

SL2CMatrix H2Isometry::getSU11Matrix() const
{
    double s = 1.0 / sqrt(1.0 - norm(a));
    complex t = sqrt(u);
    return SL2CMatrix( s*t , -a*s*t , -conj(a)*s/t , s/t );
}

SL2RMatrix H2Isometry::getSL2RMatrix() const
{
    SL2CMatrix A(1.0/sqrt(2.0),-I/sqrt(2.0),-I/sqrt(2.0),1.0/sqrt(2.0));
    SL2CMatrix out = A*getSU11Matrix()*A.inverse();
    return out.getRealPart();
}

/*bool H2Isometry::isElliptic() const
{
    return (cosh(a) - sqrt(1.0 - norm(u)) < 2.0);
}

bool H2Isometry::isParabolic() const
{
    return (std::abs(cosh(angle) - sqrt(1.0 - norm(alpha)) - 2.0) < ERROR);
}

bool H2Isometry::isHyperbolic() const
{
    return (std::abs(cosh(angle) - sqrt(1.0 - norm(alpha))) > 2.0);
}

double H2Isometry::translationLength() const
{
    return 2.0*acosh( cosh(angle) / sqrt(1.0 - norm(alpha)) );
}
*/
H2Isometry H2Isometry::inverse() const
{
    H2Isometry fOut;
    fOut.setDiskCoordinates(conj(u),-u*a);
    return fOut;
}


H2Isometry operator *(const H2Isometry & f1, const H2Isometry & f2)
{
    H2Isometry fOut;
    complex temp = 1.0 + f1.a*conj(f2.u*f2.a);
    complex u = f1.u*f2.u*(temp*temp)/norm(temp);
    complex a = (f2.a + (f1.a*conj(f2.u)))/temp;
    fOut.setDiskCoordinates(u,a);
    return fOut;
}

H2Point operator *(const H2Isometry & f, const H2Point & p)
{
    complex z = p.getDiskCoordinate();
    H2Point pOut;
    pOut.setDiskCoordinate(f.u*((z-f.a)/(1.0 - (conj(f.a)*z))));
    return pOut;
}





