#ifndef H2GEODESIC_H
#define H2GEODESIC_H
#include "types.h"
#include "h2point.h"

class H2Geodesic
{    
public:
    H2Geodesic();
    H2Geodesic(const double & angle1, const double & angle2);
    H2Geodesic(const H2point & p1, const H2point & p2);

    bool isIn(const H2point & p) const;

private:
    double angle1, angle2;
};

bool intersectionH2Geodesics(const H2Geodesic & l1, const H2Geodesic & l2, H2point & p);

#endif // H2GEODESIC_H
