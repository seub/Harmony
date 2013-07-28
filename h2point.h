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


    static double H2distance(const H2Point & p1, const H2Point & p2);
    static H2Point H2MidPoint(const H2Point & p1, const H2Point & p2);

private:
    complex z;
};



#endif // H2POINT_H
