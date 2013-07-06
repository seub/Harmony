#ifndef CIRCLE_H
#define CIRCLE_H
#include "types.h"
#include "planarline.h"


class Circle
{
public:
    Circle();
    Circle(complex center, double radius);
    Circle(const complex & p1, const complex & p2, const complex & p3);

    void getCenterAndRadius(complex & center, double &radius) const;

    bool isIn(const complex & z) const;
    complex pointAtAngle(double angle) const;


private:
    complex center;
    double radius;
};

bool intersectCircles(Circle & C1, Circle & C2, complex & out1, complex & out2);
bool intersectCircleAndLine(Circle & C, PlanarLine & L, complex & out1, complex & out2);

#endif // CIRCLE_H
