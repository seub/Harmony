#ifndef H2POINT_H
#define H2POINT_H

#include "tools.h"

class H2TangentVector;

class H2Point
{
    friend std::ostream & operator<<(std::ostream & out, const H2Point &p);
    friend bool operator ==(const H2Point &p1, const H2Point &p2);

public:
    H2Point();

    Complex getUpperHalfPlaneCoordinate() const;
    Complex getDiskCoordinate() const;
    Complex getKleinCoordinate() const;
    void getHyperboloidCoordinate(double &x, double &y, double &z) const;
    Complex getHyperboloidProjection() const;

    void setUpperHalfPlaneCoordinate(Complex z);
    void setDiskCoordinate(Complex z);
    void setHyperboloidProjection(Complex z);
    void setKleinCoordinate(Complex z);
    void weightedLogSum(const std::vector<H2Point> & points, const std::vector<double> & weights, H2TangentVector & output);

    static double distance(const H2Point & p1, const H2Point & p2);
    static H2Point midpoint(const H2Point & p1, const H2Point & p2);
    static double angle(const H2Point &previous, const H2Point &point, const H2Point &next);
    static double tanHalfAngle(const H2Point &previous, const H2Point &point, const H2Point &next);
    static double cotangentAngle(const H2Point &previous, const H2Point &point, const H2Point &next);
    static H2Point centroid(const std::vector<H2Point> & points, const std::vector<double> & weights);

    static H2Point proportionalPoint(const H2Point & p1, const H2Point & p2, const double & s);
    static H2Point exponentialMap(const H2Point &p0, const Complex &u, const double &t);
    static H2Point fromDiskCoordinate(const Complex &z);
    // p0 is the base point, u is a tangent oriented direction (its norm does not matter) represented by a complex number in the disk model, t is the length of the tangent vector

    void computeWeightsCentroid(const std::vector<H2Point> &neighbors, std::vector<double> &outputWeights) const;
    void computeWeightsCentroidNaive(const std::vector<H2Point> &neighbors, std::vector<double> &outputWeights) const;
    void computeWeightsEnergy(const std::vector<H2Point> &neighbors, std::vector<double> &outputWeights) const;

    bool compareAngles(const H2Point &p1, const H2Point &p2);
private:
    Complex z;
};




#endif // H2POINT_H
