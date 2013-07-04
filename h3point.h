#ifndef H3POINT_H
#define H3POINT_H
#include "types.h"


class SL2Cmatrix;

class H3point
{
    friend std::ostream & operator<<(std::ostream & out, const H3point & p);

public:
    H3point();

    void getBallCoordinates(double & x, double & y, double & z) const;
    void getHalfSpaceCoordinates(double & x, double & y, double & z) const;
    void getHyperboloidCoordinates(double & x0, double & x1, double & x2, double & x3) const;
    void getKleinCoordinates(double & x, double & y, double & z) const;
    void getHermitianCoordinates(SL2Cmatrix & A) const;

    void setBallCoordinates(double x, double y, double z);
    void setHalfSpaceCoordinates(double x, double y, double z);
    void setHyperboloidCoordinates(double x0, double x1, double x2, double x3);
    void setKleinCoordinates(double x, double y, double z);
    void setHermitianCoordinates(const SL2Cmatrix & A);


private:
    double X, Y, Z;

};

double H3distance(const H3point & x, const H3point & w);

#endif // H3POINT_H
