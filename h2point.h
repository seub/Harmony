#ifndef H2POINT_H
#define H2POINT_H

#include "tools.h"

class H2Point
{
    friend std::ostream & operator<<(std::ostream & out, const H2Point &p);
    friend bool operator ==(H2Point &p1, H2Point &p2);

public:
    H2Point();

    complex getUpperHalfPlaneCoordinate() const;
    complex getDiskCoordinate() const;
    complex getKleinCoordinate() const;
    complex getHyperboloidProjection() const;

    void setUpperHalfPlaneCoordinate(complex z);
    void setDiskCoordinate(complex z);
    void setHyperboloidProjection(complex z);
    void setKleinCoordinate(complex z);

    static double distance(const H2Point & p1, const H2Point & p2);
    static H2Point midpoint(const H2Point & p1, const H2Point & p2);
    static double angle(const H2Point &previous, const H2Point &point, const H2Point &next);

private:
    complex z;
};



#endif // H2POINT_H
