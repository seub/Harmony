#include "planarline.h"

PlanarLine::PlanarLine()
{
}

PlanarLine::PlanarLine(const complex & p1, const complex & p2)
{
    if (p1 == p2)
    {
        std::cout << "You fucked up!" << std::endl;
        throw(0);
    }
    point = p1;
    direction = p1 - p2;
}

complex PlanarLine::getPoint()
{
    return point;
}

complex PlanarLine::getDirection()
{
    return direction;
}

void PlanarLine::getPointAndDirection(complex & point0, complex & direction0) const
{
    point0 = point;
    direction0 = direction;
    return;
}

void PlanarLine::setPointAndDirection(complex & point0, complex & direction0)
{
    if (direction0 == 0.0)
    {
        std::cout << "PlanarLine::setPointAndDirection may not assign direction 0" << std::endl;
        throw(0);
    }
    point = point0;
    direction = direction0;
    return;
}

void PlanarLine::setValuesForPerpindicularBisector(const complex &z1, const complex &z2)
{
    if  (z1 == z2)
    {
        std::cout << "PlanarLine::setValuesForPerpindicularBisector : A point and itself don't have a perpindicular bisector" << std::endl;
        throw(0);
    }
    point = (z1 + z2) / 2.0;
    direction = I*(z1 - z2) / 2.0;
    return;
}

bool PlanarLine::isIn(const complex & z) const
{
    return (imag((z - point) / direction) == 0);
}

bool intersectionPlanarLines(const PlanarLine & l1, const PlanarLine & l2, complex & output)
{
    complex p1, p2, d1, d2;
    l1.getPointAndDirection(p1,d1);
    l2.getPointAndDirection(p2,d2);
    if (imag(d1 / d2) == 0.0 && !l2.isIn(p1))
    {
        return false;
    }

    double t = ((real(d1)*imag(p1 - p2)) + (imag(d1)*real(p1 - p2))) / ((real(d1)*imag(d2)) - (imag(d1)*real(d2)));
    output = p2 + t*d2;
    return true;
}
