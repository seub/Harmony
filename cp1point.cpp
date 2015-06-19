#include "cp1point.h"

CP1Point::CP1Point()
{
}

CP1Point::CP1Point(const Complex &z)
{
    z1 = z;
    z2 = 1.0;
}

CP1Point::CP1Point(const Complex &z1, const Complex &z2) : z1(z1), z2(z2)
{
}

bool CP1Point::isInfinity() const
{
    return z2 == 0.0;
}

Complex CP1Point::getComplexCoordinate() const
{
    if (!isInfinity())
    {
        return z1 / z2;
    }
    else
    {
        std::cout << "Warning in CP1Point::getComplexCoordinate(): [1 : 0] is not a complex number" << std::endl;
        return Complex(1.0/0.0);
    }
}

bool operator==(const CP1Point & p1, const CP1Point & p2)
{
    Complex test = p1.z1*p2.z2 - p1.z2*p2.z1;
    return test == 0.0;
}

std::ostream & operator<<(std::ostream & out, const CP1Point &p)
{
    if (p.z2 == 0.0)
    {
        out << "infinity";
    }
    else
    {
        out << p.z1 / p.z2 ;
    }
    return out;
}
