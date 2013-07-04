#include "h3point.h"
#include "sl2cmatrix.h"

H3point::H3point()
{    
}


void H3point::getBallCoordinates(double & x, double & y, double & z) const
{
    x = X;
    y = Y;
    z = Z;
    return;
}

void H3point::getHalfSpaceCoordinates(double & x, double & y, double & z) const
{
    double s = X*X + Y*Y + (Z-1.0)*(Z-1.0);

    x = (2.0*X / s);
    y = (2.0*Y / s);
    z = ((2.0*Z - 2.0) / s) + 1.0;
    return;
}

void H3point::getHyperboloidCoordinates(double & x0, double & x1, double & x2, double & x3) const
{
    double s = 1 - X*X - Y*Y - Z*Z;
    x0 = (2 / s) - 1;
    x1 = (2*X / s);
    x2 = (2*Y / s);
    x3 = (2*Z / s);
    return;
}

void H3point::getKleinCoordinates(double & x, double & y, double & z) const
{
    double s = 1 + X*X + Y*Y + Z*Z;
    x = (2.0*X / s);
    y = (2.0*Y / s);
    z = (2.0*Z / s);
    return;
}

void H3point::getHermitianCoordinates(SL2Cmatrix & A) const
{
    double x0,x1,x2,x3;
    getHyperboloidCoordinates(x0,x1,x2,x3);
    A = SL2Cmatrix(complex(x0+x1,0.0),complex(x2,x3),complex(x2,-x3),complex(x0-x1,0.0));
    return;
}


void H3point::setBallCoordinates(double x, double y, double z)
{
    assert(x*x + y*y + z*z < 1.0);
    X = x;
    Y = y;
    Z = z;
    return;
}

void H3point::setHalfSpaceCoordinates(double x, double y, double z)
{
    assert(z>0.0);
    double s = x*x + y*y + (z + 1.0)*(z + 1.0);
    X = (2.0*x / s);
    Y = (2.0*y / s);
    Z = ((2.0*z - 2.0) / s) + 1.0;
}

void H3point::setHyperboloidCoordinates(double x0, double x1, double x2, double x3)
{
    assert(x0 > 0.0 && x1*x1 + x2*x2 + x3*x3 == x0*x0 - 1.0);
    X = x1 / (x0 + 1.0);
    Y = x2 / (x0 + 1.0);
    Z = x3 / (x0 + 1.0);
    return;
}

void H3point::setKleinCoordinates(double x, double y, double z)
{
    assert(x*x + y*y + z*z < 1.0);
    double s = 1.0 + sqrt(1.0 - x*x - y*y - z*z);
    X = (x / s);
    Y = (y / s);
    Z = (z / s);
    return;
}

void H3point::setHermitianCoordinates(const SL2Cmatrix &A)
{
    assert(A.adjoint() == A);
    complex a,b,c,d;
    A.getCoefficients(a,b,c,d);
    double x0,x1,x2,x3;
    x0 = real((a + d) / 2.0);
    x1 = real((a - d) / 2.0);
    x2 = real(b);
    x3 = imag(b);
    setHyperboloidCoordinates(x0,x1,x2,x3);
    return;
}

double H3distance(const H3point & x, const H3point & w)
{
    return 0;
}


std::ostream & operator<<(std::ostream & out, const H3point & p)
{
    out << "( " << p.X << ", " << p.Y << "; " << p.Z << " )";
    return out;
}
