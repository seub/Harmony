#include "h2isometry.h"
#include "h2point.h"
#include "sl2rmatrix.h"


H2Isometry::H2Isometry()
{
}


void H2Isometry::getDiskCoordinates(complex &u, complex &a) const
{
    u = this->u;
    a = this->a;
    return;
}

void H2Isometry::setDiskCoordinates(const complex &u, const complex &a)
{
    this->u=u;
    this->a=a;
    return;
}

void H2Isometry::setByMappingBoundaryAndInteriorPointsNormalized(const complex &boundaryPoint, const complex &interiorPoint)
{
    u = - (1.0 - conj(interiorPoint)*boundaryPoint)/(boundaryPoint - interiorPoint);
    a = interiorPoint;
    return;
}

void H2Isometry::setByMappingBoundaryAndInteriorPoints(const complex &boundaryPoint1, const complex &boundaryPoint2,
                                                       const complex &interiorPoint1, const complex &interiorPoint2)
{
    H2Isometry f1,f2;
    f1.setByMappingBoundaryAndInteriorPointsNormalized(boundaryPoint1,interiorPoint1);
    f2.setByMappingBoundaryAndInteriorPointsNormalized(boundaryPoint2,interiorPoint2);
    *this = f2.inverse()*f1;
    return;
}

/*void H2Isometry::setByMappingGeodesic(const H2Geodesic &L1, const H2Geodesic &L2)
{
H2Point p1,p2;
complex a1,a2,b1,b2;
L1.getEndpointsInDiskModel(a1,a2);
L2.getEndpointsInDiskModel(b1,b2);
if (intersectionH2Geodesics(L1,L2,p1))
{
setByMappingBoundaryAndInteriorPoints(a1,b1,p1,p1);
return;
}
complex c;
else if (commonEndpoint(L1,L2))
{
H2Isometry f;
complex u;
if (a1 == b1)
{

} else if (a2 == b1)
{

} else if (a1 == b2)
{

} else if (a2 == b2)
{

}
f.setDiskCoordinates();
}
H2Geodesic L3;
commonPerpendicular(L1,L2,L3);
intersectionH2Geodesics(L3,L1,p1);
intersectionH2Geodesics(L3,L2,p2);
this->setByMappingBoundaryAndInteriorPoints(a1,b1,p1.getDiskCoordinate(),p2.getDiskCoordinate());
return;
}*/

SL2CMatrix H2Isometry::getSU11Matrix() const
{
    double s = 1.0 / sqrt(1.0 - norm(a));
    complex t = sqrt(u);
    return SL2CMatrix( s*t , -a*s*t , -conj(a)*s/t , s/t );
}

SL2RMatrix H2Isometry::getSL2RMatrix() const
{
    SL2CMatrix A(1.0/sqrt(2.0),-I/sqrt(2.0),-I/sqrt(2.0),1.0/sqrt(2.0));
    SL2CMatrix out = A*getSU11Matrix()*A.inverse();
    SL2RMatrix output;
    out.getRealPart(output);
    return output;
}

bool H2Isometry::isElliptic() const
{
    return (traceSquared()<4.0);
}

bool H2Isometry::isParabolic() const
{
    return (traceSquared()==4.0);
}

bool H2Isometry::isHyperbolic() const
{
    return (traceSquared()>4.0);
}

double H2Isometry::traceSquared() const
{
    return 2.0*(1.0 + real(u))/(1.0 - norm(a));
}

double H2Isometry::translationLength() const
{
    return acosh(.5*traceSquared() - 1.0);
}

H2Isometry H2Isometry::inverse() const
{
    H2Isometry fOut;
    fOut.setDiskCoordinates(conj(u),-u*a);
    return fOut;
}

void H2Isometry::fixedPointsInDiskModel(CP1Point &c1, CP1Point &c2) const
{
    if (a == 0.0)
    {
        c1 = CP1Point(0.0,1.0);
        c2 = CP1Point(1.0,0.0);
        return;
    }
    complex D = sqrt((u - 1.0)*(u - 1.0) + 4.0*norm(a)*u);
    c1 = CP1Point((1.0 - u + D),(2.0*conj(a)));
    c2 = CP1Point((1.0 - u - D),(2.0*conj(a)));
    return;
}

bool H2Isometry::axis(H2Geodesic & L) const
{
    if (!isHyperbolic())
    {
        return false;
    } else
    {
        complex c1,c2;
        CP1Point C1,C2;
        fixedPointsInDiskModel(C1,C2);
        c1 = C1.getComplexCoordinate();
        c2 = C2.getComplexCoordinate();
        if(norm(1.0 - conj(a)*c2) < norm(1.0 - conj(a)*c1))
        {
            L = H2Geodesic(c2,c1);
            return true;
        } else
        {
            L = H2Geodesic(c1,c2);
            return true;
        }
    }
}

H2Isometry operator *(const H2Isometry & f1, const H2Isometry & f2)
{
    H2Isometry fOut;
    complex temp = 1.0 + f1.a*conj(f2.u*f2.a);
    complex u = f1.u*f2.u*(temp*temp)/norm(temp);
    complex a = (f2.a + (f1.a*conj(f2.u)))/temp;
    fOut.setDiskCoordinates(u,a);
    return fOut;
}

H2Point operator *(const H2Isometry & f, const H2Point & p)
{
    complex z = p.getDiskCoordinate();
    H2Point pOut;
    pOut.setDiskCoordinate(f.u*((z-f.a)/(1.0 - (conj(f.a)*z))));
    return pOut;
}

H2Polygon operator*(const H2Isometry &f, const H2Polygon &P)
{
    H2Polygon res;
    std::vector<H2Point> vertices = P.getVertices();
    for(unsigned int i=0; i<vertices.size(); i++)
    {
        res.addVertex(f*vertices[i]);
    }
    return res;
}

H2Geodesic operator*(const H2Isometry &f, const H2Geodesic &L)
{
    complex z1,z2,w1,w2;
    L.getEndpointsInDiskModel(z1,z2);
    w1 = f.u*((z1-f.a)/(1.0 - (conj(f.a)*z1)));
    w2 = f.u*((z2-f.a)/(1.0 - (conj(f.a)*z2)));
    return H2Geodesic(w1,w2);
}

void H2Isometry::setByMappingEndpointsToPlusOrMinusI(const H2Geodesic &L)
{
    complex a1,a2;
    L.getEndpointsInDiskModel(a1,a2);
    a = L.closestPointToOriginInDiskModel();
    u = I*((conj(a)*(a1 + a2) - 2.0)/(a1 - a2));
    return;
}

void H2Isometry::setByFixingPlusMinusI(const complex & pointMappedToOne)
{
    u = 1.0;
    a = -(1.0 - pointMappedToOne)/(1.0 + pointMappedToOne);
    return;
}

void H2Isometry::setByNormalizingPair(const H2Isometry &f1, const H2Isometry &f2)
{
    H2Isometry fFirst,fSecond;
    H2Geodesic L1,L2;
    f1.axis(L1);
    f2.axis(L2);
    fFirst.setByMappingEndpointsToPlusOrMinusI(L1);
    complex b1,b2;
    L2.getEndpointsInDiskModel(b1,b2);
    fSecond.setByFixingPlusMinusI(fFirst.hitComplexNumberInDiskModel(b1));

    *this = fSecond*fFirst;
    return;
}

complex H2Isometry::hitComplexNumberInDiskModel(const complex &z) const
{
    return u*(z - a)/(1.0 - conj(a)*z);
}









