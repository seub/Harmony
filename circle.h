#ifndef CIRCLE_H
#define CIRCLE_H
#include "tools.h"
#include "planarline.h"


class Circle
{
public:
    Circle() {}
    Circle(Complex center, double radius) : center(center), radius(radius) {}

    Complex getCenter() const;
    double getRadius() const;
    void getCenterAndRadius(Complex &centerOut, double &radiusOut) const;

    bool contains(const Complex &z) const;
    Complex pointAtAngle(double angle) const;

    static bool intersectCircles(Circle &C1, Circle &C2, Complex &out1, Complex &out2);
    static bool intersectCircleAndLine(Circle& C, PlanarLine& L, Complex &out1, Complex &out2);

private:
    Complex center;
    double radius;
};

#endif // CIRCLE_H
