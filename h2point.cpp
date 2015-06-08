#include "h2point.h"

H2Point::H2Point()
{
}

Complex H2Point::getUpperHalfPlaneCoordinate() const
{
    Complex I(0.0, 1.0);
    return -I*((z + I)/(z - I));
}

Complex H2Point::getDiskCoordinate() const
{
    return z;
}

Complex H2Point::getKleinCoordinate() const
{
//    complex z = getDiskCoordinate();
    return 2.0*z / (1.0 + norm(z));
}

Complex H2Point::getHyperboloidProjection() const
{
    return 2.0*z / (1.0 - norm(z));
}

void H2Point::getHyperboloidCoordinate(double &x, double &y, double &z) const
{
    double s = 1.0 - norm(this->z);
    z = (2.0 / s) - 1.0;
    x = (2.0*real(this->z) / s);
    y = (2.0*imag(this->z) / s);
    return;
}

void H2Point::setUpperHalfPlaneCoordinate(Complex z)
{
    Complex I(0.0, 1.0);
    this->z = I*((z - I)/(z + I));
    return;
}

void H2Point::setDiskCoordinate(Complex z)
{
    this->z = z;
    return;
}

void H2Point::setHyperboloidProjection(Complex z)
{
    this->z = z / (1.0 + sqrt(1.0 + norm(z)));
    return;
}

void H2Point::setKleinCoordinate(Complex z)
{
    this->z = z / (1.0 + sqrt(1.0 - norm(z)));
    return;
}

double H2Point::distance(const H2Point & p1, const H2Point & p2)
{
    double s;
    Complex z1,z2;
    z1 = p1.z;
    z2 = p2.z;
    s = 2.0 * (norm(z1 - z2) / ((1 - norm(z1))*(1 - norm(z2))));

    return acosh(1.0 + s);
}

H2Point H2Point::midpoint(const H2Point &p1, const H2Point &p2)
{
    Complex w1 = (p2.z - p1.z)/(1.0 - conj(p1.z)*p2.z);
    Complex w2 = w1*sqrt((1.0 - sqrt(1.0 - norm(w1)))/(norm(w1)*(1.0 + sqrt(1.0 - norm(w1)))));

    H2Point res;
    res.setDiskCoordinate((w2 + p1.z)/(1.0 + conj(p1.z)*w2));
    return res;
}

double H2Point::angle(const H2Point &previous, const H2Point &point, const H2Point &next)
{
    Complex u = (previous.z - point.z) / (1.0 - conj(point.z)*previous.z);
    Complex v = (next.z - point.z) / (1.0 - conj(point.z)*next.z);
    return Tools::mod2Pi(arg(v*conj(u)));
}

std::ostream & operator<<(std::ostream & out, const H2Point &p)
{
    out << "H2Point with disk coordinate " << p.z;
    return out;
}

bool operator ==(H2Point & p1, H2Point & p2)
{
    return p1.z == p2.z;
}

H2Point H2Point::centroid(const std::vector<H2Point> &points, const std::vector<double> &weights)
{
    //assert sum weights = 1
    std::vector<double> X,Y,Z;
    double a,b,c,xout=0.0,yout=0.0,zout=0.0;
    H2Point out;
    X.reserve(points.size());
    Y.reserve(points.size());
    Z.reserve(points.size());
    for(const auto & p: points)
    {
        p.getHyperboloidCoordinate(a,b,c);
        X.push_back(a);
        Y.push_back(b);
        Z.push_back(c);
    }
    for(unsigned int j=0; j<points.size(); ++j)
    {
        xout += weights[j]*X[j];
        yout += weights[j]*Y[j];
        zout += weights[j]*Z[j];
    }
    double s = 1/ sqrt(zout*zout - xout*xout - yout*yout);
    xout = s*xout;
    yout = s*yout;
    out.setHyperboloidProjection(Complex(xout,yout));
    return out;
}

