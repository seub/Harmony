#ifndef H3ISOMETRY_H
#define H3ISOMETRY_H


#include "tools.h"
#include "sl2cmatrix.h"
#include "h3point.h"
#include "cp1point.h"

class H3Isometry
{
    friend H3Isometry operator *(const H3Isometry &f1, const H3Isometry &f2);
    friend CP1Point operator *(const H3Isometry & f, const CP1Point & p);
    friend H3Point operator *(const H3Isometry & f, const H3Point & p);
    friend bool operator ==(const H3Isometry & f1, const H3Isometry & f2);
    friend std::ostream & operator<<(std::ostream & out, const H3Isometry & A);

public:
    H3Isometry();
    H3Isometry(int i);
    H3Isometry(const SL2CMatrix &M);


    void setByMappingThreeCP1PointsToZeroOneInfinity(const CP1Point & p1,const CP1Point & p2,const CP1Point & p3);
    void setByImagesOfThreeCP1Points(const CP1Point & p1,const CP1Point & p2,const CP1Point & p3,
                                     const CP1Point & p1out,const CP1Point & p2out,const CP1Point & p3out);
    void setIdentity();
    H3Isometry inverse() const;
    void fixedPointsOnCP1(CP1Point &p1, CP1Point &p2) const;
    bool isParabolic() const;
    bool isElliptic() const;
    bool isLoxodromic() const;
    bool isH2Isometry() const;
    double error() const;

private:
    SL2CMatrix M;

};

#endif // H3ISOMETRY_H
