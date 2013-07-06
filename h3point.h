#ifndef H3POINT_H
#define H3POINT_H
#include "types.h"


class SL2CMatrix;

class H3Point
{
    friend std::ostream & operator<<(std::ostream & out, const H3Point & p);

public:
    H3Point();

    void getBallCoordinates(double & x, double & y, double & z) const;
    void getHalfSpaceCoordinates(double & x, double & y, double & z) const;
    void getHyperboloidCoordinates(double & x0, double & x1, double & x2, double & x3) const;
    void getKleinCoordinates(double & x, double & y, double & z) const;
    void getHermitianCoordinates(SL2CMatrix & A) const;

    void setBallCoordinates(double x, double y, double z);
    void setHalfSpaceCoordinates(double x, double y, double z);
    void setHyperboloidCoordinates(double x0, double x1, double x2, double x3);
    void setKleinCoordinates(double x, double y, double z);
    void setHermitianCoordinates(const SL2CMatrix & A);


private:
    double X, Y, Z;

};

double H3distance(const H3Point & p1, const H3Point & p2);

#endif // H3POINT_H
