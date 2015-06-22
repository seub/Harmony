#include "h2geodesic.h"
#include "planarline.h"
#include "circle.h"

H2Geodesic::H2Geodesic()
{    
}

void H2Geodesic::setEndpointsInDiskModel(const Complex &z1, const Complex &z2)
{
    if (z1 == z2)
    {
        throw(QString("ERROR in H2Geodesic::setEndpointsInDiskModel: there is no H2Geodesic between a point and itself"));
    }
    this->z1 = z1;
    this->z2 = z2;
}

void H2Geodesic::setPassingThroughTwoPoints(const H2Point & p1, const H2Point & p2)
{
    H2GeodesicArc A(p1, p2);
    *this = A.getGeodesic();
}

void H2Geodesic::getEndpointsInDiskModel(Complex &z1, Complex &z2) const
{
    z1 = this->z1;
    z2 = this->z2;
}

bool H2Geodesic::getCircleAndEndpointsInDiskModel(Complex &centerOut, double &radiusOut, Complex &endpoint1Out, Complex &endpoint2Out) const
{
    endpoint1Out = z1;
    endpoint2Out = z2;

    if (isCircleInDiskModel())
    {
        centerOut = 2.0*(z1*z2)/(z1 + z2);
        radiusOut = abs(z1 - z2)/abs(z1 + z2);
        return true;
    }
    else
    {
        return false;
    }
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
    return (std::abs(imag(z1 / z2)) > 0);
}

