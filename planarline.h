#ifndef PLANARLINE_H
#define PLANARLINE_H
#include "tools.h"

class PlanarLine
{
public:
    PlanarLine();
    PlanarLine(const Complex & p1, const Complex & p2);

    void getPointAndDirection(Complex & point, Complex & direction) const;
    void setPointAndDirection(Complex & point, Complex & direction);
    void setPerpendicularBisector(const Complex & z1, const Complex & z2);
    Complex getDirection() const;

    bool contains(const Complex & z) const;

private:
    Complex point;
    Complex direction;
};

bool intersectionPlanarLines(const PlanarLine & l1, const PlanarLine & l2, Complex & output);

#endif // PLANARLINE_H
