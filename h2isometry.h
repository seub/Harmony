#ifndef H2ISOMETRY_H
#define H2ISOMETRY_H

#include "tools.h"
#include "tools.h"
#include "sl2cmatrix.h"
#include "h2point.h"
#include "h2geodesic.h"
#include "h2polygon.h"

class H2Isometry
{
    friend H2Isometry operator *(const H2Isometry & f1, const H2Isometry & f2);
    friend H2Point operator *(const H2Isometry & f, const H2Point & p);
    friend H2Polygon operator *(const H2Isometry &f, const H2Polygon &P);
    friend std::vector<H2Isometry> operator *(const H2Isometry & f, const std::vector<H2Isometry> & listOfIsoms);
    friend std::vector<H2Polygon> operator *(const std::vector<H2Isometry> & listOfIsoms, const H2Polygon &P);
    friend H2Geodesic operator *(const H2Isometry & f, const H2Geodesic & L);
    friend H2GeodesicArc operator *(const H2Isometry & f, const H2GeodesicArc & L);
    friend bool operator ==(const H2Isometry & f1, const H2Isometry & f2);
    friend std::ostream & operator<<(std::ostream & out, const H2Isometry &f);
    friend class FenchelNielsenConstructor;
    friend class PantsTree;
    friend class PantsTreeNode;

public:
    H2Isometry();

    void getDiskCoordinates(complex &u, complex &a) const;

    SL2CMatrix getSU11Matrix() const;
    SL2RMatrix getSL2RMatrix() const;

    void setIdentity();
    void setSU11Matrix(const SL2CMatrix & A);
    void setSL2Rmatrix(const SL2RMatrix & A);
    void setDiskCoordinates(const complex & u, const complex & a);

    void setByNormalizingPairOnLeftHandSide(const H2Isometry &f1, const H2Isometry &f1left);
    void setByNormalizingPairOnRightHandSide(const H2Isometry &f1, const H2Isometry &f1left);

    bool isHyperbolic() const;
    bool isElliptic() const;
    bool isParabolic() const;

    bool axis(H2Geodesic &L) const;

    H2Isometry inverse() const;
    double traceSquared() const;
    double translationLength() const;
    double error() const;

    static H2Isometry findConjugatorForGluing(const H2Isometry & f1, const H2Isometry & f1left,
                                     const H2Isometry & f2, const H2Isometry &f2left, double twistNormalized);

    static H2Isometry identity();

    // We don't use these (for now?)...
    void setTranslationAxisAndLength(const H2Geodesic & axis, double length);
    void setByMappingPointInDiskModelNormalized(const complex & zIn, const complex & zOut);
    void setByMappingGeodesic(const H2Geodesic & L1, const H2Geodesic & L2);

//private:
    complex u,a;

    void setTranslationLengthNormalized(double length);

    void setVerticalTranslation(double t);
    void setVerticalTranslation(double t1, double t2);
    void setVerticalTranslation(double t1, double t2, double t3);

    void setByMappingToVerticalUp(const H2Geodesic & L);
    void setByMappingToVerticalDown(const H2Geodesic & L);

    void fixedPointsInDiskModel(CP1Point &c1, CP1Point &c2) const;

    static double geodesicNormalizer(const H2Geodesic & L);
};

#endif // H2ISOMETRY_H
