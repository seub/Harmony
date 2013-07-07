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
    double a1,a2;
    a1 = arg(z1 - center);
    a2 = arg(z2 - center);
    outAngle1 = std::min(a1,a2);
    outAngle2 = std::max(a1,a2);
    return true;
}

bool H2Geodesic::contains(const H2Point &p) const
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
    return (imag(z1 / z2) != 0.0);
}

bool H2Geodesic::getCircleInDiskModel(Circle &output) const
{
    if (isCircleInDiskModel())
    {
        complex center = ((z1 + z2) / abs(z1 + z2))*(2.0/sqrt(4.0 - norm(z1 - z2)));
        double radius = abs(center)*abs(z1 - z2) / 2.0;
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

bool intersectionH2Geodesics(const H2Geodesic & l1, const H2Geodesic & l2, H2Point & p)
{
    complex p1,p2;

    if (l1.isCircleInDiskModel())
    {
        Circle C1;
        l1.getCircleInDiskModel(C1);
        if (l2.isCircleInDiskModel())
        {
            Circle C2;
            l2.getCircleInDiskModel(C2);
            if (intersectCircles(C1,C2,p1,p2))
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

bool H2GeodesicArc::isCircleArcInDisModel() const
{
    return H2Geodesic(p1, p2).isCircleInDiskModel();
}

bool H2GeodesicArc::getCircleAndAnglesInDiskModel(Circle &outC, double &outAngle1, double &outAngle2) const
{
    H2Geodesic L(p1, p2);
    if (L.isCircleInDiskModel())
    {
        L.getCircleInDiskModel(outC);
        double a1,a2;
        a1 = arg(p1.getDiskCoordinate() - outC.getCenter());
        a2 = arg(p2.getDiskCoordinate() - outC.getCenter());
        outAngle1 = std::min(a1,a2);
        outAngle2 = std::max(a1,a2);
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
