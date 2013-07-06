#ifndef CIRCLE_H
#define CIRCLE_H
#include "types.h"
#include "planarline.h"


class Circle
{
public:
    Circle();
    Circle(complex center, dobule radius);
    Circle(const complex & p1, const complex & p2, const complex & p3);

    bool isIn(const complex & z) const;

private:
    complex center;
    double radius;
};

bool intersectCircles(Circle C1, Circle C2, complex & z);

#endif // CIRCLE_H