bool H2Geodesic::getCircleInDiskModel(Circle &output) const
{
    if (isCircleInDiskModel())
    {
        Complex center = 2.0*(z1*z2)/(z1 + z2);
        double radius = abs(z1 - z2)/abs(z1 + z2);
        output = Circle(center, radius);
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
    H2Geodesic L;
    L.setEndpointsInDiskModel(z2, z1);
    return L;
}

bool H2Geodesic::intersectionH2Geodesics(const H2Geodesic & l1, const H2Geodesic & l2, H2Point & p)
{
    Complex p1,p2;
    if (!H2Geodesic::doIntersect(l1,l2))
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
            Circle::intersectCircles(C1,C2,p1,p2);
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
            if  (Circle::intersectCircleAndLine(C1,L2,p1,p2))
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
            if  (Circle::intersectCircleAndLine(C2,L1,p1,p2))
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

Complex H2Geodesic::closestPointToOriginInDiskModel() const
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
    if (H2Geodesic::commonEndpoint(L1,L2))
    {
        return false;
    } else if (!H2Geodesic::commonPerpendicular(L1,L2,Lperp))
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
    if (H2Geodesic::commonEndpoint(L1,L2))
    {
        return false;
    }
    else if (!H2Geodesic::commonPerpendicular(L1,L2,Lperp))
    {
        intersectionH2Geodesics(L1,L2,q);
        p1 = q;
        return true;
    }
    intersectionH2Geodesics(L1,Lperp,p1);
    return true;
}

bool H2Geodesic::commonPerpendicular(const H2Geodesic &L1, const H2Geodesic &L2, H2Geodesic &output)
{
    H2Point p;
    if (H2Geodesic::intersectionH2Geodesics(L1,L2,p) || H2Geodesic::commonEndpoint(L1,L2))
    {
        return false;
    }
    Complex a1,a2,b1,b2,s1,p1,s2,p2,S,P,delta,c1,c2;
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

    H2Geodesic L;
    L.setEndpointsInDiskModel(c1, c2);
    output = L;
    return true;
}

<<<<<<< HEAD
double H2Geodesic::distanceGeodesics(const H2Geodesic &L1, const H2Geodesic &L2)
{
    H2Point p1,p2;
    if (!closestPoint(L1,L2,p2) || !closestPoint(L2,L1,p1))
    {
        return 0.0;
    }
    return H2Point::distance(p1,p2);
}

bool commonEndpointInDiskModel(const H2Geodesic &L1, const H2Geodesic &L2, Complex &z)
=======
bool H2Geodesic::commonEndpointInDiskModel(const H2Geodesic &L1, const H2Geodesic &L2, Complex &z)
>>>>>>> 1b7e4fbdb4159858088c847450fdf6fa71408048
{
    Complex z1,z2,w1,w2;
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

bool H2Geodesic::commonEndpoint(const H2Geodesic & L1, const H2Geodesic & L2)
{
    Complex z1,z2,w1,w2;
    L1.getEndpointsInDiskModel(z1,z2);
    L2.getEndpointsInDiskModel(w1,w2);
    if (z1 == w1 || z1 == w2 || z2 == w1 || z2 == w2)
    {
        return true;
    }
    return false;
}

bool H2Geodesic::doIntersect(const H2Geodesic &L1, const H2Geodesic &L2)
{
    Complex a1,a2,b1,b2;
    L1.getEndpointsInDiskModel(a1,a2);
    L2.getEndpointsInDiskModel(b1,b2);
    Complex crossRatio = ((b2 - a1) / (b2 - a2))*((b1 - a2)/(b1 - a1));
    return (real(crossRatio) < 0.0);
}




H2GeodesicArc::H2GeodesicArc()
{
}

H2GeodesicArc::H2GeodesicArc(const H2Point &p1, const H2Point &p2) : p1(p1), p2(p2)
{
}

void H2GeodesicArc::setPoints(const H2Point &p1, const H2Point &p2)
{
    this->p1 = p1;
    this->p2 = p2;
}

H2Geodesic H2GeodesicArc::getGeodesic() const
{
    Complex c = getCircleCenterInDiskModel();
    double r = getCircleRadiusInDiskModel();
    Complex z1 = Complex(1.0, -r)/conj(c);
    Complex z2 = Complex(1.0, r)/conj(c);

    H2Geodesic L;
    if (norm(z1 - p1.getDiskCoordinate()) < norm(z2 - p1.getDiskCoordinate()))
    {
        L.setEndpointsInDiskModel(z1, z2);
    }
    else
    {
        L.setEndpointsInDiskModel(z2, z1);
    }
    return L;
}

bool H2GeodesicArc::isLineSegmentInDiskModel() const
{    
    Complex z1 = p1.getDiskCoordinate();
    Complex z2 = p2.getDiskCoordinate();
    return imag(z2*conj(z1))==0;
}

<<<<<<< HEAD
bool H2GeodesicArc::getCircleAndAnglesInDiskModel(Circle &outC, double &outAngle1, double &outAngle2) const
{
    Complex z1 = p1.getDiskCoordinate();
    Complex z2 = p2.getDiskCoordinate();
    if (imag(z2*conj(z1)) != 0.0)
    {
        outC = getCircleInDiskModel();
        outAngle1 = arg(z1 - outC.getCenter());
        outAngle2 = arg(z2 - outC.getCenter());
        return true;
    }
    else
    {
        return false;
    }
}

=======
>>>>>>> 1b7e4fbdb4159858088c847450fdf6fa71408048
void H2GeodesicArc::getEndpointsInDiskModel(Complex &output1, Complex &output2) const
{
    output1 = p1.getDiskCoordinate();
    output2 = p2.getDiskCoordinate();
}

double H2GeodesicArc::length() const
{
    return H2Point::distance(p1, p2);
}

void H2GeodesicArc::getEndpoints(H2Point &output1, H2Point &output2) const
{
    output1 = p1;
    output2 = p2; 
}

Circle H2GeodesicArc::getCircleInDiskModel() const
{
    Complex z1 = p1.getDiskCoordinate(), z2 = p2.getDiskCoordinate();
    Complex c = (z2*(1.0+norm(z1)) - z1*(1.0+norm(z2)))/(2.0*I*imag(conj(z1)*z2));
    double r = sqrt(norm(c) - 1);
    return Circle(c, r);
}

bool H2GeodesicArc::getCircleAndEndpointsInDiskModel(Complex &centerOut, double &radiusOut, Complex &endpoint1Out, Complex &endpoint2Out) const
{
    endpoint1Out = p1.getDiskCoordinate();
    endpoint2Out = p2.getDiskCoordinate();
    if (imag(endpoint2Out*conj(endpoint1Out)) != 0.0)
    {
        centerOut = (endpoint2Out*(1.0+norm(endpoint1Out)) - endpoint1Out*(1.0+norm(endpoint2Out)))/(2.0*I*imag(conj(endpoint1Out)*endpoint2Out));
        radiusOut = sqrt(norm(centerOut) - 1);
        return true;
    }
    else
    {
        return false;
    }
}

Complex H2GeodesicArc::getCircleCenterInDiskModel() const
{
    Complex z1 = p1.getDiskCoordinate(), z2 = p2.getDiskCoordinate();
    return (z2*(1.0+norm(z1)) - z1*(1.0+norm(z2)))/(2.0*I*imag(conj(z1)*z2));
}

double H2GeodesicArc::getCircleRadiusInDiskModel() const
{
    Complex z1 = p1.getDiskCoordinate(), z2 = p2.getDiskCoordinate();
    Complex w = conj(z2)*z1;
    Complex Z = (z2 - z1)*(1.0 - w);
    return std::abs(Z)/(2.0*std::abs(imag(w)));
}

void H2GeodesicArc::getExtremalCoordinatesInHyperboloidProjection(double &xMin, double &xMax, double &yMin, double &yMax)
{
    Complex a,b;
    a = p1.getHyperboloidProjection();
    b = p2.getHyperboloidProjection();
    double a1 = real(a), a2 = imag(a), b1 = real(b), b2 = imag(b), x, y;
    xMin = std::min(a1,b1);
    xMax = std::max(a1,b1);
    yMin = std::min(a2,b2);
    yMax = std::max(a2,b2);

    if (xMin == xMax)
    {
        return;
    }
    if (yMin == yMax)
    {
        return;
    }

    Complex aK = p1.getKleinCoordinate(), bK = p2.getKleinCoordinate();

    double a3 = sqrt(1.0 + a1*a1 + a2*a2);
    double b3 = sqrt(1.0 + b1*b1 + b2*b2);

    double s = a1*b1 + a2*b2 - a3*b3;
    double Tx = (b1 + a1*s)/(a1 + b1*s);
    double Cx = 1.0/(1.0 + Tx*Tx - 2.0*s*Tx);
    double l,m;

    H2Point p;
    Complex w;
    if (Cx > 0.0)
    {
        l = sqrt(Cx);
        m = Tx*l;
        x = l*a1 + m*b1;
        y = l*a2 + m*b2;
        p.setHyperboloidProjection(x+I*y);
        w = p.getKleinCoordinate();

        if (real(conj(w - bK)*(w - aK)) < 0.0)
        {
            if (x<xMin)
            {
                xMin = x;
            }
            if (x > xMax)
            {
                xMax = x;
            }
        }
    }
    double Ty = (b2 + a2*s)/(a2 + b2*s);
    double Cy = 1.0/(1.0 + Ty*Ty - 2.0*s*Ty);

    if (Cy > 0.0)
    {
        l = sqrt(Cy);
        m = Ty*l;
        x = l*a1 + m*b1;
        y = l*a2 + m*b2;
        p.setHyperboloidProjection(Complex(x, y));
        w = p.getKleinCoordinate();

        if (real(conj(w - bK)*(w - aK)) < 0.0)
        {

            if (y<yMin)
            {
                yMin = y;
            }
            if (y > yMax)
            {
                yMax = y;
            }
        }
    }

}


std::ostream & operator<<(std::ostream & out, const H2Geodesic & L)
{
    out << "{z1=" << L.z1 << ", z2=" << L.z2 << "}";
    return out;
}

std::ostream & operator<<(std::ostream & out, const H2GeodesicArc &arc)
{
    out << "{p1=" << arc.p1 << ", p2=" << arc.p2 << "}";
    return out;
}

std::vector<H2Point> H2GeodesicArc::getEvenSubdivision(uint nbCuts) const
{
    std::vector<H2Point> output;
    output.reserve(nbCuts + 2);

    for(uint j=0; j<nbCuts+2; ++j)
    {
        output.push_back(H2Point::proportionalPoint(p1, p2, j*1.0/(nbCuts+1)));
    }
    return output;
}


bool H2GeodesicArc::shareEndpoint(const H2GeodesicArc &L1, const H2GeodesicArc &L2)
{
    H2Point p1,q1,p2,q2;
    L1.getEndpoints(p1,q1);
    L2.getEndpoints(p2,q2);
    return (p1==p2)||(p1==q2)||(q1==p2)||(q1==q2);
}






