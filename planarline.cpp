#include "planarline.h"

PlanarLine::PlanarLine()
{
}

PlanarLine::PlanarLine(const Complex & p1, const Complex & p2)
{
    if (p1 == p2)
    {
        std::cout << "Planar Line won't draw a line from a point to itself!" << std::endl;
        //throw(0);
        return;
    }
    point = p1;
    direction = p1 - p2;
}


void PlanarLine::getPointAndDirection(Complex & point, Complex & direction) const
{
    point = this->point;
    direction = this->direction;
    return;
}

void PlanarLine::setPointAndDirection(Complex & point, Complex & direction)
{
    if (direction == 0.0)
    {
        std::cout << "PlanarLine::setPointAndDirection may not assign direction 0" << std::endl;
        throw(0);
    }
    this->point = point;
    this->direction = direction;
    return;
}

void PlanarLine::setPerpendicularBisector(const Complex &z1, const Complex &z2)
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

bool PlanarLine::contains(const Complex & z) const
{
    return (imag((z - point) / direction) == 0);
}

Complex PlanarLine::getDirection() const
{
    return direction;
}

bool intersectionPlanarLines(const PlanarLine & l1, const PlanarLine & l2, Complex & output)
{
    Complex p1, p2, d1, d2;
    l1.getPointAndDirection(p1,d1);
    l2.getPointAndDirection(p2,d2);
    if (imag(d1 / d2) == 0.0 && !l2.contains(p1))
    {
        return false;
    }

    double t = ((real(d1)*imag(p1 - p2)) + (imag(d1)*real(p1 - p2))) / ((real(d1)*imag(d2)) - (imag(d1)*real(d2)));
    output = p2 + t*d2;
    return true;
}
