#ifndef PLANARLINE_H
#define PLANARLINE_H
#include "tools.h"

class PlanarLine
{
public:
    PlanarLine();
    PlanarLine(const complex & p1, const complex & p2);

    void getPointAndDirection(complex & point, complex & direction) const;
    void setPointAndDirection(complex & point, complex & direction);
    void setPerpendicularBisector(const complex & z1, const complex & z2);
    complex getDirection() const;

    bool contains(const complex & z) const;

private:
    complex point;
    complex direction;
};

bool intersectionPlanarLines(const PlanarLine & l1, const PlanarLine & l2, complex & output);

#endif // PLANARLINE_H
