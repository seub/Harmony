#include "h2point.h"

H2Point::H2Point()
{
}

complex H2Point::getUpperHalfPlaneCoordinate() const
{
    return z;
}

complex H2Point::getDiskCoordinate() const
{
    return (I*(z - I)/(z + I));
}

void H2Point::setUpperHalfPlaneCoordiante(complex z)
{
    this->z = z;
}

void H2Point::setDiskCoordinate(complex z)
{
    this->z = (-I*(z + I)/(z - I));
}
