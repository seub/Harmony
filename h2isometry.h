#ifndef H2ISOMETRY_H
#define H2ISOMETRY_H
#include "types.h"
#include "sl2cmatrix.h"
#include "h2point.h"
#include "h2geodesic.h"


class H2Isometry
{
    friend H2Isometry operator *(const H2Isometry & f1, const H2Isometry & f2);
    friend H2Point operator *(const H2Isometry & f, const H2Point & p);

public:
    H2Isometry();

    void getDiskCoordinates(complex &u, complex &a) const;
    SL2CMatrix getSU11Matrix() const;
    SL2RMatrix getSL2RMatrix() const;

    void setDiskCoordinates(const complex & u, const complex & a);

    bool isHyperbolic() const;
    bool isElliptic() const;
    bool isParabolic() const;
    double translationLength() const;

    H2Geodesic axis() const;

    H2Isometry inverse() const;


private:
    complex u,a;
};

#endif // H2ISOMETRY_H
