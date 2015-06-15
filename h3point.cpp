#include "h3point.h"
#include "sl2cmatrix.h"

H3Point::H3Point()
{    
}


void H3Point::getBallCoordinates(double & x, double & y, double & z) const
{
    x = X;
    y = Y;
    z = Z;
    return;
}

void H3Point::getHalfSpaceCoordinates(double & x, double & y, double & z) const
{
    //std::cout << "Entering H3point::getHalfSpaceCoordinates" << std::endl;
    double s = X*X + (Y+1.0)*(Y+1.0) + Z*Z;

    x = (2.0*X / s);
    y = (2.0*Z / s);
    z = ((1.0 - X*X - Y*Y - Z*Z) / s);
    return;
}

void H3Point::getHyperboloidCoordinates(double & x0, double & x1, double & x2, double & x3) const
{
    double s = 1.0 - X*X - Y*Y - Z*Z;
    x0 = (2.0 / s) - 1.0;
    x1 = (2.0*X / s);
    x2 = (2.0*Y / s);
    x3 = (2.0*Z / s);
    return;
}

void H3Point::getKleinCoordinates(double & x, double & y, double & z) const
{
    double s = 1.0 + X*X + Y*Y + Z*Z;
    x = (2.0*X / s);
    y = (2.0*Y / s);
    z = (2.0*Z / s);
    return;
}

void H3Point::getHermitianCoordinates(SL2CMatrix & A) const
{
    double x0,x1,x2,x3;
    getHyperboloidCoordinates(x0,x1,x2,x3);
    A = SL2CMatrix(Complex(x0+x1,0.0),Complex(x2,x3),Complex(x2,-x3),Complex(x0-x1,0.0));
    return;
}


void H3Point::setBallCoordinates(double x, double y, double z)
{
    double s = x*x + y*y + z*z;
    if (1.0 - s <= ERROR)
    {
        std::cout << "WARNING in H3point::setBallCoordinates : norm(x,y,z) >= 1 (norm(x,y,z) = " << s << ")" << std::endl;
    }
    X = x;
    Y = y;
    Z = z;
    return;
}

void H3Point::setHalfSpaceCoordinates(double x, double y, double z)
{
    if (z <= ERROR)
    {
        std::cout << "WARNING in H3point::setHalfSpaceCoordinates : z<=0 (z = " << z << ")" << std::endl;
    }
    double s = x*x + y*y + (z + 1.0)*(z + 1.0);
    X = (2.0*x / s);
    Z = -(2.0*y / s);
    Y = ((x*x + y*y + z*z - 1.0) / s);
}

void H3Point::setHyperboloidCoordinates(double x0, double x1, double x2, double x3)
{
    if (x0 <= ERROR)
    {
        std::cout << "WARNING in H3point::setHyperboloidCoordinates : x0<=0 (x0 = " << x0 << ")" << std::endl;
    }
    double q = x1*x1 + x2*x2 + x3*x3 - x0*x0;
    if (std::abs(q + 1.0) > ERROR)
    {
        std::cout << "WARNING in H3point::setHyperboloidCoordinates : q(x) + 1 != 0 (q(x) + 1 = " << q+1 << ")" << std::endl;
    }
    X = x1 / (x0 + 1.0);
    Y = x2 / (x0 + 1.0);
    Z = x3 / (x0 + 1.0);
    return;
}

void H3Point::setKleinCoordinates(double x, double y, double z)
{
    double d = x*x + y*y + z*z;
    if (1.0 - d <= ERROR)
    {
        std::cout << "WARNING in H3point::setKleinCoordinates : norm(x,y,z) >= 1 (norm(x,y,z) = " << d << ")" << std::endl;
    }
    double s = 1.0 + sqrt(1.0 - d);
    X = (x / s);
    Y = (y / s);
    Z = (z / s);
    return;
}

void H3Point::setHermitianCoordinates(const SL2CMatrix &A)
{
    if (!(A.adjoint() == A))
    {
        std::cout << "WARNING in H3point::setHermitianCoordinates : A.adjoint != A" << std::endl;
    }
    Complex a,b,c,d;
    A.getCoefficients(a,b,c,d);
    double x0,x1,x2,x3;
    x0 = real((a + d) / 2.0);
    x1 = real((a - d) / 2.0);
    x2 = real(b);
    x3 = imag(b);
    setHyperboloidCoordinates(x0,x1,x2,x3);
    return;
}

double H3distance(const H3Point & p1, const H3Point & p2)
{
    double X1,Y1,Z1,X2,Y2,Z2,r1,r2,s;
    p1.getBallCoordinates(X1,Y1,Z1);
    p2.getBallCoordinates(X2,Y2,Z2);
    r1 = X1*X1 + Y1*Y1 + Z1*Z1;
    r2 = X2*X2 + Y2*Y2 + Z2*Z2;
    s = 2.0 * (((X1-X2)*(X1-X2) + (Y1-Y2)*(Y1-Y2) + (Z1-Z2)*(Z1-Z2)) / ((1 - r1)*(1 - r2)));

    return acosh(1.0 + s);
}


std::ostream & operator<<(std::ostream & out, const H3Point & p)
{
    out << "( " << p.X << ", " << p.Y << ", " << p.Z << " )";
    return out;
}
