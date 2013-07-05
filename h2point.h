#ifndef H2POINT_H
#define H2POINT_H

#include "types.h"

class H2point
{
public:
    H2point();

    complex getUpperHalfPlaneCoordinate() const;
    complex getDiskCoordinate() const;

    void setUpperHalfPlaneCoordiante(complex z);
    void setDiskCoordinate(complex z);

private:
    complex z;
};

#endif // H2POINT_H
