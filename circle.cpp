#include "circle.h"


void Circle::getCenterAndRadius(Complex &centerOut, double &radiusOut) const
{
    centerOut = this->center;
    radiusOut = this->radius;
}

Complex Circle::getCenter() const
{
    return center;
}

double Circle::getRadius() const
{
    return radius;
}

Complex Circle::pointAtAngle(double angle) const
{
    return center + radius*exp(angle*I);
}

bool Circle::contains(const Complex & z) const
{
    return (norm(center - z) == radius*radius);
}

bool Circle::intersectCircles(Circle &C1, Circle &C2, Complex &out1, Complex &out2)
{
    Complex c1, c2;
    double r1, r2;
    C1.getCenterAndRadius(c1,r1);
    C2.getCenterAndRadius(c2,r2);
    if (norm(c1 - c2) >= (r1 + r2)*(r1 + r2))
    {
        return false;
    }
    double theta = acos((r2*r2 + norm(c1 - c2) - r1*r1) / (2.0*r2*std::abs(c1 -c2)));
    out1 = C2.pointAtAngle(arg(c1 - c2) + theta);
    out2 = C2.pointAtAngle(arg(c1 - c2) - theta);
    return true;
}

bool Circle::intersectCircleAndLine(Circle &C, PlanarLine &L, Complex &out1, Complex &out2)
{
    Complex center, point, direction;
    double radius, c1,c2,d1,d2,p1,p2;
    C.getCenterAndRadius(center,radius);
    L.getPointAndDirection(point,direction);
    c1 = real(center);
    c2 = imag(center);
    p1 = real(point);
    p2 = imag(point);
    d1 = real(direction);
    d2 = imag(direction);

    double a = d1*d1 + d2*d2;
    double b = d1*(p1 - c1) + d2*(p2 - c2);
    double c = (p1 -  c1)*(p1 - c1) + (p2 - c2)*(p2 - c2) - radius*radius;

    double Delta = b*b - a*c;
    if (Delta < 0.0)
    {
        throw(QString("Error in global function intersectCircleAndLine"));
        return false;
    }
    double s1 = (-b - sqrt(Delta)) / a;
    double s2 = (-b + sqrt(Delta)) / a;
    out1 = point + s1*direction;
    out2 = point + s2*direction;
    return true;
}
