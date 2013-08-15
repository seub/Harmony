#include "h2point.h"

H2Point::H2Point()
{
}

complex H2Point::getUpperHalfPlaneCoordinate() const
{
    complex I(0.0, 1.0);
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
    complex I(0.0, 1.0);
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

double H2Point::distance(const H2Point & p1, const H2Point & p2)
{
    double s;
    complex z1,z2;
    z1 = p1.z;
    z2 = p2.z;
    s = 2.0 * (norm(z1 - z2) / ((1 - norm(z1))*(1 - norm(z2))));

    return acosh(1.0 + s);
}

H2Point H2Point::midpoint(const H2Point &p1, const H2Point &p2)
{
    complex w1 = (p2.z - p1.z)/(1.0 - conj(p1.z)*p2.z);
    complex w2 = w1*sqrt((1.0 - sqrt(1.0 - norm(w1)))/(norm(w1)*(1.0 + sqrt(1.0 - norm(w1)))));

    H2Point res;
    res.setDiskCoordinate((w2 + p1.z)/(1.0 + conj(p1.z)*w2));
    return res;
}

double H2Point::angle(const H2Point &previous, const H2Point &point, const H2Point &next)
{
    complex u = (previous.z - point.z) / (1.0 - conj(point.z)*previous.z);
    complex v = (next.z - point.z) / (1.0 - conj(point.z)*next.z);
    return Tools::mod2Pi(arg(v*conj(u)));
}

std::ostream & operator<<(std::ostream & out, const H2Point &p)
{
    out << "H2Point with disk coordinate " << p.z;
    return out;
}

bool operator ==(H2Point & p1, H2Point & p2)
{
    return p1.z == p2.z;
}
