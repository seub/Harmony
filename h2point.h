#ifndef H2POINT_H
#define H2POINT_H

#include "tools.h"

class H2Point
{
    friend std::ostream & operator<<(std::ostream & out, const H2Point &p);

public:
    H2Point();

    complex getUpperHalfPlaneCoordinate() const;
    complex getDiskCoordinate() const;
    complex getKleinCoordinate() const;
    complex getHyperboloidProjection() const;

    void setUpperHalfPlaneCoordinate(complex z);
    void setDiskCoordinate(complex z);
    void setHyperboloidProjection(complex z);


private:
    complex z;
};

double H2distance(const H2Point & p1, const H2Point & p2);
H2Point H2Midpoint(const H2Point & p1, const H2Point & p2);


#endif // H2POINT_H
