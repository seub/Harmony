#include "planarline.h"

PlanarLine::PlanarLine()
{
}

PlanarLine::PlanarLine(const Complex & p1, const Complex & p2)
{
    if (p1 == p2)
    {
        throw(QString("Error in PlanarLine::PlanarLine: identical endpoints"));
    }
    point = p1;
    direction = p1 - p2;
}


void PlanarLine::getPointAndDirection(Complex & point, Complex & direction) const
{
    point = this->point;
    direction = this->direction;
}

void PlanarLine::setPointAndDirection(Complex & point, Complex & direction)
{
    if (direction == 0.0)
    {
       throw(QString("Error in PlanarLine::setPointAndDirection: direction cannot be zero"));
    }
    this->point = point;
    this->direction = direction;
}

void PlanarLine::setPerpendicularBisector(const Complex &z1, const Complex &z2)
{
    if  (z1 == z2)
    {
        throw(QString("Error in PlanarLine::setPerpendicularBisector: identical points"));
    }
    point = (z1 + z2) / 2.0;
    direction = I*(z1 - z2) / 2.0;
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
