#ifndef PLANARLINE_H
#define PLANARLINE_H
#include "types.h"

class PlanarLine
{
public:
    PlanarLine();
    PlanarLine(const complex & p1, const complex & p2);

    complex getPoint();
    complex getDirection();

    void getPointAndDirection(complex & point0, complex & direction0) const;
    void setPointAndDirection(complex & point0, complex & direction0);
    void setValuesForPerpindicularBisector(const complex & z1, const complex & z2);

    bool isIn(const complex & z) const;

private:
    complex point;
    complex direction;
};

bool intersectionPlanarLines(const PlanarLine & l1, const PlanarLine & l2, complex & output);

#endif // PLANARLINE_H
