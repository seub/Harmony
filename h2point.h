#ifndef H2POINT_H
#define H2POINT_H

#include "types.h"

class H2Point
{
public:
    H2Point();

    complex getUpperHalfPlaneCoordinate() const;
    complex getDiskCoordinate() const;
    complex getKleinCoordinate() const;

    void setUpperHalfPlaneCoordiante(complex z);
    void setDiskCoordinate(complex z);


private:
    complex z;
};

double H2distance(const H2Point & p1, const H2Point & p2);



#endif // H2POINT_H
