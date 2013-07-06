#ifndef H2POINT_H
#define H2POINT_H

#include "types.h"

class H2Point
{
public:
    H2Point();

    complex getUpperHalfPlaneCoordinate() const;
    complex getDiskCoordinate() const;

    void setUpperHalfPlaneCoordiante(complex z);
    void setDiskCoordinate(complex z);

private:
    complex z;
};

#endif // H2POINT_H
