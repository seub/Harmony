#include "h2geodesic.h"
#include "planarline.h"
#include "circle.h"

H2Geodesic::H2Geodesic()
{    
}

H2Geodesic::H2Geodesic(const complex &z1, const complex &z2)
{
    if (z1 == z2)
    {
        std::cout << "There is no H2Geodesic between a point and itself." << std::endl;
        throw(0);
    }
    this->z1 = z1;
    this->z2 = z2;
}

H2Geodesic::H2Geodesic(const H2Point & p1, const H2Point & p2)
{
    complex p1Klein, p2Klein;
    p1Klein = p1.getKleinCoordinate();
    p2Klein = p2.getKleinCoordinate();
    PlanarLine L(p1Klein,p2Klein);
    Circle C(0.0,1.0);
    intersectCircleAndLine(C,L,z1,z2);
}

void H2Geodesic::getEndpointsInDiskModel(complex &z1, complex &z2) const
{
    z1 = this->z1;
    z2 = this->z2;
    return;
}

bool H2Geodesic::getCircleAndAnglesInDiskModel(Circle &outC, double &outAngle1, double &outAngle2) const
{
    if (!isCircleInDiskModel())
    {
        return false;
    }
    getCircleInDiskModel(outC);
    double radius;
    complex center;
    outC.getCenterAndRadius(center,radius);
    outAngle1 = arg(z1 - center);
    outAngle2 = arg(z2 - center);
    return true;
}

bool H2Geodesic::contains(const H2Point & p) const
{
    if (isCircleInDiskModel())
    {
        Circle C;
        getCircleInDiskModel(C);
        return C.contains(p.getDiskCoordinate());
    }
    else
    {
        PlanarLine L;
        getLineInDiskModel(L);
        return L.contains(p.getDiskCoordinate());
    }
}

bool H2Geodesic::isCircleInDiskModel() const
{
    return (std::abs(imag(z1 / z2)) > ERROR);
}

bool H2Geodesic::getCircleInDiskModel(Circle &output) const
{
    if (isCircleInDiskModel())
    {
        complex center = 2.0*(z1*z2)/(z1 + z2);
        double radius = abs(z1 - z2)/abs(z1 + z2);
        output.setCenterAndRadius(center,radius);
        return true;
    }
    else
    {
        return false;
    }
}

bool H2Geodesic::getLineInDiskModel(PlanarLine &output) const
{
    if (isCircleInDiskModel())
    {
        return false;
    }
    else
    {
        output = PlanarLine(z1, z2);
        return true;
    }
}

H2Geodesic H2Geodesic::swapOrientation() const
{
    return H2Geodesic(z2,z1);
}

