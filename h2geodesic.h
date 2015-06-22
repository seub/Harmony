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
    bool getCircleAndEndpointsInDiskModel(Complex &centerOut, double &radiusOut, Complex &endpoint1Out, Complex &endpoint2Out) const;
    bool getLineInDiskModel(PlanarLine & output) const;
    bool contains(const H2Point & p) const;
    Complex closestPointToOriginInDiskModel() const;
    H2Geodesic swapOrientation() const;

    static bool closestPoints(const H2Geodesic & L1, const H2Geodesic &  L2, H2Point & p1, H2Point & p2);
    static bool closestPoint(const H2Geodesic & L1, const H2Geodesic &  L2, H2Point & p1);
    static double distanceGeodesics(const H2Geodesic & L1, const H2Geodesic & L2);

    static bool doIntersect(const H2Geodesic & L1, const H2Geodesic & L2);
    static bool intersectionH2Geodesics(const H2Geodesic & L1, const H2Geodesic & L2, H2Point & p);
    static bool commonPerpendicular(const H2Geodesic & L1, const H2Geodesic & L2, H2Geodesic &output);
    static bool commonEndpointInDiskModel(const H2Geodesic & L1, const H2Geodesic & L2, Complex & z);
    static bool commonEndpoint(const H2Geodesic & L1, const H2Geodesic & L2);

private:
    Complex z1, z2;
};




class H2GeodesicArc
{
    friend std::ostream & operator<<(std::ostream & out, const H2GeodesicArc &arc);
public:
    H2GeodesicArc();
    H2GeodesicArc(const H2Point & p1, const H2Point & p2);

    void setPoints(const H2Point & p1, const H2Point & p2);
    double length() const;

    Circle getCircleInDiskModel() const;
    Complex getCircleCenterInDiskModel() const;
    double getCircleRadiusInDiskModel() const;
    bool getCircleAndEndpointsInDiskModel(Complex &centerOut, double &radiusOut, Complex &endpoint1Out, Complex &endpoint2Out) const;

    std::vector<H2Point> getEvenSubdivision(uint nbCuts) const;

    void getExtremalCoordinatesInHyperboloidProjection(double & xMin, double & xMax, double & yMin, double & yMax);

    H2Geodesic getGeodesic() const;
    bool isLineSegmentInDiskModel() const;
    void getEndpointsInDiskModel(Complex &output1, Complex &output2) const;
    void getEndpoints(H2Point &output1, H2Point &output2) const;

    static bool shareEndpoint(const H2GeodesicArc &L1, const H2GeodesicArc &L2);

private:
    H2Point p1, p2;
};

#endif // H2GEODESIC_H
