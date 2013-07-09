#ifndef H2ISOMETRY_H
#define H2ISOMETRY_H
#include "types.h"
#include "sl2cmatrix.h"
#include "h2point.h"
#include "h2geodesic.h"
#include "h2polygon.h"

class H2Isometry
{
    friend H2Isometry operator *(const H2Isometry & f1, const H2Isometry & f2);
    friend H2Point operator *(const H2Isometry & f, const H2Point & p);
    friend H2Polygon operator *(const H2Isometry &f, const H2Polygon &P);
    friend H2Geodesic operator *(const H2Isometry & f, const H2Geodesic & L);

public:
    H2Isometry();

    void getDiskCoordinates(complex &u, complex &a) const;
    SL2CMatrix getSU11Matrix() const;
    SL2RMatrix getSL2RMatrix() const;

    void setByMappingBoundaryAndInteriorPointsNormalized(const complex & boundaryPoint, const complex & interiorPoint);
    void setByMappingBoundaryAndInteriorPoints(const complex & boundaryPoint1, const complex & boundaryPoint2,
                                               const complex & interiorPoint1, const complex & interiorPoint2);

    void setByMappingGeodesic(const H2Geodesic & L1, const H2Geodesic & L2);

    void setByMappingEndpointsToPlusOrMinusI(const H2Geodesic & L);
    void setByFixingPlusMinusI(const complex & pointMappedToOne);
    void setByFixingPlusMinusIWithChosenPoints(const complex & pointIn, const complex & pointOut);
    void setByNormalizingPairWithRepulsivePointAtOne(const H2Isometry & f1, const H2Isometry & f2);
    void setByNormalizingPairWithChosenNearestPointToAxis(const H2Isometry & f1, const H2Isometry & f2, const complex & P);

    void setDiskCoordinates(const complex & u, const complex & a);

    bool isHyperbolic() const;
    bool isElliptic() const;
    bool isParabolic() const;

    void fixedPointsInDiskModel(CP1Point &c1, CP1Point &c2) const;
    bool axis(H2Geodesic &L) const;

    H2Isometry inverse() const;
    double traceSquared() const;
    double translationLength() const;

    complex hitComplexNumberInDiskModel(const complex & z) const;


private:
    complex u,a;
};



#endif // H2ISOMETRY_H