bool intersectionH2Geodesics(const H2Geodesic & l1, const H2Geodesic & l2, H2Point & p)
{
    complex p1,p2;
    if (!doIntersect(l1,l2))
    {
        return false;
    }
    if (l1.isCircleInDiskModel())
    {
        Circle C1;
        l1.getCircleInDiskModel(C1);
        if (l2.isCircleInDiskModel())
        {
            Circle C2;
            l2.getCircleInDiskModel(C2);
            intersectCircles(C1,C2,p1,p2);
            if (abs(p1) > abs(p2))
            {
                p.setDiskCoordinate(p2);
            }
            else
            {
                p.setDiskCoordinate(p1);
            }
            return true;
        }
        else
        {
            PlanarLine L2;
            l2.getLineInDiskModel(L2);
            if  (intersectCircleAndLine(C1,L2,p1,p2))
            {
                if (abs(p1) > abs(p2))
                {
                    p.setDiskCoordinate(p2);
                }
                else
                {
                    p.setDiskCoordinate(p1);
                }
                return true;
            }
            else
            {
                return false;
            }
        }
    }
    else
    {
        PlanarLine L1;
        l1.getLineInDiskModel(L1);
        if (l2.isCircleInDiskModel())
        {
            Circle C2;
            l2.getCircleInDiskModel(C2);
            if  (intersectCircleAndLine(C2,L1,p1,p2))
            {
                if (abs(p1) > abs(p2))
                {
                    p.setDiskCoordinate(p2);
                }
                else
                {
                    p.setDiskCoordinate(p1);
                }
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            p.setDiskCoordinate(0.0);
            return true;
        }

    }
}

complex H2Geodesic::closestPointToOriginInDiskModel() const
{
    if (z1 == -z2)
    {
        return 0.0;
    }
    return ((z1*z2)/(z1 + z2))*(2.0 - abs(z1 - z2));
}

bool H2Geodesic::closestPoints(const H2Geodesic &L1, const H2Geodesic &L2, H2Point &p1, H2Point &p2)
{
    H2Geodesic Lperp;
    H2Point q;
    if (commonEndpoint(L1,L2))
    {
        return false;
    } else if (!commonPerpendicular(L1,L2,Lperp))
    {

        intersectionH2Geodesics(L1,L2,q);
        p2 = q;
        p1 = q;
        return true;
    }
    intersectionH2Geodesics(L1,Lperp,p1);
    intersectionH2Geodesics(L2,Lperp,p2);
    return true;
}

bool H2Geodesic::closestPoint(const H2Geodesic &L1, const H2Geodesic &L2, H2Point &p1)
{
    H2Geodesic Lperp;
    H2Point q;
    if (commonEndpoint(L1,L2))
    {
        return false;
    }
    else if (!commonPerpendicular(L1,L2,Lperp))
    {

        intersectionH2Geodesics(L1,L2,q);
        p1 = q;
        return true;
    }
    intersectionH2Geodesics(L1,Lperp,p1);
    return true;
}

bool commonPerpendicular(const H2Geodesic &L1, const H2Geodesic &L2, H2Geodesic &output)
{
    H2Point p;
    if (intersectionH2Geodesics(L1,L2,p) || commonEndpoint(L1,L2))
    {
        return false;
    }
    complex a1,a2,b1,b2,s1,p1,s2,p2,S,P,delta,c1,c2;
    L1.getEndpointsInDiskModel(a1,a2);
    L2.getEndpointsInDiskModel(b1,b2);
    s1 = a1 + a2;
    s2 = b1 + b2;
    p1 = a1*a2;
    p2 = b1*b2;
    S = 2.0*(p1 - p2)/(s1 - s2);
    P = (s1*p2 - s2*p1)/(s2 - s1);
    delta = sqrt(S*S - 4.0*P);
    c1 = (S - delta)/2.0;
    c2 = (S + delta)/2.0;
    output = H2Geodesic(c1,c2);
    return true;
}

bool commonEndpointInDiskModel(const H2Geodesic &L1, const H2Geodesic &L2, complex &z)
{
    complex z1,z2,w1,w2;
    L1.getEndpointsInDiskModel(z1,z2);
    L2.getEndpointsInDiskModel(w1,w2);
    if (z1 == w1 || z1 == w2)
    {
        z = z1;
        return true;
    }
    if (z2 == w1 || z2 == w2)
    {
        z = z2;
        return true;
    }
    return false;
}

bool commonEndpoint(const H2Geodesic & L1, const H2Geodesic & L2)
{
    complex z1,z2,w1,w2;
    L1.getEndpointsInDiskModel(z1,z2);
    L2.getEndpointsInDiskModel(w1,w2);
    if (z1 == w1 || z1 == w2 || z2 == w1 || z2 == w2)
    {
        return true;
    }
    return false;
}

bool doIntersect(const H2Geodesic &L1, const H2Geodesic &L2)
{
    complex a1,a2,b1,b2;
    L1.getEndpointsInDiskModel(a1,a2);
    L2.getEndpointsInDiskModel(b1,b2);
    complex crossRatio = ((b2 - a1) / (b2 - a2))*((b1 - a2)/(b1 - a1));
    return (real(crossRatio) < 0.0);
}




H2GeodesicArc::H2GeodesicArc()
{
}

H2GeodesicArc::H2GeodesicArc(const H2Point &p1, const H2Point &p2) : p1(p1), p2(p2)
{
}

H2Geodesic H2GeodesicArc::getGeodesic() const
{
    return H2Geodesic(p1, p2);
}

bool H2GeodesicArc::isCircleArcInDiskModel() const
{
    return H2Geodesic(p1, p2).isCircleInDiskModel();
}

bool H2GeodesicArc::getCircleAndAnglesInDiskModel(Circle &outC, double &outAngle1, double &outAngle2) const
{
    H2Geodesic L(p1, p2);
    if (L.isCircleInDiskModel())
    {
        L.getCircleInDiskModel(outC);
        outAngle1 = arg(p1.getDiskCoordinate() - outC.getCenter());
        outAngle2 = arg(p2.getDiskCoordinate() - outC.getCenter());
        return true;
    }
    else
    {
        return false;
    }
}

void H2GeodesicArc::getEndpointsInDiskModel(complex &output1, complex &output2) const
{
    output1 = p1.getDiskCoordinate();
    output2 = p2.getDiskCoordinate();
    return;
}

std::ostream & operator<<(std::ostream & out, const H2Geodesic & L)
{
    out << "{z1=" << L.z1 << ", z2=" << L.z2 << "}";
    return out;
}
