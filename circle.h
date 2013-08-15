#ifndef CIRCLE_H
#define CIRCLE_H
#include "tools.h"
#include "planarline.h"


class Circle
{
public:
    Circle();
    Circle(Complex center, double radius);

    Complex getCenter() const;
    double getRadius() const;
    void getCenterAndRadius(Complex & center, double & radius) const;

    bool contains(const Complex & z) const;
    Complex pointAtAngle(double angle) const;


private:
    Complex center;
    double radius;
};

bool intersectCircles(Circle & C1, Circle & C2, Complex & out1, Complex & out2);
bool intersectCircleAndLine(Circle & C, PlanarLine & L, Complex & out1, Complex & out2);

#endif // CIRCLE_H
