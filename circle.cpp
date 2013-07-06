#include "circle.h"

Circle::Circle()
{
}

Circle::Circle(complex center, double radius): center(center), radius(radius)
{

}

Circle::Circle(const complex & p1, const complex & p2, const complex & p3)
{
    PlanarLine l12 = PlanarLine(p1,p2);
    if (l12.isIn(p3))
    {
        std::cout << "Error in Circle::Circle(const complex & p1, const complex & p2, const complex & p3) : Points are collinear" << std::endl;
        throw(0);
    }
    PlanarLine perpindicularBisector1, perpindicularBisector2;
    perpindicularBisector1.setValuesForPerpindicularBisector(p1,p2);
    perpindicularBisector2.setValuesForPerpindicularBisector(p2,p3);
    intersectionPlanarLines(perpindicularBisector1,perpindicularBisector2,center);
    radius = abs(center - p1);
}

void Circle::getCenterAndRadius(complex & center0, double & radius0) const
{
    center0 = center;
    radius0 = radius;
    return;
}

complex Circle::pointAtAngle(double angle) const
{
    return center + radius*exp(angle*I);
}

bool Circle::isIn(const complex & z) const
{
    return (norm(center - z) == radius*radius);
}

bool intersectCircles(Circle &C1, Circle &C2, complex & out1, complex & out2)
{
    complex c1, c2;
    double r1, r2;
    C1.getCenterAndRadius(c1,r1);
    C2.getCenterAndRadius(c2,r2);
    if (norm(c1 - c2) >= (r1 + r2)*(r1 + r2))
    {
        return false;
    }
    double theta = acos((r2*r2 + norm(c1 - c2) - r1*r1) / (2.0*r2*abs(c1 -c2)));
    out1 = C2.pointAtAngle(arg(c1 - c2) + theta);
    out2 = C2.pointAtAngle(arg(c1 - c2) - theta);
    return true;
}

bool intersectCircleAndLine(Circle &C, PlanarLine &L, complex &out1, complex &out2)
{
    complex center, point, direction;
    double radius, s,c1,c2,d1,d2,p1,p2;
    C.getCenterAndRadius(center,radius);
    L.getPointAndDirection(point,direction);
    c1 = real(center);
    c2 = imag(center);
    p1 = real(point);
    p2 = imag(point);
    d1 = real(direction);
    d2 = imag(direction);
    double disc = (4*((d1*(p1 - c1)) + (d2*(p2 - c2)))*((d1*(p1 - c1)) + (d2*(p2 - c2)))) -
            4*(d1*d1 + d2*d2)*((p1 -  c1)*(p1 - c1) + (p2 - c2)*(p2 - c2));
    if (disc < 0.0)
    {
        return false;
    }
    s = ( -2*((d1*(p1 - c1)) + (d2*(p2 - c2))) + sqrt(disc)) / ( 2*(d1*d1 + d2*d2) );
    out1 = point + s*direction;
    out2 = point - s*direction;
    return true;
}
