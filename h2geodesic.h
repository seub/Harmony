#ifndef H2GEODESIC_H
#define H2GEODESIC_H
#include "types.h"
#include "h2point.h"

class Circle;
class PlanarLine;

class H2Geodesic
{    
public:

    H2Geodesic();
    H2Geodesic(const complex & z1, const complex & z2);
    H2Geodesic(const H2Point & p1, const H2Point & p2);

    void getEndpointsInDiskModel(complex & z1, complex & z2) const;

    bool getCircleOrLineInDiskModel(Circle & outC, PlanarLine & outL) const;
    bool getCircleAndAnglesInDiskModel(Circle & outC, double & outAngle1, double & outAngle2) const;
    bool contains(const H2Point & p) const;
    bool isCircleInDiskModel() const;

private:
    complex z1, z2;
};

bool intersectionH2Geodesics(const H2Geodesic & l1, const H2Geodesic & l2, H2Point & p);


class H2GeodesicArc
{
public:
    H2GeodesicArc();
    H2GeodesicArc(const H2Point & p1, const H2Point & p2);

    H2Geodesic getGeodesic() const;
    void getCircleAndAnglesInDiskModel(Circle & outC, double & outAngle1, double & outAngle2) const;

private:
    H2Point p1, p2;
};




#endif // H2GEODESIC_H
