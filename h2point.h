#ifndef H2POINT_H
#define H2POINT_H

#include "tools.h"

class H2Point
{
    friend std::ostream & operator<<(std::ostream & out, const H2Point &p);
    friend bool operator ==(H2Point &p1, H2Point &p2);

public:
    H2Point();

    Complex getUpperHalfPlaneCoordinate() const;
    Complex getDiskCoordinate() const;
    Complex getKleinCoordinate() const;
    Complex getHyperboloidProjection() const;
    void getHyperboloidCoordinate(double & x, double & y, double & z) const;

    void setUpperHalfPlaneCoordinate(Complex z);
    void setDiskCoordinate(Complex z);
    void setHyperboloidProjection(Complex z);
    void setKleinCoordinate(Complex z);

    static double distance(const H2Point & p1, const H2Point & p2);
    static H2Point midpoint(const H2Point & p1, const H2Point & p2);
    static double angle(const H2Point &previous, const H2Point &point, const H2Point &next);
    static H2Point centroid(const std::vector<H2Point> & points, const std::vector<double> & weights);

private:
    Complex z;
};



#endif // H2POINT_H
