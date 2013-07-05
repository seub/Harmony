#include "h2point.h"

H2point::H2point()
{
}

complex H2point::getUpperHalfPlaneCoordinate() const
{
    return z;
}

complex H2point::getDiskCoordinate() const
{
    return (I*(z - I)/(z + I));
}

void H2point::setUpperHalfPlaneCoordiante(complex z)
{
    this->z = z;
}

void H2point::setDiskCoordinate(complex z)
{
    this->z = (-I*(z + I)/(z - I));
}
