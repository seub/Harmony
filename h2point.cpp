#include "h2point.h"

H2Point::H2Point()
{
}

complex H2Point::getUpperHalfPlaneCoordinate() const
{
    return -I*((z + I)/(z - I));
}

complex H2Point::getDiskCoordinate() const
{
    return z;
}

complex H2Point::getKleinCoordinate() const
{
//    complex z = getDiskCoordinate();
    return 2.0*z / (1.0 + norm(z));
}

complex H2Point::getHyperboloidProjection() const
{
    return 2.0*z / (1.0 - norm(z));
}

void H2Point::setUpperHalfPlaneCoordinate(complex z)
{
    this->z = I*((z - I)/(z + I));
    return;
}

void H2Point::setDiskCoordinate(complex z)
{
    this->z = z;
    return;
}

void H2Point::setHyperboloidProjection(complex z)
{
    this->z = z / (1.0 + sqrt(1.0 + norm(z)));
    return;
}

void H2Point::setKleinCoordinate(complex z)
{
    this->z = z / (1.0 + sqrt(1.0 - norm(z)));
    return;
}

double H2Point::H2distance(const H2Point & p1, const H2Point & p2)
{
    double s;
    complex z1,z2;
    z1 = p1.getDiskCoordinate();
    z2 = p2.getDiskCoordinate();
    s = 2.0 * (norm(z1 - z2) / ((1 - norm(z1))*(1 - norm(z2))));

    return acosh(1.0 + s);
}

H2Point H2Point::H2MidPoint(const H2Point &p1, const H2Point &p2)
{
    complex z1 = p1.getDiskCoordinate();
    complex z2 = p2.getDiskCoordinate();
    H2Point p;
    complex w1 = (z2 - z1)/(1.0 - conj(z1)*z2);
    complex w2 = w1*sqrt((1.0 - sqrt(1.0 - norm(w1)))/(norm(w1)*(1.0 + sqrt(1.0 - norm(w1)))));
    complex out = (w2 + z1)/(1.0 + conj(z1)*w2);
    p.setDiskCoordinate(out);
    return p;
}

std::ostream & operator<<(std::ostream & out, const H2Point &p)
{
    out << "z= " << p.z;
    return out;
}

bool operator ==(H2Point & p1, H2Point & p2)
{
    return p1.z == p2.z;
}
