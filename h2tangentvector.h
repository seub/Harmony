#ifndef H2TANGENTVECTOR_H
#define H2TANGENTVECTOR_H

#include "tools.h"
#include "h2point.h"

class H2Isometry;

class H2TangentVector
{
    friend std::ostream & operator<<(std::ostream & out, const H2TangentVector &v);
    friend bool operator ==(H2TangentVector &v1, H2TangentVector &v2);
    friend H2TangentVector operator *(const double &t, const H2TangentVector &v);
    friend H2TangentVector operator *(const H2Isometry &f, const H2TangentVector &v);
    friend H2TangentVector operator +(const H2TangentVector & v1, const H2TangentVector & v2);

public:
    H2TangentVector();
    H2TangentVector(const H2Point & root, const Complex & vector);
    H2TangentVector(const H2Point & from, const H2Point & to);
    H2TangentVector(const H2Point & root);
    H2Point getRoot() const;
    Complex getVector() const;
    H2Point exponentiate() const;
    H2Point exponentiateBetter() const;
    H2TangentVector parallelTransport(const double &t);
    H2TangentVector parallelTransportBetter(const double &t);
    double length() const;
    double lengthSquared() const;

    static H2Point exponentiate(const H2TangentVector &v);
    static std::vector<H2Point> exponentiate(const std::vector<H2TangentVector> &V);
    static double scalProd(const H2TangentVector &v1, const H2TangentVector &v2);

private:
    H2Point root;
    Complex vector;
};




#endif // H2TANGENTVECTOR_H
