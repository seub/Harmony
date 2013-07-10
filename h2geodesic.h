#ifndef H2GEODESIC_H
#define H2GEODESIC_H

#include "types.h"
#include "h2point.h"

class Circle;
class PlanarLine;

class H2Geodesic
{    
    friend std::ostream & operator<<(std::ostream & out, const H2Geodesic & L);
public:

    H2Geodesic();
    H2Geodesic(const complex & z1, const complex & z2);
    H2Geodesic(const H2Point & p1, const H2Point & p2);


    void getEndpointsInDiskModel(complex & z1, complex & z2) const;

    bool isCircleInDiskModel() const;
    bool getCircleInDiskModel(Circle & output) const;
    bool getLineInDiskModel(PlanarLine & output) const;
    bool getCircleAndAnglesInDiskModel(Circle & outC, double & outAngle1, double & outAngle2) const;
    bool contains(const H2Point & p) const;
    complex closestPointToOriginInDiskModel() const;
    H2Geodesic swapOrientation() const;

    static bool closestPoints(const H2Geodesic & L1, const H2Geodesic &  L2, H2Point & p1, H2Point & p2);
    static bool closestPoint(const H2Geodesic & L1, const H2Geodesic &  L2, H2Point & p1);

private:
    complex z1, z2;

};

bool doIntersect(const H2Geodesic & L1, const H2Geodesic & L2);
bool intersectionH2Geodesics(const H2Geodesic & L1, const H2Geodesic & L2, H2Point & p);
bool commonPerpendicular(const H2Geodesic &  L1, const H2Geodesic & L2, H2Geodesic &output);
bool commonEndpointInDiskModel(const H2Geodesic & L1, const H2Geodesic & L2, complex & z);
bool commonEndpoint(const H2Geodesic & L1, const H2Geodesic & L2);



class H2GeodesicArc
{
public:
    H2GeodesicArc();
    H2GeodesicArc(const H2Point & p1, const H2Point & p2);

    H2Geodesic getGeodesic() const;
    bool isCircleArcInDiskModel() const;
    bool getCircleAndAnglesInDiskModel(Circle & outC, double & outAngle1, double & outAngle2) const;
    void getEndpointsInDiskModel(complex &output1, complex &output2) const;

private:
    H2Point p1, p2;
};




#endif // H2GEODESIC_H
