#ifndef H2GEODESIC_H
#define H2GEODESIC_H

#include "tools.h"
#include "h2point.h"

class Circle;
class PlanarLine;

class H2Geodesic
{    
    friend std::ostream & operator<<(std::ostream & out, const H2Geodesic & L);
public:

    H2Geodesic();

    void setEndpointsInDiskModel(const Complex & z1, const Complex & z2);
    void setPassingThroughTwoPoints(const H2Point & p1, const H2Point & p2);

    void getEndpointsInDiskModel(Complex & z1, Complex & z2) const;

    bool isCircleInDiskModel() const;
    bool getCircleInDiskModel(Circle & output) const;
    bool getLineInDiskModel(PlanarLine & output) const;
    bool getCircleAndAnglesInDiskModel(Circle & outC, double & outAngle1, double & outAngle2) const;
    bool contains(const H2Point & p) const;
    Complex closestPointToOriginInDiskModel() const;
    H2Geodesic swapOrientation() const;

    static bool closestPoints(const H2Geodesic & L1, const H2Geodesic &  L2, H2Point & p1, H2Point & p2);
    static bool closestPoint(const H2Geodesic & L1, const H2Geodesic &  L2, H2Point & p1);

private:
    Complex z1, z2;

};

bool doIntersect(const H2Geodesic & L1, const H2Geodesic & L2);
bool intersectionH2Geodesics(const H2Geodesic & L1, const H2Geodesic & L2, H2Point & p);
bool commonPerpendicular(const H2Geodesic & L1, const H2Geodesic & L2, H2Geodesic &output);
bool commonEndpointInDiskModel(const H2Geodesic & L1, const H2Geodesic & L2, Complex & z);
bool commonEndpoint(const H2Geodesic & L1, const H2Geodesic & L2);


class H2GeodesicArc
{
public:
    H2GeodesicArc();
    H2GeodesicArc(const H2Point & p1, const H2Point & p2);

    void setPoints(const H2Point & p1, const H2Point & p2);
    double length() const;

    Circle getCircleInDiskModel() const;
    Complex getCircleCenterInDiskModel() const;
    double getCircleRadiusInDiskModel() const;

    void getExtremalCoordinatesInHyperboloidProjection(double & xMin, double & xMax, double & yMin, double & yMax);

    H2Geodesic getGeodesic() const;
    bool isLineSegmentInDiskModel() const;
    bool getCircleAndAnglesInDiskModel(Circle & outC, double & outAngle1, double & outAngle2) const;
    void getEndpointsInDiskModel(Complex &output1, Complex &output2) const;
    void getEndpoints(H2Point &output1, H2Point &output2) const;

private:
    H2Point p1, p2;
};

#endif // H2GEODESIC_H
