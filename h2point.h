#ifndef H2POINT_H
#define H2POINT_H

#include "tools.h"
#include <tuple>

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
    static H2Point proportionalPoint(const H2Point & p1, const H2Point & p2, const double & s);
    static double angle(const H2Point &previous, const H2Point &point, const H2Point &next);
    static double tanHalfAngle(const H2Point &previous, const H2Point &point, const H2Point &next);
    static H2Point centroid(const std::vector<H2Point> & points, const std::vector<double> & weights);

    void computeAffineWeights(const std::vector<H2Point> &neighbors, std::vector<double> &outputWeights) const;
    void computeNaiveWeights(const std::vector<H2Point> &neighbors, std::vector<double> &outputWeights) const;
    void computeQuadraticWeights(const std::vector<H2Point> &neighbors, std::vector<double> &outputWeights) const;

    bool compareAngles(const H2Point &p1, const H2Point &p2);
private:
    Complex z;
};


#endif // H2POINT_H
