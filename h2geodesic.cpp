#include "h2geodesic.h"
#include "planarline.h"
#include "circle.h"
#include "planarline.h"

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
    return;
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
    PlanarLine L;
    getCircleOrLineInDiskModel(outC,L);
    double radius;
    complex center;
    outC.getCenterAndRadius(center,radius);
    double a1,a2;
    a1 = arg(z1 - center);
    a2 = arg(z2 - center);
    outAngle1 = std::min(a1,a2);
    outAngle2 = std::max(a1,a2);
    return true;
}

bool H2Geodesic::contains(const H2Point &p) const
{
    Circle C;
    PlanarLine L;
    if (!getCircleOrLineInDiskModel(C,L))
    {
        return L.contains(p.getDiskCoordinate());
    }
    else return C.contains(p.getDiskCoordinate());
}

bool H2Geodesic::isCircleInDiskModel() const
{
    return (imag(z1 / z2) != 0.0);
}

bool H2Geodesic::getCircleOrLineInDiskModel(Circle & outC, PlanarLine & outL) const
{
    if (!isCircleInDiskModel())
    {
        outL = PlanarLine(z1,z2);
        return false;
    }
    complex center = ((z1 + z2) / abs(z1 + z2))*(2.0/sqrt(4.0 - norm(z1 - z2)));
    double radius = abs(center)*abs(z1 - z2) / 2.0;
    outC.setCenterAndRadius(center,radius);
    return true;
}

bool intersectionH2Geodesics(const H2Geodesic & l1, const H2Geodesic & l2, H2Point & p)
{
    complex p1,p2;
    Circle C1,C2;
    PlanarLine L1,L2;
    l1.getCircleOrLineInDiskModel(C1,L1);
    l2.getCircleOrLineInDiskModel(C2,L2);
    if (l1.isCircleInDiskModel() && l2.isCircleInDiskModel())
    {
        if (intersectCircles(C1,C2,p1,p2))
        {
            if (abs(p1) > abs(p2))
            {
                p.setDiskCoordinate(p2);
            }
            else p.setDiskCoordinate(p1);
            return true;
        } else return false;
    }
    else if (l1.isCircleInDiskModel())
    {
        if  (intersectCircleAndLine(C1,L2,p1,p2))
        {
            if (abs(p1) > abs(p2))
            {
                p.setDiskCoordinate(p2);
            }
            else p.setDiskCoordinate(p1);
            return true;
        } else return false;
    }
    else if (l2.isCircleInDiskModel())
    {
        if  (intersectCircleAndLine(C2,L1,p1,p2))
        {
            if (abs(p1) > abs(p2))
            {
                p.setDiskCoordinate(p2);
            }
            else p.setDiskCoordinate(p1);
            return true;
        } else return false;
    }
    p.setDiskCoordinate(0.0);
    return true;
}


